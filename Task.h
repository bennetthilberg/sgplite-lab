#ifndef TASK_H
#define TASK_H

#include <cmath>
#include <iostream>
#include <string>

class Task {
  public:
    virtual double CheckOutput(float output, float inputs[4]) = 0;
    virtual std::string name() const = 0;
};
class NewTask : public Task {
  public:
    double CheckOutput(float output, float inputs[4]) override {

        for (int i = 0; i < 4; i++) {
            // For each input value in the array
            // Calculate the square of it
            float square = inputs[i] * inputs[i];
            //std::cout << "Trying to solve square NewTask task..." << std::endl;

            // Since these are floats, allowing for some floating point error
            if (fabs(output - square) < 0.001) {
                // They squared an input, yey!
                std::cout << "\033[32mSquared!\033[0m" << std::endl;
                // Give them 5 points for solving this task
                return 5.5;
            }
            /*else{
                std::cout << "Not squared :(" << std::endl;
            }*/
        }
        return 0.1;
    }
    std::string name() const override {
      return "Square";
    }
};
#endif
