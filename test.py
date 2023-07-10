import os
import subprocess
import random

def run_script(script_path, input_file):
    try:
        result = subprocess.run([script_path, input_file], capture_output=True, text=True, timeout=1)
        return result.stdout.strip()
    except subprocess.TimeoutExpired:
        return "Timeout Error"
    except subprocess.CalledProcessError as e:
        return f"Error: {e.returncode}"

def compare_scripts(script1_path, script2_path, input_dir):
    list_dir = os.listdir(input_dir)
    # random.shuffle(list_dir)
    for filename in list_dir:
        input_file = os.path.join(input_dir, filename)
        output1 = run_script(script1_path, input_file)
        output2 = run_script(script2_path, input_file)
        equal = (output1 == output2)
        if not equal:
            print(filename)
            print(output1)
            print("COMPARED TO")
            print(output2)
            exit(1)

script1_path = os.path.join(os.getcwd(), "build", "lrsnash")
script2_path = os.path.join(os.getcwd(), "build", "lrsnashgmp")
input_dir = os.path.join(os.getcwd(), "new_games")

compare_scripts(script1_path, script2_path, input_dir)