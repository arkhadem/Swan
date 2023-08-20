import time
import general
import mask

def get_performance_run_command(KERNEL_DIR, platform, library, kernel, core = None):
	mask_str = mask.get_mask(core)
	return f"cd {KERNEL_DIR} && {mask_str} ./swan_phone -p {platform} -l {library} -k {kernel}"

def run_perf_process(KERNEL_DIR, platform, library, kernel, core = None):
	mask.init_mask()
	while True:
		logs = general.run_shell_command(get_performance_run_command(KERNEL_DIR, platform, library, kernel, core))
		if "Successfully" in logs:
			break
	mask.finish_mask(core)
	return logs

def parse_performance_logs(logs):
	iterations = 0
	total_time = 0
	iteration_time = 0
	granularity = 0
	kernel_time = 0
	log_lines = logs.split("\n")
	for log_line in log_lines:
		log_phrases = log_line.split()
		if len(log_phrases) < 2:
			continue
		if log_phrases[0] == "iterations:":
			iterations = int(log_phrases[1])
		elif log_phrases[0] == "total_time:":
			total_time = float(log_phrases[1])
		elif log_phrases[0] == "iteration_time:":
			iteration_time = float(log_phrases[1])
		elif log_phrases[0] == "granularity:":
			granularity = int(log_phrases[1])
		elif log_phrases[0] == "kernel_time:":
			kernel_time = float(log_phrases[1])
	return iterations, total_time, iteration_time, granularity, kernel_time

def get_performance(KERNEL_DIR, platform, library, kernel, mask):
	logs = run_perf_process(KERNEL_DIR, platform, library, kernel, mask)
	return parse_performance_logs(logs)