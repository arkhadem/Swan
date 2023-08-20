import sys
import tests
import os
import os.path
import time


libraries = tests.lib_list
kernels = tests.tests_bench

for argv in sys.argv[1:]:
    if argv in libraries:
        libraries = [argv]
    else:
        assert len(libraries) == 1, "unknow argv \"{}\". If it's a kernel please insert library first!\n".format(argv)
        kernels = {libraries[0] : [argv]}

def run_command(command):
    print("Running: " + command)
    os.system(command)

def run_benchmark(library, kernel):
    run_command("./swan_local -p compare -l {} -k {} 2>&1 | tee ./data/compare/temp.log".format(library, kernel))
    log_file = open("./data/compare/temp.log", "r")
    lines = log_file.readlines()
    log_file.close()
    for line in lines:
        if "!=" in line:
            return False
    return True
    
os.system("mkdir -p ./data/compare/ &> /dev/null")
time.sleep(2)

for library in libraries:
    print("evaluating: {}".format(library))

    for kernel in kernels[library]:
        time.sleep(1)
        if (run_benchmark(library, kernel) == False):
            print("failed comparison for library {} kernel {}".format(library, kernel))