#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line, label;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> label >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string line, key, value;
  float mem_total, mem_free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {          
          mem_total = stof(value);
        }
        if (key == "MemFree:") {          
          mem_free = stof(value);
        }
      }
    }
    return (mem_total-mem_free)/mem_total;
  }   
  return 0.0; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  string uptime;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime ;
    return stol(uptime); 
  } 
  return 0;  
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line;
  long total = 0;
  vector<string> utilization = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <=kSteal_; i++) {
      total += stol(utilization[i]);
  }
  return total; 
} 

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::string line;
  long utime, stime,cutime,cstime;
  std::vector<std::string> stats;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {   
    while (std::getline(stream, line, ' ')) {
        stats.push_back(line);
    }
    utime = stol(stats[13]);
    stime = stol(stats[14]);
    cutime = stol(stats[15]);
    cstime = stol(stats[16]);
    return utime+stime+cutime+cstime;   
  }      
  return 0; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  vector<string> jiffies;
  vector<string> utilization = LinuxParser::CpuUtilization();
  return stol(utilization[kIdle_]) + stol(utilization[kIOwait_]); 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::string line, key, value;
  std::vector<std::string> cpu_stats;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {   
    std::getline(stream, line) ;
    std::istringstream linestream(line);
    while (std::getline(linestream, value, ' ')) {      
      /* if (key.rfind("cpu",0) == 0) {          
        cpu_stats.push_back(line);
      }*/
      if ((value == "cpu") || (value == "")){
        continue;
      }
      cpu_stats.push_back(value);
    }
    return cpu_stats;
  }      
  return vector<string>{};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {          
          return stoi(value);
        }
      }
    }
  }   
  return 0;    
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {          
          return stoi(value);
        }
      }
    }
  }   
  return 0;  

}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::string line, cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {  
    std::getline(stream, line);
    return line; 
  }
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::string line, key, value;
  std::ostringstream oss;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {          
          oss << std::fixed << std::setfill ('0') << std::setprecision (2) << stof(value)/1000;          
          return oss.str();
        }
      }
    }
  }   
  return string("0"); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string line, key, value, uid_value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {          
          return uid_value = value;
        }
      }
    }
  }   
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string line,username,x, id;
  std::string uid = Uid(pid);
  std::ifstream pass_stream(kPasswordPath);
  if (pass_stream.is_open()) {   
    while (std::getline(pass_stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >>id) {
        if (id == uid) {          
          return username;
        }
      }
    }
  }
  return std::string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line;
  std::vector<std::string> stats;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line, ' ')) {
        stats.push_back(line);
    }
    return stol(stats[21])/sysconf(_SC_CLK_TCK);
  }      
  return 0; 
}