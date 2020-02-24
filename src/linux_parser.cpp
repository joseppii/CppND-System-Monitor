#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  long uptime;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime ;
  } 

  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::string line, cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {  
    std::getline(stream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {   
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {          
          return value;
        }
      }
    }
  }   
  return string(); 
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
      //std::replace(line.begin(), line.end(), ':', ' ');
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
  long uptime;
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