import subprocess
import os
from contextlib import contextmanager


def main():
    subprocess.check_call(["touch .mbed"], shell=True)
    subprocess.check_call(["mbed toolchain GCC_ARM"], shell=True)
    target = []
    with open("mbed-os/targets/targets.json") as file:
        for line in file:
            if "ZEST" in line:
                target.append(line.split('"')[1])

    for t in target:
        subprocess.check_call(["mbed target " + t], shell=True)
        subprocess.check_call(["mbed compile"], shell=True)


if __name__ == "__main__":
    main()
