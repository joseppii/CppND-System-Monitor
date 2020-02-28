#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

// Return the aggregate CPU utilization
float Processor::Utilization() { 
	long diff_active, diff_total;

	long total_jiffies = LinuxParser::Jiffies();
	long active_jiffies = LinuxParser::ActiveJiffies();

	diff_total = total_jiffies - total_jiffies_;
	diff_active = active_jiffies - active_jiffies_;

	return float(diff_total/diff_active) ;    
}