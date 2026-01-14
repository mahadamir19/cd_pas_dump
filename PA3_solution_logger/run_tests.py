import os
import subprocess
import json
import glob
import sys
import platform
import time
import re
from difflib import SequenceMatcher

# --- Configuration ---
BUILD_DIR = "build"
EXEC_NAME = "compiler.exe" if platform.system() == "Windows" else "compiler"
COMPILER_EXE = os.path.join(BUILD_DIR, EXEC_NAME)

TEST_DIRECTORIES = [
    {
        "name": "Public Tests",
        "input": os.path.join("tests", "inputs"),
        "output": os.path.join("tests", "outputs")
    },
    {
        "name": "Hidden Tests", 
        "input": os.path.join("hidden_tests", "inputs"),
        "output": os.path.join("hidden_tests", "outputs")
    }
]

RESEARCH_LOG_FILE = "research_metrics.json"

def calculate_similarity(actual, expected):
    """Calculates a similarity score (0-100) between two strings."""
    return SequenceMatcher(None, actual, expected).ratio() * 100

def parse_compiler_output(output_text):
    """
    Parses stdout to extract:
    1. Semantic Error counts and types
    2. IR metrics (instruction count, temps, labels, branches)
    3. The raw IR section for comparison
    """
    metrics = {
        "semantic_error_count": 0,
        "semantic_error_types": {},
        "ir_instruction_count": 0,
        "temps_used": 0,
        "labels_generated": 0,
        "loops_decisions_count": 0, # IFZ + GOTO
        "ir_content": ""
    }

    lines = output_text.splitlines()
    in_ir_phase = False
    ir_lines = []
    
    # Regex patterns
    # Matches "Error at line X: [Message]"
    sem_error_pattern = re.compile(r"Error at line \d+: (.*)")
    # Matches t0, t1, t99
    temp_pattern = re.compile(r"\bt\d+\b")
    # Matches L0, L1, L99
    label_pattern = re.compile(r"\bL\d+\b")
    
    unique_temps = set()
    unique_labels = set()

    for line in lines:
        clean_line = line.strip()
        
        # --- 1. Analyze Semantic Errors ---
        err_match = sem_error_pattern.match(clean_line)
        if err_match:
            metrics["semantic_error_count"] += 1
            error_msg = err_match.group(1).strip()
            metrics["semantic_error_types"][error_msg] = metrics["semantic_error_types"].get(error_msg, 0) + 1
            continue

        # --- 2. Detect Phase Boundaries ---
        if "--- Phase 4: IR Generation ---" in clean_line:
            in_ir_phase = True
            continue

        # --- 3. Analyze IR Generation ---
        if in_ir_phase and clean_line:
            # Skip messages like "No IR generated"
            if "No IR generated" in clean_line:
                continue
                
            ir_lines.append(clean_line)
            metrics["ir_instruction_count"] += 1
            
            # Count Temps
            temps = temp_pattern.findall(clean_line)
            unique_temps.update(temps)
            
            # Count Labels
            labels = label_pattern.findall(clean_line)
            unique_labels.update(labels)
            
            # Count Loops/Decisions (IFZ and GOTO)
            if clean_line.startswith("IFZ") or clean_line.startswith("GOTO"):
                metrics["loops_decisions_count"] += 1

    metrics["temps_used"] = len(unique_temps)
    metrics["labels_generated"] = len(unique_labels)
    metrics["ir_content"] = "\n".join(ir_lines)
    
    return metrics

def run_tests():
    if not os.path.exists(COMPILER_EXE):
        print(f"Error: Executable {COMPILER_EXE} not found.")
        sys.exit(1)

    aggregate_data = {
        "passedTests_count": 0,
        "failedTestCount": 0,
        "total_execution_time_ms": 0.0,
        
        # PA3 Specific Metrics
        "total_semantic_errors": 0,
        "semantic_error_distribution": {}, # Type -> Count
        "total_ir_instructions": 0,
        "max_temps_used": 0,
        "total_labels_generated": 0,
        "total_decisions_loops": 0,
        "average_ir_score": 0.0
    }

    total_ir_score_sum = 0.0
    tests_with_ir = 0

    for test_group in TEST_DIRECTORIES:
        print(f"\n--- Running {test_group['name']} ---")
        
        # Look for input files
        if not os.path.exists(test_group["input"]):
             print(f"Skipping {test_group['name']} (Path not found)")
             continue

        test_files = glob.glob(os.path.join(test_group["input"], "*.txt"))
        
        if not test_files:
            print(f"No tests found in {test_group['input']}")
            continue

        for test_file in sorted(test_files):
            file_name = os.path.basename(test_file)
            expected_out_file = os.path.join(test_group["output"], file_name.replace(".txt", "_output.txt"))
            
            try:
                with open(test_file, "r") as f_in:
                    start_time = time.perf_counter()
                    # PA3 compiler outputs to stdout
                    proc = subprocess.run([COMPILER_EXE, test_file], capture_output=True, text=True)
                    end_time = time.perf_counter()
                
                duration_ms = (end_time - start_time) * 1000
                aggregate_data["total_execution_time_ms"] += duration_ms

                actual_output = proc.stdout.strip().replace("\r\n", "\n")
                
                # --- Metrics Analysis ---
                file_metrics = parse_compiler_output(actual_output)
                
                # Update Aggregate Semantic Metrics
                aggregate_data["total_semantic_errors"] += file_metrics["semantic_error_count"]
                for err_type, count in file_metrics["semantic_error_types"].items():
                    aggregate_data["semantic_error_distribution"][err_type] = \
                        aggregate_data["semantic_error_distribution"].get(err_type, 0) + count

                # Update Aggregate IR Metrics
                aggregate_data["total_ir_instructions"] += file_metrics["ir_instruction_count"]
                aggregate_data["total_labels_generated"] += file_metrics["labels_generated"]
                aggregate_data["total_decisions_loops"] += file_metrics["loops_decisions_count"]
                if file_metrics["temps_used"] > aggregate_data["max_temps_used"]:
                    aggregate_data["max_temps_used"] = file_metrics["temps_used"]

                # --- Comparison & Scoring ---
                passed = False
                ir_score = 0.0
                
                if os.path.exists(expected_out_file):
                    with open(expected_out_file, "r") as f_exp:
                        expected_full = f_exp.read().strip().replace("\r\n", "\n")
                    
                    # 1. Strict Pass/Fail
                    if actual_output == expected_full:
                        passed = True
                        ir_score = 100.0
                    else:
                        # 2. Calculate IR Score (Partial Credit)
                        # Extract IR part from Expected Output for fair comparison
                        expected_metrics = parse_compiler_output(expected_full)
                        ir_score = calculate_similarity(file_metrics["ir_content"], expected_metrics["ir_content"])

                if file_metrics["ir_instruction_count"] > 0 or passed:
                    total_ir_score_sum += ir_score
                    tests_with_ir += 1

                if passed:
                    print(f"PASS: {file_name} (IR Score: 100%)")
                    aggregate_data["passedTests_count"] += 1
                else:
                    print(f"FAIL: {file_name} (IR Score: {ir_score:.1f}%)")
                    aggregate_data["failedTestCount"] += 1

            except Exception as e:
                print(f"ERROR executing {file_name}: {e}")
                aggregate_data["failedTestCount"] += 1

    # Finalize Averages
    if tests_with_ir > 0:
        aggregate_data["average_ir_score"] = round(total_ir_score_sum / tests_with_ir, 2)

    # Save Report
    with open(RESEARCH_LOG_FILE, "w") as f:
        json.dump(aggregate_data, f, indent=4)
    
    print("-" * 30)
    print(f"Metrics saved to: {RESEARCH_LOG_FILE}")
    print(f"Tests Passed: {aggregate_data['passedTests_count']}")
    print(f"Avg IR Score: {aggregate_data['average_ir_score']}%")

if __name__ == "__main__":
    run_tests()