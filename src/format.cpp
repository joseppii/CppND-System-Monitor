#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 

    std::ostringstream str;
    int hours = seconds / 3600;
    int minutes = (seconds % 3600)/60;
    int secs = seconds % 60;
    
    str <<std::dec << std::setw(2) << std::setfill('0') << hours << ":" << minutes << ":" << secs;
    return str.str(); 
}
