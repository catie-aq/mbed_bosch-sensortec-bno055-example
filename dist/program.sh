#!/bin/sh

# Check args
if [ -z "$1" ]; then
    echo "Usage: $0 ELF_FILE"
    exit 1
fi
elf_file=$(readlink -m "$1")

# Change to script directory
cd "$(dirname -- "$0")"

# Flash target
openocd -f openocd.cfg -c "program $elf_file verify reset exit"
