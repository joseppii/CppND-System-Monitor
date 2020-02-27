#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 

    std::ostringstream str;
    int days = seconds / 86400;
    int hours = (seconds % 86400) / 3600;
    int minutes = (seconds % 3600)/60;
    int secs = seconds % 60;
    
    str <<std::dec << std::setfill('0') 
                   << std::setw(2) << days << ":" 
                   << std::setw(2) << hours << ":"
                   << std::setw(2) << minutes << ":" 
                   << std::setw(2) << secs;
    return str.str(); 
}
