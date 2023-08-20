import time
import general
import mask

def get_now_current():
	power = int(general.run_shell_command("cat /sys/class/power_supply/battery/current_avg").rstrip().lstrip())
	return power

def get_now_voltage():
	power = int(general.run_shell_command("cat /sys/class/power_supply/battery/voltage_avg").rstrip().lstrip())
	return power

def get_avg_current(NUMSAMPLES = 10, FUNC = "min"):
	last_currents = []
	while True:
		if len(last_currents) == NUMSAMPLES:
			if max(last_currents) - min(last_currents) < 20:
				if FUNC == "min":
					# general.debug("Min: {}".format(min(last_currents)))
					# print("{}".format(last_currents))					
					return min(last_currents)
				else:
					# general.debug("Max: {}".format(max(last_currents)))
					# print("{}".format(last_currents))					
					return max(last_currents)
			# remove 1 element from last power
			last_currents = last_currents[1:]
		last_currents.append(get_now_current())
		# general.debug("{}".format(last_currents))
		time.sleep(1)

def get_power_run_command(KERNEL_DIR, platform, library, kernel, core = None):
	mask_str = mask.get_mask(core)
	return f"cd {KERNEL_DIR} && {mask_str} ./swan_phone -p {platform} -l {library} -k {kernel} -i 10000000"

def start_power_process(KERNEL_DIR, platform, library, kernel, core = None):
	mask.init_mask()
	while True:
		proc = general.start_shell_command(get_power_run_command(KERNEL_DIR, platform, library, kernel, core))
		time.sleep(1)
		if general.still_running(proc) == True:
			break
	mask.finish_mask(core)
	return proc

def get_power(KERNEL_DIR, platform, library, kernel, mask):
	pre_current = get_avg_current(20, "max")
	pre_voltage = get_now_voltage()
	proc = start_power_process(KERNEL_DIR, platform, library, kernel, mask)
	post_current = get_avg_current(20, "min")
	post_voltage = get_now_voltage()
	assert pre_current > post_current
	assert (pre_voltage + post_voltage) > 0
	general.terminate_command(proc)
	assert general.kill_swan()
	return pre_current, pre_voltage / 1000.0000, post_current, post_voltage / 1000.0000