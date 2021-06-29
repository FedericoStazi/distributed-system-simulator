#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_

class NodesBehaviour {
 public:
  double getProcessingTime(double current_time, double duration, int id) {
    return current_time;
  }
};

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_
