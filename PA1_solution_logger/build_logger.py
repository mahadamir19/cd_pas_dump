import time
import subprocess
import re
import json
import os

LOG_FILE = "compile_log.json"
LEXER_PATH = os.path.join("src", "lexer.l")

def analyze_lexer_file():
    if not os.path.exists(LEXER_PATH):
        return {}

    with open(LEXER_PATH, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    # Extract Rules Section
    parts = content.split('%%')
    rules_section = parts[1] if len(parts) >= 2 else ""

    return {
        "flex_rule_count": len(re.findall(r'\n[^\s].*', rules_section)),
        "keyword_count": len(re.findall(r'"(if|else|while|for|return|int|float)"', rules_section)),
        "handled_comments": 1 if ("//" in rules_section) else 0,
        "whitespaces_handled": 1 if ("[ \\t\\r\\n]+" in rules_section) else 0
    }

def main():
    print("--- Starting Build ---")
    
    # Clean and Make
    subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    start_time = time.time()
    result = subprocess.run(['make'], capture_output=True, text=True)
    end_time = time.time()
    
    metrics = {
        "compilation_status": "SUCCESS" if result.returncode == 0 else "FAILURE",
        "compile_time": round(end_time - start_time, 4),
        "warning_count": result.stderr.count("warning:"),
        "error_count": result.stderr.count("error:")
    }
    
    metrics.update(analyze_lexer_file())
    
    with open(LOG_FILE, "w") as f:
        json.dump(metrics, f, indent=4)
        
    print(f"Build Status: {metrics['compilation_status']}")
    if metrics['compilation_status'] == "FAILURE":
        print(result.stderr)

if __name__ == "__main__":
    main()