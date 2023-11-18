#ifndef TASK_H
#define TASK_H

#include <cmath>
#include <string>

class Task {
public:
  virtual double CheckOutput(float output, float inputs[4]) = 0;
  virtual std::string name() const = 0;
};

#endif
