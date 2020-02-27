#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
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
    long jiffies_t0, jiffies_t1, uptime_t0, uptime_t1;
    jiffies_t0 = LinuxParser::ActiveJiffies(pid_);
    uptime_t0 = LinuxParser::UpTime(pid_);
    usleep(100000);
    jiffies_t1 = LinuxParser::ActiveJiffies(pid_);
    uptime_t1 = LinuxParser::UpTime(pid_);

    return (float)(jiffies_t1 - jiffies_t0) / (float)(uptime_t1 - uptime_t0); 
}

// Return the command that generated this process
string Process::Command() { return cmd_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }