import os
import argparse

# Check args
parser = argparse.ArgumentParser(description='Upload elf binary on board')
parser.add_argument('-f', required=True, help="Path to the elf file")
args = parser.parse_args()
elf_path = args.f.replace("\\","/")

# Change to script directory
script_path = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_path)

# Flash target
cmd = "openocd -f openocd.cfg -c \"program " + elf_path +" verify reset exit\""
os.system(cmd)
