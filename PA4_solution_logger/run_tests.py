import subprocess
import os
import glob
import json
import time
import re
import platform

# --- Configuration ---
COMPILER_BIN = "./build/compiler"
if platform.system() == "Windows":
    COMPILER_BIN += ".exe"

METRICS_FILE = "pa4_metrics.json"

# Define test suites
TEST_SUITES = [
    {
        "name": "Visible Tests",
        "input_dir": "tests/inputs",
        "expected_ir_dir": "tests/expected_ir",
        "expected_spim_dir": "tests/expected_output",
        "mips_output_dir": "tests/mips"
    },
    {
        "name": "Hidden Tests",
        "input_dir": "hidden_tests/inputs",
        "expected_ir_dir": "hidden_tests/expected_ir",
        "expected_spim_dir": "hidden_tests/expected_output",
        "mips_output_dir": "hidden_tests/mips"
    }
]

def run_command(command):
    result = subprocess.run(command, capture_output=True, text=True)
    return result

def clean_spim_output(raw_output):
    """
    Strips the first 5 lines of SPIM output (header) to isolate program output.
    """
    lines = raw_output.splitlines()
    if len(lines) < 5:
        return raw_output.strip()
    return "\n".join(lines[5:]).strip()

def parse_mips_stats(mips_filename):
    """
    Analyzes the generated MIPS assembly file for research metrics.
    """
    stats = {
        "mips_loc": 0,           # Lines of code in .text
        "stack_ops": 0,          # $sp usage
        "branches": 0,           # Control flow complexity
        "arithmetic_ops": 0      # Computational density
    }

    if not os.path.exists(mips_filename):
        return stats

    with open(mips_filename, "r") as f:
        lines = f.readlines()

    in_text = False
    re_stack = re.compile(r"\$sp")
    re_branch = re.compile(r"\b(j|jal|jr|beq|bne|beqz|bnez|bgt|blt|bge|ble)\b")
    re_math = re.compile(r"\b(add|sub|mul|div|rem|seq|sne|slt|sgt|sle|sge)\b")
    re_label = re.compile(r"^\s*\w+:")

    for line in lines:
        raw = line.strip()
        if not raw or raw.startswith("#"): continue

        if ".text" in raw:
            in_text = True
            continue
        if ".data" in raw:
            in_text = False
            continue

        if in_text:
            # Skip labels (e.g. "main:")
            if re_label.match(raw):
                continue
            
            stats["mips_loc"] += 1
            if re_stack.search(raw):
                stats["stack_ops"] += 1
            if re_branch.search(raw):
                stats["branches"] += 1
            if re_math.search(raw):
                stats["arithmetic_ops"] += 1

    return stats

def run_tests():
    print("--- Building Compiler ---")
    make_proc = subprocess.run(["make"], capture_output=True, text=True)
    if make_proc.returncode != 0:
        print("❌ Make failed!")
        print(make_proc.stderr)
        return

    # Data container for JSON export
    research_data = {
        "summary": {
            "total_tests": 0,
            "passed_tests": 0,
            "failed_tests": 0,
            "avg_compilation_ms": 0.0,
            "avg_mips_loc": 0.0
        },
        "tests": []
    }

    total_compile_time = 0.0
    total_loc = 0
    total_tests_count = 0
    total_passed_count = 0

    # Iterate over both Visible and Hidden test suites
    for suite in TEST_SUITES:
        print(f"\n🔹 Running {suite['name']}...")
        
        # Create output directory for this suite
        os.makedirs(suite["mips_output_dir"], exist_ok=True)

        if not os.path.exists(suite["input_dir"]):
            print(f"   ⚠️ Directory not found: {suite['input_dir']}")
            continue

        input_files = glob.glob(os.path.join(suite["input_dir"], "*.txt"))
        input_files.sort()

        if not input_files:
            print(f"   ⚠️ No .txt files found in {suite['input_dir']}")
            continue

        for input_path in input_files:
            total_tests_count += 1
            ir_matched, spim_matched = False, False
            
            test_name = os.path.basename(input_path).replace(".txt", "")
            mips_path = os.path.join(suite["mips_output_dir"], f"{test_name}.s")
            
            print(f"   🚀 {test_name}...", end=" ", flush=True)

            # --- 1. Compile & Profile ---
            start_time = time.perf_counter()
            compiler_proc = run_command([COMPILER_BIN, input_path, mips_path])
            duration_ms = (time.perf_counter() - start_time) * 1000
            
            total_compile_time += duration_ms
            actual_ir = compiler_proc.stdout.strip()
            
            # Heuristic IR count
            ir_instruction_count = len([l for l in actual_ir.splitlines() if l.strip()])

            if compiler_proc.returncode != 0:
                print("❌ Compile Error")
                status = "COMPILE_ERROR"
            else:
                # --- 2. Verify IR (Optional) ---
                # Note: Assuming hidden tests also have expected_ir files. If not, this check is skipped.
                expected_ir_path = os.path.join(suite["expected_ir_dir"], f"{test_name}_output.txt")
                if os.path.exists(expected_ir_path):
                    with open(expected_ir_path, 'r') as f:
                        expected_ir = f.read().strip()
                    if actual_ir == expected_ir:
                        ir_matched = True
                else:
                    ir_matched = True # Assume true if no expected file to compare against

                # --- 3. Run SPIM ---
                spim_proc = run_command(["spim", "-file", mips_path])
                actual_runtime_output = clean_spim_output(spim_proc.stdout)
                
                # --- 4. Verify Runtime Output ---
                # Check for _spim.txt first (PA3 style), then _out.txt (Standard style)
                expected_spim_path = os.path.join(suite["expected_spim_dir"], f"{test_name}_spim.txt")
                if not os.path.exists(expected_spim_path):
                     expected_spim_path = os.path.join(suite["expected_spim_dir"], f"{test_name}_out.txt")

                if os.path.exists(expected_spim_path):
                    with open(expected_spim_path, 'r') as f:
                        expected_runtime_output = clean_spim_output(f.read())
                    
                    if actual_runtime_output == expected_runtime_output:
                        spim_matched = True
                        print("✅ PASS")
                    else:
                        print("❌ FAIL (Output Mismatch)")
                        # Uncomment for detailed debug
                        # print(f"     Expected: {expected_runtime_output}")
                        # print(f"     Got:      {actual_runtime_output}")
                else:
                    print("⚠️  SKIP (No Expected Output)")
                
                # --- 5. Gather Metrics ---
                mips_stats = parse_mips_stats(mips_path)
                total_loc += mips_stats["mips_loc"]
                
                status = "FAIL"
                if spim_matched:
                    status = "PASS"
                    total_passed_count += 1
                    if not ir_matched and os.path.exists(expected_ir_path):
                        status = "PASS_IR_MISMATCH"

                # Log entry
                research_data["tests"].append({
                    "test_name": test_name,
                    "status": status,
                    "compilation_time_ms": round(duration_ms, 2),
                    "final_ir_count": ir_instruction_count,
                    "mips_loc": mips_stats["mips_loc"],
                    "stack_ops": mips_stats["stack_ops"],
                    "branches": mips_stats["branches"]
                })

    # --- Finalize Stats ---
    research_data["summary"]["total_tests"] = total_tests_count
    if total_tests_count > 0:
        research_data["summary"]["passed_tests"] = total_passed_count
        research_data["summary"]["failed_tests"] = total_tests_count - total_passed_count
        research_data["summary"]["avg_compilation_ms"] = round(total_compile_time / total_tests_count, 2)
        research_data["summary"]["avg_mips_loc"] = round(total_loc / total_tests_count, 2)

    # Save JSON
    with open(METRICS_FILE, "w") as f:
        json.dump(research_data, f, indent=4)
    
    print("-" * 40)
    print(f"Final Results: Passed {total_passed_count}/{total_tests_count}")
    print(f"Metrics saved to {METRICS_FILE}")

if __name__ == "__main__":
    run_tests()