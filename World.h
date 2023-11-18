#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "Task.h"

#include "Org.h"

class OrgWorld : public emp::World<Organism> {
  emp::Random &random;
  std::vector<emp::WorldPosition> reproduce_queue;
  std::vector<Task*> tasks{new NewTask()};
  int successfulOrgs = 0;

public:
  OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) { }

  ~OrgWorld() {}

  const pop_t &GetPopulation() { return pop;}

  void Update() {
    emp::World<Organism>::Update();

    //Process each organism
    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
    for(int i : schedule) {
      if(!IsOccupied(i)) {continue;}
      pop[i]->Process(i);
      if(pop[i]->cpu.state.task_succeeded && !pop[i]->cpu.state.countedSuccess){
        successfulOrgs++;
        pop[i]->cpu.state.countedSuccess = true;
      }
    }
    //std::cout << "Finished processing organisms. Checking for reproduction next..." << std::endl;

    //Time to allow reproduction for any organisms that ran the reproduce instruction
    for (emp::WorldPosition location : reproduce_queue) {
      if (!IsOccupied(location)) {
        return;
      }
      std::optional<Organism> offspring =
          pop[location.GetIndex()]->CheckReproduction();
      if (offspring.has_value()) {
        DoBirth(offspring.value(), location.GetIndex());
      }
    }
    reproduce_queue.clear();

    std::cout << "successfulOrgs at end of update: " << successfulOrgs << std::endl;
  }

  void CheckOutput(float output, OrgState &state) {
    //TODO: Check if the organism solved a task!
    //std::cout << "Checking output (world)..." << std::endl;
    for(Task* taskPtr : tasks){
      double preCheckPoints = state.points;
      state.points += taskPtr->CheckOutput(output, state.last_inputs);
      if((state.points - preCheckPoints) > 0.2){
        state.task_succeeded = true;
      }
    }
  }

  void ReproduceOrg(emp::WorldPosition location) {
    // Wait until after all organisms have been processed to perform
    // reproduction. If reproduction happened immediately then the child could
    // ovewrite the parent, and then we would be running the code of a deleted
    // organism
    reproduce_queue.push_back(location);
  }
};

#endif