import os
import shutil
import input_generate
import ocean

script_dir = os.path.dirname(os.path.abspath(__file__))
output_path = os.path.join(script_dir, "files", "outputs")
input_path = os.path.join(script_dir, "files", "inputs")

os.makedirs(output_path, exist_ok=True)
os.makedirs(input_path, exist_ok=True)


test_count = 30
line_count = 5000
digits = len(str(test_count))
for i in range(1, test_count + 1):
    input_file = os.path.join(input_path, f"input{i:0{digits}d}.txt")
    output_file = os.path.join(output_path, f"output{i:0{digits}d}.txt")
    if i!=1:
        with open(input_file, "w") as f:
            f.write(input_generate.generate(line_count))
    else:
        source_file = "../test/randtest0.in"  # The file you want to copy
        with open(source_file, "r") as src, open(input_file, "w") as dst:
            shutil.copyfileobj(src, dst)
    ocean.main(input_file, output_file)
