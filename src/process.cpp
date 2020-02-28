#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid) {
    cmd_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    long jiffies, uptime;
    jiffies = LinuxParser::ActiveJiffies(pid_);
    uptime = LinuxParser::UpTime(pid_);

    long timediff = uptime - uptime_;
	if (timediff == 0)
 		return 0;
		
    cpu_ = (float)(jiffies - jiffies_) / (float)(uptime - uptime_); 
	jiffies_ = jiffies;
	uptime_ = uptime;
	return cpu_;
}

// Return the command that generated this process
string Process::Command() { return cmd_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overloaded "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
	if (cpu_ > a.cpu_) 
		return true;
	else
		return false;
}