import sys
import tests
import os
import os.path
import time
import subprocess

VERBOSE = False
device_serial_number = None

def debug(string):
	if VERBOSE:
		print(string)

def start_command(command):
	debug("[Running]: " + command)
	proc  = subprocess.Popen(
			command, universal_newlines=True, shell=True,
			stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	return proc

def start_shell_command(command):
	return start_command(f"adb -s {device_serial_number} shell '{command}'")

def communicate_command(proc):
	log,error = proc.communicate()
	return log

def still_running(proc):
	poll = proc.poll()
	return poll == None

def terminate_command(proc):
	assert(still_running(proc) == True)
	proc.terminate()
	time.sleep(1)
	assert(still_running(proc) == False)

def run_command(command):
	proc = start_command(command)
	log = communicate_command(proc)
	debug("[LOG]: " + log.rstrip().lstrip())
	return log

def run_shell_command(command):
	return run_command(f"adb -s {device_serial_number} shell '{command}'")

def kill_swan():
	grep_log = run_shell_command("top -b -n 1 | grep \"swan\"")
	lines = grep_log.split("\n")
	for line in lines:
		if "swan_phone" in line:
			pid = line.split()[0]
			run_shell_command(f"kill {pid}")
			return True
	return False

def check_device(device):
	global device_serial_number
	log = run_command("adb devices")
	if "command not found" in log:
		assert False, "Android Debug Bridge (adb) not found!"
	serial_numbers = []
	log_lines = log.split("\n")
	for log_line in log_lines:
		log_phrases = [phrase.rstrip().lstrip() for phrase in log_line.split() if phrase != '']
		if len(log_phrases) == 2 and log_phrases[1] == "device":
			serial_numbers.append(log_phrases[0])
	if len(serial_numbers) == 0:
		assert False, "No online device connected to machine! Use \"adb devices\" to check devices!"
	if device != None:
		assert device in serial_numbers, f"Device \"{device}\" is not online. Use \"adb devices\" to check devices!"
		device_serial_number = device
	else:
		device_serial_number = serial_numbers[0]