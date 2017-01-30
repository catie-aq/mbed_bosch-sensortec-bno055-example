# Print whole array
set print elements 0

# Load program into target
target remote | openocd -f dist/openocd.cfg -c "gdb_port pipe"
monitor reset halt
load
disconnect

# Restart target and break at main()
target remote | openocd -f dist/openocd.cfg -c "gdb_port pipe"
monitor reset halt
break main
continue
