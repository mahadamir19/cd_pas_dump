import time
import subprocess
import re
import json
import os

LOG_FILE = "compile_log.json"
PARSER_PATH = os.path.join("src", "parser.y")

def analyze_parser_file():
    if not os.path.exists(PARSER_PATH):
        return {}

    with open(PARSER_PATH, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    # Extract Grammar Rules Section (between first %% and second %%)
    parts = content.split('%%')
    rules_section = parts[1] if len(parts) >= 2 else ""

    return {
        # Count lines that look like rules (start with a letter or |)
        "grammar_rule_count": len(re.findall(r'^\s*(\w+|\|)', rules_section, re.MULTILINE)),
        # Check if they used precedence declarations
        "precedence_rules_used": 1 if "%left" in content or "%right" in content else 0,
    }

def main():
    print("--- Starting Build ---")
    
    # Clean and Make
    subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    start_time = time.time()
    # Capture stderr because Bison prints conflicts to stderr
    result = subprocess.run(['make'], capture_output=True, text=True)
    end_time = time.time()
    
    # Parse Bison Output for Conflicts
    shift_reduce = 0
    reduce_reduce = 0
    
    # Regex to find "warning: X shift/reduce conflicts"
    sr_match = re.search(r'(\d+) shift/reduce conflict', result.stderr)
    if sr_match:
        shift_reduce = int(sr_match.group(1))

    rr_match = re.search(r'(\d+) reduce/reduce conflict', result.stderr)
    if rr_match:
        reduce_reduce = int(rr_match.group(1))

    metrics = {
        "compilation_status": "SUCCESS" if result.returncode == 0 else "FAILURE",
        "compile_time": round(end_time - start_time, 4),
        "shift_reduce_conflicts": shift_reduce,
        "reduce_reduce_conflicts": reduce_reduce,
        "error_count": result.stderr.count("error:")
    }
    
    metrics.update(analyze_parser_file())
    
    with open(LOG_FILE, "w") as f:
        json.dump(metrics, f, indent=4)
        
    print(f"Build Status: {metrics['compilation_status']}")
    if shift_reduce > 0:
        print(f"Warnings: {shift_reduce} shift/reduce conflicts detected.")
    
    if metrics['compilation_status'] == "FAILURE":
        print(result.stderr)

if __name__ == "__main__":
    main()