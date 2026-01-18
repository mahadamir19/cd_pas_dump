import os
import glob
import subprocess
import shutil
import csv
import json
import re
import time
import zipfile
from difflib import SequenceMatcher

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

# PA3 Reference Files (These overwrite student files to ensure harness correctness)
REQUIRED_REF_FILES = [
    os.path.join(CONFIG_DIR, "reference", "driver.cpp"),
    os.path.join(CONFIG_DIR, "reference", "lexer.l"),
    os.path.join(CONFIG_DIR, "reference", "parser.y"),
    os.path.join(CONFIG_DIR, "reference", "ast.cpp"),
    os.path.join(CONFIG_DIR, "reference", "ast.h"),
    os.path.join(CONFIG_DIR, "reference", "Makefile")
]

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
    Unzips student code, flattens structure, and injects reference harness.
    """
    clean_workspace()
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(CURRENT_STUDENT_DIR)
    except Exception as e:
        return False, f"Failed to unzip: {str(e)}"

    # Flatten directory if nested (look for semantics.cpp as anchor)
    anchor_files = glob.glob(os.path.join(CURRENT_STUDENT_DIR, "**", "semantics.cpp"), recursive=True)
    if not anchor_files:
        return False, "semantics.cpp not found in submission."
    
    source_root = os.path.dirname(anchor_files[0])
    if source_root != CURRENT_STUDENT_DIR:
        for item in os.listdir(source_root):
            shutil.move(os.path.join(source_root, item), CURRENT_STUDENT_DIR)

    # Inject Reference Files (Overwrites student's parser/lexer/driver/Makefile)
    for file_path in REQUIRED_REF_FILES:
        if os.path.exists(file_path):
            shutil.copy(file_path, CURRENT_STUDENT_DIR)
        else:
            print(f"Warning: Reference file missing: {file_path}")

    return True, "Setup OK"

def compile_project():
    """Compiles the project and captures build metrics."""
    build_stats = {
        "status": "FAILURE",
        "compile_time": 0.0,
        "error_count": 0
    }

    start_time = time.time()
    # Using 'make' assuming the PA3 Makefile creates an executable named 'compiler'
    proc = subprocess.run(
        ["make"], cwd=CURRENT_STUDENT_DIR, 
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    build_stats["compile_time"] = round(time.time() - start_time, 4)
    build_stats["error_count"] = proc.stderr.count("error:")

    if proc.returncode == 0:
        build_stats["status"] = "SUCCESS"
        return True, "", build_stats
    else:
        return False, proc.stderr, build_stats

def calculate_similarity(actual, expected):
    """Calculates a similarity score (0-100) between two strings."""
    return SequenceMatcher(None, actual, expected).ratio() * 100

def parse_compiler_output(output_text):
    """Parses stdout for Semantic Errors and IR Metrics."""
    metrics = {
        "semantic_error_count": 0,
        "semantic_error_types": {},
        "ir_instruction_count": 0,
        "temps_used": 0,
        "labels_generated": 0,
        "loops_decisions_count": 0,
        "ir_content": ""
    }

    lines = output_text.splitlines()
    in_ir_phase = False
    ir_lines = []
    
    sem_error_pattern = re.compile(r"Error at line \d+: (.*)")
    temp_pattern = re.compile(r"\bt\d+\b")
    label_pattern = re.compile(r"\bL\d+\b")
    
    unique_temps = set()
    unique_labels = set()

    for line in lines:
        clean_line = line.strip()
        
        # 1. Analyze Semantic Errors
        err_match = sem_error_pattern.match(clean_line)
        if err_match:
            metrics["semantic_error_count"] += 1
            error_msg = err_match.group(1).strip()
            metrics["semantic_error_types"][error_msg] = metrics["semantic_error_types"].get(error_msg, 0) + 1
            continue

        # 2. Detect Phase Boundaries
        if "--- IR Generation ---" in clean_line: # Updated to match typical PA3 driver output
            in_ir_phase = True
            continue

        # 3. Analyze IR Generation
        if in_ir_phase and clean_line:
            if "No IR generated" in clean_line or clean_line.startswith("---"):
                continue
                
            ir_lines.append(clean_line)
            metrics["ir_instruction_count"] += 1
            
            unique_temps.update(temp_pattern.findall(clean_line))
            unique_labels.update(label_pattern.findall(clean_line))
            
            if clean_line.startswith("IFZ") or clean_line.startswith("GOTO"):
                metrics["loops_decisions_count"] += 1

    metrics["temps_used"] = len(unique_temps)
    metrics["labels_generated"] = len(unique_labels)
    metrics["ir_content"] = "\n".join(ir_lines)
    
    return metrics

def run_tests(config, research_data):
    """Runs all test cases."""
    feedback_log = []
    total_score = 0.0
    
    research_data["runtime"] = {
        "total_semantic_errors": 0,
        "semantic_error_distribution": {},
        "total_ir_instructions": 0,
        "max_temps_used": 0,
        "total_labels_generated": 0,
        "total_decisions_loops": 0,
        "average_ir_score": 0.0
    }
    
    total_ir_score_sum = 0.0
    tests_with_ir = 0

    def run_suite(inputs_pattern, outputs_dir, total_weight, suite_name):
        nonlocal total_ir_score_sum, tests_with_ir
        
        input_files = glob.glob(os.path.join(BASE_DIR, inputs_pattern, "*.txt"))
        if not input_files: return 0.0

        points_per_test = total_weight / len(input_files)
        suite_score = 0.0
        
        feedback_log.append(f"\n--- {suite_name} Tests ({total_weight}%) ---")
        
        for input_file in sorted(input_files):
            test_name = os.path.basename(input_file)
            expected_output_file = os.path.join(BASE_DIR, outputs_dir, test_name.replace(".txt", "_output.txt"))
            
            executable = os.path.join(CURRENT_STUDENT_DIR, "build", "compiler") # Makefile builds 'compiler'

            try:
                # Capture stdout (PA3 driver prints everything to stdout)
                proc = subprocess.run(
                    [executable, input_file], capture_output=True, text=True, timeout=5
                )
                
                actual = proc.stdout.strip().replace("\r\n", "\n")
                
                # --- Metrics Analysis ---
                file_metrics = parse_compiler_output(actual)
                
                # Update Aggregates
                rt_data = research_data["runtime"]
                rt_data["total_semantic_errors"] += file_metrics["semantic_error_count"]
                
                for et, count in file_metrics["semantic_error_types"].items():
                    rt_data["semantic_error_distribution"][et] = rt_data["semantic_error_distribution"].get(et, 0) + count
                    
                rt_data["total_ir_instructions"] += file_metrics["ir_instruction_count"]
                rt_data["total_labels_generated"] += file_metrics["labels_generated"]
                rt_data["total_decisions_loops"] += file_metrics["loops_decisions_count"]
                if file_metrics["temps_used"] > rt_data["max_temps_used"]:
                    rt_data["max_temps_used"] = file_metrics["temps_used"]
                # ------------------------

                if os.path.exists(expected_output_file):
                    with open(expected_output_file, 'r') as f_exp:
                        expected_full = f_exp.read().strip().replace("\r\n", "\n")
                    
                    # 1. Strict Check
                    # 1. Strict Check ONLY
                    if actual == expected_full:
                        suite_score += points_per_test
                        feedback_log.append(f"[PASS] {test_name}")
                        ir_score = 100.0
                    else:
                        feedback_log.append(f"[FAIL] {test_name} (Output Mismatch)")
                        ir_score = 0.0
                else:
                    feedback_log.append(f"[SKIP] {test_name} (Missing Expected Output)")

            except subprocess.TimeoutExpired:
                feedback_log.append(f"[FAIL] {test_name} (Timeout)")
            except Exception as e:
                feedback_log.append(f"[FAIL] {test_name} (Error: {e})")
        
        return suite_score

    s1 = run_suite(config["paths"]["visible_inputs"], config["paths"]["visible_outputs"], 
                   config["policy"]["visible_weight"], "Visible")
    s2 = run_suite(config["paths"]["hidden_inputs"], config["paths"]["hidden_outputs"], 
                   config["policy"]["hidden_weight"], "Hidden")
    
    if tests_with_ir > 0:
        research_data["runtime"]["average_ir_score"] = round(total_ir_score_sum / tests_with_ir, 2)

    return s1 + s2, "\n".join(feedback_log)

def main():
    for d in [RESULTS_DIR, FEEDBACK_DIR, METRICS_DIR]:
        if not os.path.exists(d): os.makedirs(d)
    
    config = load_config()
    if not config: return

    results_data = []
    submission_zips = glob.glob(os.path.join(SUBMISSIONS_DIR, "*.zip"))
    print(f"Found {len(submission_zips)} submissions.")

    for zip_path in sorted(submission_zips):
        filename = os.path.basename(zip_path)
        match = re.match(r"([a-zA-Z0-9]+)_PA3\.zip", filename)
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
            compile_success, compile_err, build_stats = compile_project()
            student_metrics["build"] = build_stats
            
            if not compile_success:
                feedback += f"\n[CRITICAL ERROR] Compilation Failed:\n{compile_err}\n"
                results_data.append([roll_number, 0, "Compilation Failed"])
            else:
                score, logs = run_tests(config, student_metrics)
                feedback += logs
                results_data.append([roll_number, round(score, 2), "Success"])

        student_metrics["score"] = round(score, 2)
        feedback += f"\n\nFINAL SCORE: {student_metrics['score']} / 100"

        with open(os.path.join(FEEDBACK_DIR, f"{roll_number}.txt"), "w") as f:
            f.write(feedback)
            
        metrics_file = os.path.join(METRICS_DIR, f"{roll_number}_metrics.json")
        with open(metrics_file, "w") as f:
            json.dump(student_metrics, f, indent=4)

    with open(os.path.join(RESULTS_DIR, "grades.csv"), "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Roll Number", "Score", "Status"])
        writer.writerows(results_data)

    print("Grading Complete.")
    clean_workspace()

if __name__ == "__main__":
    main()