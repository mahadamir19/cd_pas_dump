import os
import glob
import subprocess
import shutil
import csv
import json
import re
import time
import zipfile
import sys

# --- Configuration Paths ---
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) 
SUBMISSIONS_DIR = os.path.join(BASE_DIR, "submissions")
WORKSPACE_DIR = os.path.join(BASE_DIR, "workspace")
CURRENT_STUDENT_DIR = os.path.join(WORKSPACE_DIR, "current_student")
RESULTS_DIR = os.path.join(BASE_DIR, "results")
FEEDBACK_DIR = os.path.join(RESULTS_DIR, "feedback")
METRICS_DIR = os.path.join(RESULTS_DIR, "metrics")
CONFIG_DIR = os.path.join(BASE_DIR, "config")
WEIGHTAGE_FILE = os.path.join(CONFIG_DIR, "weightage.json")

# Files to inject from reference (Note: ast.h is intentionally excluded so we use the student's)
REQUIRED_REF_FILES = [
    os.path.join(CONFIG_DIR, "reference", "driver.cpp"),
    os.path.join(CONFIG_DIR, "reference", "lexer.l"),
    os.path.join(CONFIG_DIR, "reference", "Makefile")
]

NESTING_THRESHOLD = 5

def load_config():
    if not os.path.exists(WEIGHTAGE_FILE):
        return None
    with open(WEIGHTAGE_FILE, 'r') as f:
        return json.load(f)

def clean_workspace():
    if os.path.exists(CURRENT_STUDENT_DIR):
        shutil.rmtree(CURRENT_STUDENT_DIR)
    os.makedirs(CURRENT_STUDENT_DIR)

def setup_student_env(zip_path):
    """
    Unzips student code and injects the reference harness (driver/lexer/Makefile).
    Retains student's parser.y and ast files.
    """
    clean_workspace()
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(CURRENT_STUDENT_DIR)
    except Exception as e:
        return False, f"Failed to unzip: {str(e)}"

    # Flatten directory if nested (move files from subfolder to root)
    # searching for parser.y is a good indicator of the source root
    parser_files = glob.glob(os.path.join(CURRENT_STUDENT_DIR, "**", "parser.y"), recursive=True)
    if not parser_files:
        return False, "parser.y not found in submission."
    
    source_root = os.path.dirname(parser_files[0])
    if source_root != CURRENT_STUDENT_DIR:
        # Move everything from source_root to CURRENT_STUDENT_DIR
        for item in os.listdir(source_root):
            shutil.move(os.path.join(source_root, item), CURRENT_STUDENT_DIR)

    # Inject Reference Files (Overwrites student's driver/lexer/Makefile if present)
    for file_path in REQUIRED_REF_FILES:
        if os.path.exists(file_path):
            shutil.copy(file_path, CURRENT_STUDENT_DIR)
        else:
            print(f"Warning: Reference file missing: {file_path}")

    return True, "Setup OK"

def analyze_parser_file(student_dir):
    """Extracts static metrics from parser.y"""
    parser_path = os.path.join(student_dir, "parser.y")
    if not os.path.exists(parser_path):
        return {}

    try:
        with open(parser_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()

        parts = content.split('%%')
        rules_section = parts[1] if len(parts) >= 2 else ""

        return {
            "grammar_rule_count": len(re.findall(r'^\s*(\w+|\|)', rules_section, re.MULTILINE)),
            "precedence_rules_used": 1 if "%left" in content or "%right" in content else 0,
            "error_token_used": 1 if "error" in rules_section else 0
        }
    except Exception:
        return {}

def analyze_ast_structure(output_text):
    """Calculates tree metrics based on indentation."""
    lines = output_text.splitlines()
    node_count = 0
    max_depth = 0
    
    for line in lines:
        stripped = line.lstrip()
        if not stripped: continue
        # Ignore non-tree lines
        if stripped.startswith("---") or stripped.startswith("Parse"):
            continue
            
        node_count += 1
        indentation = len(line) - len(stripped)
        depth = indentation // 2
        if depth > max_depth:
            max_depth = depth
            
    return node_count, max_depth

def compile_project():
    """Compiles the project and captures build metrics."""
    build_stats = {
        "status": "FAILURE",
        "compile_time": 0.0,
        "shift_reduce_conflicts": 0,
        "reduce_reduce_conflicts": 0,
        "error_count": 0
    }

    start_time = time.time()
    # Capture stderr because Bison prints conflicts there
    proc = subprocess.run(
        ["make"], cwd=CURRENT_STUDENT_DIR, 
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    build_stats["compile_time"] = round(time.time() - start_time, 4)
    
    # Parse Bison Conflicts
    sr_match = re.search(r'(\d+) shift/reduce conflict', proc.stderr)
    if sr_match: build_stats["shift_reduce_conflicts"] = int(sr_match.group(1))

    rr_match = re.search(r'(\d+) reduce/reduce conflict', proc.stderr)
    if rr_match: build_stats["reduce_reduce_conflicts"] = int(rr_match.group(1))

    build_stats["error_count"] = proc.stderr.count("error:")

    if proc.returncode == 0:
        build_stats["status"] = "SUCCESS"
        # Add static analysis
        build_stats.update(analyze_parser_file(CURRENT_STUDENT_DIR))
        return True, "", build_stats
    else:
        return False, proc.stderr, build_stats

def run_tests(config, research_data):
    """Runs all test cases and calculates score."""
    feedback_log = []
    total_score = 0.0
    
    # Initialize Runtime Metrics
    research_data["runtime"] = {
        "total_ast_nodes": 0,
        "max_tree_depth": 0,
        "panic_recoveries": 0,
        "nested_constructs_handled": False,
        "syntax_errors_handled": False
    }

    def run_suite(inputs_pattern, outputs_dir, total_weight, suite_name):
        input_files = glob.glob(os.path.join(BASE_DIR, inputs_pattern, "*.txt"))
        if not input_files: return 0.0

        points_per_test = total_weight / len(input_files)
        suite_score = 0.0
        
        feedback_log.append(f"\n--- {suite_name} Tests ({total_weight}%) ---")
        
        for input_file in sorted(input_files):
            test_name = os.path.basename(input_file)
            expected_output_file = os.path.join(BASE_DIR, outputs_dir, test_name.replace(".txt", "_output.txt"))
            
            executable = os.path.join(CURRENT_STUDENT_DIR, "build", "parser")
            if not os.path.exists(executable):
                 executable = os.path.join(CURRENT_STUDENT_DIR, "build", "parser.exe")

            try:
                with open(input_file, "r") as f_in:
                    # Run Parser
                    proc = subprocess.run(
                        [executable], stdin=f_in, capture_output=True, text=True, timeout=3
                    )
                
                actual = proc.stdout.strip().replace("\r\n", "\n")
                
                # --- Metrics Collection ---
                nodes, depth = analyze_ast_structure(actual)
                research_data["runtime"]["total_ast_nodes"] += nodes
                if depth > research_data["runtime"]["max_tree_depth"]:
                    research_data["runtime"]["max_tree_depth"] = depth
                
                # Panic Mode Detection (Bison prints syntax errors to stderr)
                if "syntax error" in proc.stderr:
                    research_data["runtime"]["syntax_errors_handled"] = True
                    # Heuristic: If output contains partial AST or "Recovered", count it
                    if nodes > 0: 
                        research_data["runtime"]["panic_recoveries"] += 1

                # --------------------------

                if os.path.exists(expected_output_file):
                    with open(expected_output_file, 'r') as f_exp:
                        expected = f_exp.read().strip().replace("\r\n", "\n")
                    
                    if actual == expected:
                        suite_score += points_per_test
                        feedback_log.append(f"[PASS] {test_name}")
                        
                        # If passed a deep test, assume nesting is handled
                        if depth >= NESTING_THRESHOLD:
                            research_data["runtime"]["nested_constructs_handled"] = True
                    else:
                        feedback_log.append(f"[FAIL] {test_name} (Output Mismatch)")
                else:
                    feedback_log.append(f"[SKIP] {test_name} (Missing Expected Output)")

            except subprocess.TimeoutExpired:
                feedback_log.append(f"[FAIL] {test_name} (Timeout)")
            except Exception as e:
                feedback_log.append(f"[FAIL] {test_name} (Error: {e})")
        
        return suite_score

    # Run Visible
    s1 = run_suite(config["paths"]["visible_inputs"], config["paths"]["visible_outputs"], 
                   config["policy"]["visible_weight"], "Visible")
    # Run Hidden
    s2 = run_suite(config["paths"]["hidden_inputs"], config["paths"]["hidden_outputs"], 
                   config["policy"]["hidden_weight"], "Hidden")

    return s1 + s2, "\n".join(feedback_log)

def main():
    # Ensure directories exist
    for d in [RESULTS_DIR, FEEDBACK_DIR, METRICS_DIR]:
        if not os.path.exists(d): os.makedirs(d)
    
    config = load_config()
    if not config: 
        print("Config missing.")
        return

    results_data = []
    submission_zips = glob.glob(os.path.join(SUBMISSIONS_DIR, "*.zip"))
    print(f"Found {len(submission_zips)} submissions.")

    for zip_path in sorted(submission_zips):
        filename = os.path.basename(zip_path)
        # Extract Roll Number: 27100289_PA2.zip -> 27100289
        match = re.match(r"([a-zA-Z0-9]+)_PA2\.zip", filename)
        roll_number = match.group(1) if match else filename.replace(".zip", "")
        
        print(f"Grading {roll_number}...")
        
        student_metrics = {
            "roll_number": roll_number,
            "build": {},
            "runtime": {},
            "score": 0
        }

        score = 0
        feedback = f"Grading Report for {roll_number}\n{'='*30}\n"
        
        success, msg = setup_student_env(zip_path)
        if not success:
            feedback += f"\n[CRITICAL ERROR] Setup Failed: {msg}\n"
            results_data.append([roll_number, 0, "Setup Failed"])
        else:
            # Compile
            compile_success, compile_err, build_stats = compile_project()
            student_metrics["build"] = build_stats
            
            if not compile_success:
                feedback += f"\n[CRITICAL ERROR] Compilation Failed:\n{compile_err}\n"
                results_data.append([roll_number, 0, "Compilation Failed"])
            else:
                # Run Tests
                score, logs = run_tests(config, student_metrics)
                feedback += logs
                results_data.append([roll_number, round(score, 2), "Success"])

        student_metrics["score"] = round(score, 2)
        feedback += f"\n\nFINAL SCORE: {student_metrics['score']} / 100"

        # Save Feedback
        with open(os.path.join(FEEDBACK_DIR, f"{roll_number}.txt"), "w") as f:
            f.write(feedback)
            
        # Save Metrics JSON
        metrics_file = os.path.join(METRICS_DIR, f"{roll_number}_metrics.json")
        with open(metrics_file, "w") as f:
            json.dump(student_metrics, f, indent=4)

    # Save CSV
    with open(os.path.join(RESULTS_DIR, "grades.csv"), "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Roll Number", "Score", "Status"])
        writer.writerows(results_data)

    print("Grading Complete.")
    clean_workspace()

if __name__ == "__main__":
    main()