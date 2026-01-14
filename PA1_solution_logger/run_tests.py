import os
import subprocess
import json
import glob
import sys
import time
import platform
import re

# --- Configuration ---
BUILD_DIR = "build"
EXE_EXT = ".exe" if platform.system() == "Windows" else ""
LEXER_EXE_NAME = f"lexer{EXE_EXT}"
LEXER_EXE = os.path.join(BUILD_DIR, LEXER_EXE_NAME)

# Define directories to check
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

def parse_lexer_output(output_text):
    """
    Parses the stdout from driver.c to extract metrics without modifying the C code.
    Expected format: 
    Line    Token Type    Lexeme
    --------------------------------
    ... rows ...
    """
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
        
        # Stop if we hit the Symbol Table dump (heuristically detected by header or empty lines)
        # Adjust this condition if your symbol table header is different
        if "Symbol Table" in clean_line or clean_line == "":
            continue

        if parsing_tokens:
            # Attempt to split by whitespace (Line, Type, Lexeme)
            # We use maxsplit=2 because Lexeme might contain spaces? 
            # Actually driver.c prints simple tokens, so split() is safe usually.
            parts = clean_line.split(None, 2)
            if len(parts) < 2: 
                continue # Skip malformed lines

            token_type = parts[1]
            lexeme = parts[2] if len(parts) > 2 else ""

            metrics["token_count"] += 1
            metrics["unique_lexemes"].add(lexeme)

            # Categorize based on the strings in driver.c::get_token_name
            if token_type == "IDENTIFIER":
                metrics["identifiers"] += 1
            elif token_type.startswith("KEYWORD_") or token_type.startswith("TYPE_"):
                metrics["keywords"] += 1
            elif token_type.startswith("LITERAL_"):
                metrics["literals"] += 1
            elif token_type == "ERROR" or token_type == "UNKNOWN_TOKEN":
                metrics["errors"] += 1
    
    # Heuristic for recovery: If we had an error but total tokens > error count + 1,
    # it implies the lexer continued processing.
    if metrics["errors"] > 0 and metrics["token_count"] > metrics["errors"]:
        metrics["recovered"] = True

    return metrics

def run_tests():
    # 1. Verify Executable
    if not os.path.exists(LEXER_EXE):
        print(f"Error: Executable {LEXER_EXE} not found.")
        print("Please run compilation first.")
        sys.exit(1)

    # 2. Initialize Aggregate Metrics
    aggregate_data = {
        "run_time_metrics": {
            "total_tokens": 0,
            "unique_tokens_count": 0, # Size of global set
            "identifier_count": 0,
            "keyword_count": 0,
            "literal_count": 0,
            "invalid_token_count": 0,
            "total_input_lines": 0
        },
        "test_suite_metrics": {
            "passedTests_count": 0,
            "failedTestCount": 0,
            "edge_cases": {
                "whitespace_handled": False,
                "comments_handled": False,
                "invalid_tokens_handled": False
            }
        },
        "error_handling_metrics": {
            "error_handling_condition_triggered": False,
            "error_msg_count": 0,
            "error_recovery_count": 0
        }
    }

    global_unique_lexemes = set()

    print(f"--- Starting Extended Metrics Runner for {LEXER_EXE_NAME} ---")

    # 3. Iterate through Test Directories
    for directory in TEST_DIRECTORIES:
        print(f"\n--- Checking {directory['name']} ---")
        
        input_pattern = os.path.join(directory["input"], "*.txt")
        test_files = sorted(glob.glob(input_pattern))
        
        if not test_files:
            print(f"No tests found in {directory['input']}")
            continue

        for test_file in test_files:
            file_name = os.path.basename(test_file)
            base_name = os.path.splitext(file_name)[0]
            expected_out_file = os.path.join(directory["output"], f"{base_name}_output.txt")

            # --- Pre-Execution Analysis (Input Size & Edge Case Detection) ---
            line_count = 0
            has_comments = False
            has_whitespace_complexity = False
            
            try:
                with open(test_file, "r", encoding="utf-8") as f:
                    content = f.read()
                    line_count = len(content.splitlines())
                    if "//" in content or "/*" in content:
                        has_comments = True
                    if line_count > 1: # Basic heuristic for complexity
                        has_whitespace_complexity = True
            except:
                pass

            aggregate_data["run_time_metrics"]["total_input_lines"] += line_count

            # --- Execution ---
            try:
                proc = subprocess.run([LEXER_EXE, test_file], capture_output=True, text=True)
                actual_output = proc.stdout.strip().replace("\r\n", "\n")

                # --- Output Parsing (Without modifying driver.c) ---
                # We extract metrics from the text output of the driver
                run_metrics = parse_lexer_output(actual_output)

                # Update Aggregates
                aggregate_data["run_time_metrics"]["total_tokens"] += run_metrics["token_count"]
                aggregate_data["run_time_metrics"]["identifier_count"] += run_metrics["identifiers"]
                aggregate_data["run_time_metrics"]["keyword_count"] += run_metrics["keywords"]
                aggregate_data["run_time_metrics"]["literal_count"] += run_metrics["literals"]
                aggregate_data["run_time_metrics"]["invalid_token_count"] += run_metrics["errors"]
                
                global_unique_lexemes.update(run_metrics["unique_lexemes"])

                # Error Handling Metrics
                if run_metrics["errors"] > 0:
                    aggregate_data["error_handling_metrics"]["error_handling_condition_triggered"] = True
                    aggregate_data["error_handling_metrics"]["error_msg_count"] += run_metrics["errors"]
                    
                if run_metrics["recovered"]:
                    aggregate_data["error_handling_metrics"]["error_recovery_count"] += 1

                # --- Correctness Check ---
                passed = False
                if os.path.exists(expected_out_file):
                    with open(expected_out_file, "r", encoding="utf-8") as f_out:
                        expected_output = f_out.read().strip().replace("\r\n", "\n")
                    
                    if actual_output == expected_output:
                        passed = True
                    else:
                        print(f"FAIL: {file_name} (Output mismatch)")
                else:
                    # Fallback if no golden file
                    if proc.returncode == 0:
                        passed = True
                        print(f"WARN: {file_name} passed (exit 0), but no golden file.")
                    else:
                        print(f"FAIL: {file_name} (Crashed)")

                if passed:
                    print(f"PASS: {file_name}")
                    aggregate_data["test_suite_metrics"]["passedTests_count"] += 1
                    
                    # Update Edge Case flags only on success (proving they were handled)
                    if has_comments:
                        aggregate_data["test_suite_metrics"]["edge_cases"]["comments_handled"] = True
                    if has_whitespace_complexity:
                        aggregate_data["test_suite_metrics"]["edge_cases"]["whitespace_handled"] = True
                    if run_metrics["errors"] > 0:
                         # If we had errors but matched the golden output, we handled invalid tokens correctly
                        aggregate_data["test_suite_metrics"]["edge_cases"]["invalid_tokens_handled"] = True
                else:
                    aggregate_data["test_suite_metrics"]["failedTestCount"] += 1

            except Exception as e:
                print(f"ERROR executing {file_name}: {e}")
                aggregate_data["test_suite_metrics"]["failedTestCount"] += 1

    # Final Calculation for Unique Tokens
    aggregate_data["run_time_metrics"]["unique_tokens_count"] = len(global_unique_lexemes)

    # 4. Save Report
    with open(RESEARCH_LOG_FILE, "w") as f:
        json.dump(aggregate_data, f, indent=4)
    
    print("-" * 30)
    print(f"Metrics saved to: {RESEARCH_LOG_FILE}")
    print(f"Passed: {aggregate_data['test_suite_metrics']['passedTests_count']}")
    print(f"Tokens Processed: {aggregate_data['run_time_metrics']['total_tokens']}")
    print(f"Errors Encountered: {aggregate_data['error_handling_metrics']['error_msg_count']}")

if __name__ == "__main__":
    run_tests()