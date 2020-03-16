#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    string label, value;
    vector<string> cpu_stats = LinuxParser::CpuUtilization();
    string &cpu_stat = cpu_stats[0];
    std::istringstream linestream(cpu_stat);
    while (linestream >> label >> value) {      
        if (label == ("cpu")) {          
            std::cout << "Got jiffies!\n"; 
        }
    }     
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_ = {};
    for(int pid:pids){
        Process proc(pid);    
        processes_.emplace_back(proc);
    }
    sort(processes_.begin(), processes_.end());
    
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
