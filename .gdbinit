target remote | openocd -f dist/openocd.cfg -c "gdb_port pipe"
monitor reset halt
load
break main
continue
