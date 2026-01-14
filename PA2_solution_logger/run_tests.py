import os
import subprocess
import json
import glob
import sys
import platform
import time

# --- Configuration ---
BUILD_DIR = "build"
# Auto-detect extension
EXEC_NAME = "syntax.exe" if platform.system() == "Windows" else "syntax"
PARSER_EXE = os.path.join(BUILD_DIR, EXEC_NAME)

# Define nesting threshold (depth > 5 implies nested constructs like loops/ifs)
NESTING_THRESHOLD = 5

# Define all directories to check
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

def analyze_ast_structure(output_text):
    """Calculates tree metrics based on indentation of print_ast output."""
    lines = output_text.splitlines()
    node_count = 0
    max_depth = 0
    
    for line in lines:
        stripped = line.lstrip()
        if not stripped: continue
        
        # Ignore "--- AST Structure ---" headers if present
        if stripped.startswith("---") or stripped.startswith("Parse"):
            continue
            
        node_count += 1
        
        # Calculate depth: indentation spaces / 2
        indentation = len(line) - len(stripped)
        depth = indentation // 2
        if depth > max_depth:
            max_depth = depth
            
    return node_count, max_depth

def run_tests():
    if not os.path.exists(PARSER_EXE):
        print(f"Error: Executable {PARSER_EXE} not found.")
        sys.exit(1)

    aggregate_data = {
        "passedTests_count": 0,
        "failedTestCount": 0,
        "total_ast_nodes": 0,
        "max_tree_depth": 0,
        "total_execution_time_ms": 0.0,
        "panic_recoveries_triggered": 0,
        "edge_cases": {
            "syntax_errors_handled": False,
            "nested_constructs_handled": False # Added this metric
        }
    }

    # Loop through both Public and Hidden directories
    for test_group in TEST_DIRECTORIES:
        print(f"\n--- Running {test_group['name']} ---")
        
        # Find all .txt files in the current input directory
        test_files = glob.glob(os.path.join(test_group["input"], "*.txt"))
        
        if not test_files:
            print(f"No tests found in {test_group['input']}")
            continue

        for test_file in sorted(test_files):
            file_name = os.path.basename(test_file)
            expected_out_file = os.path.join(test_group["output"], file_name.replace(".txt", "_output.txt"))
            
            # Run Parser
            try:
                with open(test_file, "r") as f_in:
                    # Capture stderr to detect Panic Mode
                    # Measure Runtime
                    start_time = time.perf_counter()
                    proc = subprocess.run([PARSER_EXE], stdin=f_in, capture_output=True, text=True)
                    end_time = time.perf_counter()
                
                # Calculate duration in ms
                duration_ms = (end_time - start_time) * 1000
                aggregate_data["total_execution_time_ms"] += duration_ms

                actual_output = proc.stdout.strip().replace("\r\n", "\n")
                
                # --- Check Pass/Fail ---
                passed = False
                if os.path.exists(expected_out_file):
                    with open(expected_out_file, "r") as f_exp:
                        expected = f_exp.read().strip().replace("\r\n", "\n")
                    if actual_output == expected:
                        passed = True
                
                if passed:
                    print(f"PASS: {file_name} ({duration_ms:.2f}ms)")
                    aggregate_data["passedTests_count"] += 1
                else:
                    print(f"FAIL: {file_name} (Expected match failed)")
                    aggregate_data["failedTestCount"] += 1

                # --- Harvest Metrics ---
                nodes, depth = analyze_ast_structure(actual_output)
                aggregate_data["total_ast_nodes"] += nodes
                if depth > aggregate_data["max_tree_depth"]:
                    aggregate_data["max_tree_depth"] = depth

                # Count Panic Mode triggers (look at stderr)
                panic_count = proc.stderr.count("Panic Mode")
                aggregate_data["panic_recoveries_triggered"] += panic_count
                
                if panic_count > 0:
                    aggregate_data["edge_cases"]["syntax_errors_handled"] = True

                # --- Check Nested Construct Handling ---
                # If we passed a test with significant depth, we have handled nesting.
                if passed and depth >= NESTING_THRESHOLD:
                    aggregate_data["edge_cases"]["nested_constructs_handled"] = True

            except Exception as e:
                print(f"ERROR executing {file_name}: {e}")
                aggregate_data["failedTestCount"] += 1

    # Save Report
    with open(RESEARCH_LOG_FILE, "w") as f:
        json.dump(aggregate_data, f, indent=4)
    
    print("-" * 30)
    print(f"Metrics saved to: {RESEARCH_LOG_FILE}")
    print(f"Total Tests Passed: {aggregate_data['passedTests_count']}")

if __name__ == "__main__":
    run_tests()