import os
import glob
import subprocess
import shutil
import csv
import json
import re
import time
import zipfile
import platform

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

# PA4 Reference Files: Harness + Frontend + Middle-end
# Student provides ONLY codegen.cpp/h and optimizer.cpp/h
REQUIRED_REF_FILES = [
    os.path.join(CONFIG_DIR, "reference", "Makefile"),
    os.path.join(CONFIG_DIR, "reference", "driver.cpp"),
    os.path.join(CONFIG_DIR, "reference", "lexer.l"),
    os.path.join(CONFIG_DIR, "reference", "parser.y"),
    os.path.join(CONFIG_DIR, "reference", "ast.cpp"),
    os.path.join(CONFIG_DIR, "reference", "ast.h"),
    os.path.join(CONFIG_DIR, "reference", "ir.cpp"),
    os.path.join(CONFIG_DIR, "reference", "ir.h"),
    os.path.join(CONFIG_DIR, "reference", "ir_generator.cpp"),
    os.path.join(CONFIG_DIR, "reference", "ir_generator.h"),
    os.path.join(CONFIG_DIR, "reference", "semantics.cpp"),
    os.path.join(CONFIG_DIR, "reference", "semantics.h"),
    os.path.join(CONFIG_DIR, "reference", "symbol_table.cpp"),
    os.path.join(CONFIG_DIR, "reference", "symbol_table.h"),
    os.path.join(CONFIG_DIR, "reference", "errors.cpp"),
    os.path.join(CONFIG_DIR, "reference", "errors.h")
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
    Unzips student code, flattens it, and injects reference harness.
    """
    clean_workspace()
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(CURRENT_STUDENT_DIR)
    except Exception as e:
        return False, f"Failed to unzip: {str(e)}"

    # Flatten directory if nested (anchor on codegen.cpp)
    anchor_files = glob.glob(os.path.join(CURRENT_STUDENT_DIR, "**", "codegen.cpp"), recursive=True)
    
    if not anchor_files:
        return False, "codegen.cpp not found in submission."
    
    source_root = os.path.dirname(anchor_files[0])
    if source_root != CURRENT_STUDENT_DIR:
        for item in os.listdir(source_root):
            try:
                shutil.move(os.path.join(source_root, item), CURRENT_STUDENT_DIR)
            except:
                pass 

    # Inject Reference Files
    for file_path in REQUIRED_REF_FILES:
        if os.path.exists(file_path):
            shutil.copy(file_path, CURRENT_STUDENT_DIR)
        else:
            print(f"Warning: Reference file missing: {file_path}")

    return True, "Setup OK"

def compile_project():
    build_stats = {
        "status": "FAILURE",
        "compile_time": 0.0,
        "error_count": 0
    }

    start_time = time.time()
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

# --- Testing Helpers ---

def clean_spim_output(raw_output):
    """Matches logic from student's run_tests.py: skips first 5 lines."""
    lines = raw_output.splitlines()
    if len(lines) < 5: return raw_output.strip()
    return "\n".join(lines[5:]).strip()

def run_tests(config, research_data):
    feedback_log = []
    
    research_data["runtime"] = {
        "total_tests": 0,
        "passed_tests": 0,
        "ir_matches": 0,
        "runtime_matches": 0
    }
    
    def run_suite(inputs_rel_path, ir_rel_path, spim_rel_path, total_weight, suite_name):
        # Resolve full paths relative to BASE_DIR
        # Example: inputs_rel_path = "config/tests/inputs"
        inputs_full_path = os.path.join(BASE_DIR, inputs_rel_path)
        ir_full_path = os.path.join(BASE_DIR, ir_rel_path)
        spim_full_path = os.path.join(BASE_DIR, spim_rel_path)

        input_files = glob.glob(os.path.join(inputs_full_path, "*.txt"))
        if not input_files: return 0.0

        points_per_test = total_weight / len(input_files)
        suite_score = 0.0
        
        feedback_log.append(f"\n--- {suite_name} Tests ({total_weight}%) ---")
        
        executable = os.path.join(CURRENT_STUDENT_DIR, "build", "compiler")
        if platform.system() == "Windows": executable += ".exe"

        for input_file in sorted(input_files):
            test_name = os.path.basename(input_file).replace(".txt", "")
            research_data["runtime"]["total_tests"] += 1
            
            # 1. Paths for this test case
            mips_file = os.path.join(CURRENT_STUDENT_DIR, f"{test_name}.s")
            exp_ir_file = os.path.join(ir_full_path, f"{test_name}_output.txt")
            exp_spim_file = os.path.join(spim_full_path, f"{test_name}_spim.txt")

            try:
                # 2. Run Compiler (Generates IR to stdout and MIPS to file)
                proc_compile = subprocess.run(
                    [executable, input_file, mips_file], 
                    capture_output=True, text=True, timeout=5
                )

                if proc_compile.returncode != 0:
                     feedback_log.append(f"[FAIL] {test_name} (Compiler Crash)")
                     continue
                
                actual_ir = proc_compile.stdout.strip()

                # 3. Check IR (Optimization)
                ir_pass = False
                if os.path.exists(exp_ir_file):
                    with open(exp_ir_file, 'r') as f:
                        expected_ir = f.read().strip()
                    if actual_ir == expected_ir:
                        ir_pass = True
                        research_data["runtime"]["ir_matches"] += 1
                    else:
                        # Log partial failure but don't fail immediately, wait for runtime check
                        pass 
                else:
                    # If expected IR is missing, we assume this test doesn't check IR (e.g. only runtime)
                    ir_pass = True

                # 4. Check Runtime (SPIM)
                spim_pass = False
                
                # Run SPIM
                proc_spim = subprocess.run(
                    ["spim", "-file", mips_file], 
                    capture_output=True, text=True, timeout=3
                )
                actual_spim = clean_spim_output(proc_spim.stdout)

                if os.path.exists(exp_spim_file):
                    with open(exp_spim_file, 'r') as f:
                        expected_spim = clean_spim_output(f.read())
                    
                    if actual_spim == expected_spim:
                        spim_pass = True
                        research_data["runtime"]["runtime_matches"] += 1
                else:
                    # If expected output missing, we can't grade
                    feedback_log.append(f"[SKIP] {test_name} (Missing Expected Output)")
                    continue

                # 5. Final Grade for Test Case
                if ir_pass and spim_pass:
                    suite_score += points_per_test
                    feedback_log.append(f"[PASS] {test_name}")
                    research_data["runtime"]["passed_tests"] += 1
                else:
                    reasons = []
                    if not ir_pass: reasons.append("IR Mismatch")
                    if not spim_pass: reasons.append("Runtime Mismatch")
                    feedback_log.append(f"[FAIL] {test_name} ({', '.join(reasons)})")

            except subprocess.TimeoutExpired:
                feedback_log.append(f"[FAIL] {test_name} (Timeout)")
            except Exception as e:
                feedback_log.append(f"[FAIL] {test_name} (Error: {e})")
        
        return suite_score

    # Note: Adjust these paths if your weightage.json uses different keys
    # Assuming standard structure based on screenshot:
    # config/tests/inputs
    # config/tests/expected_ir
    # config/tests/expected_output
    
    s1 = run_suite(
        "config/tests/inputs", 
        "config/tests/expected_ir", 
        "config/tests/expected_output", 
        config["policy"]["visible_weight"], "Visible"
    )
    
    s2 = run_suite(
        "config/hidden_tests/inputs", 
        "config/hidden_tests/expected_ir", 
        "config/hidden_tests/expected_output", 
        config["policy"]["hidden_weight"], "Hidden"
    )
    
    return s1 + s2, "\n".join(feedback_log)

def main():
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
        match = re.match(r"([a-zA-Z0-9]+)_PA4\.zip", filename)
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