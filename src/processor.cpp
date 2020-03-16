#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

// Return the aggregate CPU utilization
float Processor::Utilization() { 
	long diff_active, diff_total;

	long total_jiffies_new = LinuxParser::Jiffies();
	long active_jiffies_new = LinuxParser::ActiveJiffies();
	
	diff_total = total_jiffies_new - total_jiffies_;
	diff_active = active_jiffies_new - active_jiffies_;
	
	total_jiffies_ = total_jiffies_new;
	active_jiffies_ = active_jiffies_new;

 	return ((float)diff_active / (float)diff_total);
   
}