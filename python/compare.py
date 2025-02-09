#!/usr/bin/env python3
import os

# Emojis para indicar resultado
CHECK = "\u2705"  # ✅
ERROR = "\u274C"  # ❌

output_dir = "outputs"
expected_dir = "expected_outputs"

# Para cada teste de 1 a 10
for i in range(1, 11):
    output_file = os.path.join(output_dir, f"output_{i}.txt")
    expected_file = os.path.join(expected_dir, f"output_{i}.txt")
    
    try:
        with open(output_file, 'r') as f:
            output_content = f.read()
    except FileNotFoundError:
        output_content = ""
    
    try:
        with open(expected_file, 'r') as f:
            expected_content = f.read()
    except FileNotFoundError:
        expected_content = ""
    
    if output_content == expected_content:
        print(f"output_{i}.txt {CHECK}")
    else:
        print(f"output_{i}.txt {ERROR}")