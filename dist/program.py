#!/usr/bin/env python
import os
import argparse

# Check args
parser = argparse.ArgumentParser(description='Program elf binary to board')
parser.add_argument("elf_file", help="Path to the elf file")
args = parser.parse_args()

elf_path = os.path.abspath(args.elf_file).replace("\\","/")

script_dirname = os.path.dirname(os.path.abspath(__file__))
config_path = os.path.join(script_dirname, "openocd.cfg").replace("\\", "/")

# Flash target
cmd = 'openocd -f {} -c "program {} verify reset exit"'.format(config_path, elf_path)
os.system(cmd)
