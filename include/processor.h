#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  // Declare any necessary private members
 private:
  long total_jiffies_{0};
  long active_jiffies_{0};
};

#endif