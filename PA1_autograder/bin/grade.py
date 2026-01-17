import os
import glob
import subprocess
import shutil
import csv
import json
import re
import time
import zipfile

# --- Configuration Paths ---
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) 
SUBMISSIONS_DIR = os.path.join(BASE_DIR, "submissions")
WORKSPACE_DIR = os.path.join(BASE_DIR, "workspace")
CURRENT_STUDENT_DIR = os.path.join(WORKSPACE_DIR, "current_student")
RESULTS_DIR = os.path.join(BASE_DIR, "results")
FEEDBACK_DIR = os.path.join(RESULTS_DIR, "feedback")
METRICS_DIR = os.path.join(RESULTS_DIR, "metrics")  # New Folder
CONFIG_DIR = os.path.join(BASE_DIR, "config")
WEIGHTAGE_FILE = os.path.join(CONFIG_DIR, "weightage.json")

REQUIRED_BUILD_FILES = [
    os.path.join(CONFIG_DIR, "reference", "driver.cpp"),
    os.path.join(CONFIG_DIR, "reference", "tokens.h"),
    os.path.join(CONFIG_DIR, "reference", "Makefile")
]

# --- Helper: Build Metrics Analysis (from build_logger.py) ---
def analyze_lexer_file(student_dir):
    # Lexer is now at the root of student_dir due to setup_student_env
    lexer_path = os.path.join(student_dir, "lexer.l")
    stats = {
        "flex_rule_count": 0,
        "keyword_count": 0,
        "handled_comments": 0,
        "whitespaces_handled": 0
    }
    
    if not os.path.exists(lexer_path):
        return stats

    try:
        with open(lexer_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()

        # Extract Rules Section
        parts = content.split('%%')
        rules_section = parts[1] if len(parts) >= 2 else ""

        stats["flex_rule_count"] = len(re.findall(r'\n[^\s].*', rules_section))
        stats["keyword_count"] = len(re.findall(r'"(if|else|while|for|return|int|float)"', rules_section))
        stats["handled_comments"] = 1 if ("//" in rules_section) else 0
        stats["whitespaces_handled"] = 1 if ("[ \\t\\r\\n]+" in rules_section) else 0
    except Exception:
        pass
        
    return stats

# --- Helper: Runtime Metrics Parser (from run_tests.py) ---
def parse_lexer_output(output_text):
    metrics = {
        "token_count": 0,
        "unique_lexemes": set(),
        "identifiers": 0,
        "keywords": 0,
        "literals": 0,
        "errors": 0,
        "recovered": False
    }

    lines = output_text.splitlines()
    parsing_tokens = False
    
    for line in lines:
        clean_line = line.strip()
        
        # Detect start of token stream
        if "Line" in clean_line and "Token Type" in clean_line:
            parsing_tokens = True
            continue
        if "----------------" in clean_line:
            continue
        if "Symbol Table" in clean_line or clean_line == "":
            continue

        if parsing_tokens:
            parts = clean_line.split(None, 2)
            if len(parts) < 2: continue 

            token_type = parts[1]
            lexeme = parts[2] if len(parts) > 2 else ""

            metrics["token_count"] += 1
            metrics["unique_lexemes"].add(lexeme)

            if token_type == "IDENTIFIER":
                metrics["identifiers"] += 1
            elif token_type.startswith("KEYWORD_") or token_type.startswith("TYPE_"):
                metrics["keywords"] += 1
            elif token_type.startswith("LITERAL_"):
                metrics["literals"] += 1
            elif token_type == "ERROR" or token_type == "UNKNOWN_TOKEN":
                metrics["errors"] += 1
    
    if metrics["errors"] > 0 and metrics["token_count"] > metrics["errors"]:
        metrics["recovered"] = True

    return metrics

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
    clean_workspace()
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(CURRENT_STUDENT_DIR)
    except Exception as e:
        return False, f"Failed to unzip: {str(e)}"

    # Locate and move lexer.l to root
    lexer_files = glob.glob(os.path.join(CURRENT_STUDENT_DIR, "**", "lexer.l"), recursive=True)
    if not lexer_files:
        return False, "lexer.l not found."
    
    lexer_path = lexer_files[0]
    if os.path.dirname(lexer_path) != CURRENT_STUDENT_DIR:
        shutil.move(lexer_path, os.path.join(CURRENT_STUDENT_DIR, "lexer.l"))

    # Inject Reference Files
    for file_path in REQUIRED_BUILD_FILES:
        if os.path.exists(file_path):
            shutil.copy(file_path, CURRENT_STUDENT_DIR)
    
    return True, "Setup OK"

def compile_project():
    """Compiles and captures build metrics."""
    build_stats = {
        "status": "FAILURE",
        "time_sec": 0.0,
        "warnings": 0,
        "errors": 0,
        "static_analysis": {}
    }

    start_time = time.time()
    try:
        # Static Analysis before compile
        build_stats["static_analysis"] = analyze_lexer_file(CURRENT_STUDENT_DIR)

        # Run Make
        proc = subprocess.run(
            ["make"], cwd=CURRENT_STUDENT_DIR, 
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )
        build_stats["time_sec"] = round(time.time() - start_time, 4)
        
        # Parse stderr for warnings/errors
        build_stats["warnings"] = proc.stderr.count("warning:")
        build_stats["errors"] = proc.stderr.count("error:")

        if proc.returncode == 0:
            build_stats["status"] = "SUCCESS"
            return True, "", build_stats
        else:
            return False, proc.stderr, build_stats
            
    except Exception as e:
        return False, str(e), build_stats

def run_tests(config, research_data):
    """Runs tests and aggregates runtime metrics."""
    feedback_log = []
    total_score = 0.0
    
    # Initialize Research Aggregates
    research_data["runtime"] = {
        "total_tokens": 0,
        "unique_tokens": 0,
        "identifiers": 0,
        "keywords": 0,
        "literals": 0,
        "errors_triggered": 0,
        "error_recovery_count": 0
    }
    global_unique_lexemes = set()

    def run_suite(inputs_pattern, outputs_dir, total_weight, suite_name):
        input_files = glob.glob(os.path.join(BASE_DIR, inputs_pattern, "*.txt"))
        if not input_files: return 0.0

        points_per_test = total_weight / len(input_files)
        suite_score = 0.0
        
        feedback_log.append(f"\n--- {suite_name} Tests ({total_weight}%) ---")
        
        for input_file in sorted(input_files):
            test_name = os.path.basename(input_file)
            expected_output_file = os.path.join(BASE_DIR, outputs_dir, test_name.replace(".txt", "_output.txt"))
            
            executable = os.path.join(CURRENT_STUDENT_DIR, "build", "lexer.exe")
            if not os.path.exists(executable): # Linux fallback
                executable = os.path.join(CURRENT_STUDENT_DIR, "build", "lexer")

            try:
                proc = subprocess.run([executable, input_file], capture_output=True, text=True, timeout=3)
                actual = proc.stdout.strip()

                # --- METRICS COLLECTION ---
                m = parse_lexer_output(actual)
                research_data["runtime"]["total_tokens"] += m["token_count"]
                research_data["runtime"]["identifiers"] += m["identifiers"]
                research_data["runtime"]["keywords"] += m["keywords"]
                research_data["runtime"]["literals"] += m["literals"]
                research_data["runtime"]["errors_triggered"] += m["errors"]
                if m["recovered"]:
                    research_data["runtime"]["error_recovery_count"] += 1
                global_unique_lexemes.update(m["unique_lexemes"])
                # --------------------------

                if os.path.exists(expected_output_file):
                    with open(expected_output_file, 'r') as f:
                        expected = f.read().strip()
                    
                    if actual == expected:
                        suite_score += points_per_test
                        feedback_log.append(f"[PASS] {test_name}")
                    else:
                        feedback_log.append(f"[FAIL] {test_name} (Output Mismatch)")
                else:
                    feedback_log.append(f"[SKIP] {test_name} (Missing Output)")

            except subprocess.TimeoutExpired:
                feedback_log.append(f"[FAIL] {test_name} (Timeout)")
            except Exception as e:
                feedback_log.append(f"[FAIL] {test_name} (Error: {e})")
        
        return suite_score

    s1 = run_suite(config["paths"]["visible_inputs"], config["paths"]["visible_outputs"], config["policy"]["visible_weight"], "Visible")
    s2 = run_suite(config["paths"]["hidden_inputs"], config["paths"]["hidden_outputs"], config["policy"]["hidden_weight"], "Hidden")
    
    # Finalize Unique Count
    research_data["runtime"]["unique_tokens"] = len(global_unique_lexemes)
    
    return s1 + s2, "\n".join(feedback_log)

def main():
    if not os.path.exists(RESULTS_DIR): os.makedirs(RESULTS_DIR)
    if not os.path.exists(FEEDBACK_DIR): os.makedirs(FEEDBACK_DIR)
    if not os.path.exists(METRICS_DIR): os.makedirs(METRICS_DIR)
    
    config = load_config()
    if not config: return

    results_data = []
    submission_zips = glob.glob(os.path.join(SUBMISSIONS_DIR, "*.zip"))

    for zip_path in sorted(submission_zips):
        filename = os.path.basename(zip_path)
        match = re.match(r"([a-zA-Z0-9]+)_PA1\.zip", filename)
        roll_number = match.group(1) if match else filename.replace(".zip", "")
        
        print(f"Grading {roll_number}...")
        
        # Metrics Container for this student
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
            # Compile with Metrics
            compile_success, compile_err, build_stats = compile_project()
            student_metrics["build"] = build_stats
            
            if not compile_success:
                feedback += f"\n[CRITICAL ERROR] Compilation Failed:\n{compile_err}\n"
                results_data.append([roll_number, 0, "Compilation Failed"])
            else:
                # Run Tests with Metrics
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

    # Save Grades CSV
    with open(os.path.join(RESULTS_DIR, "grades.csv"), "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Roll Number", "Score", "Status"])
        writer.writerows(results_data)

    print("Grading Complete.")
    clean_workspace()

if __name__ == "__main__":
    main()