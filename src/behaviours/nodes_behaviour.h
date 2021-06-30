#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_

#include <map>
#include <queue>
#include <unordered_map>
#include <iostream>

class NodeStatus {
 public:
  enum {
    Alive,
    Paused,
    Crashed
  } status;
  double time;
};


class NodesBehaviour {
 public:
  std::optional<double> getCompletionTime(double current_time, double duration, int id) {
    // Get this node's queue
    auto& status_queue = id_status_map_[id];
    if (status_queue.empty()) {
      status_queue.push_back({NodeStatus::Alive, 0.0});
    }

    // Remove any status that is too old to be relevant
    while (status_queue.size() > 1 && status_queue.at(1).time < current_time) {
      status_queue.pop_front();
    }
    status_queue.front().time = current_time;

    // Get the completion time of this task on this node
    for (auto status = status_queue.begin(); ; status++) {
      if (status + 1 == status_queue.end()) {
        status_queue.push_back(getNextStatusTime(status->time, id));
        status = status_queue.end() - 2;  // Avoid errors caused by pointer invalidation
      }
      if (status->status == NodeStatus::Alive) {
        double status_duration = (status + 1)->time - status->time;
        if (duration < status_duration) {
          return status->time + duration;
        } else {
          duration -= status_duration;
        }
      } else if (status->status == NodeStatus::Crashed) {
        return std::nullopt;
      }
    }
  }

 protected:
  // Get the start time of the status following the status starting at current_time.
  virtual NodeStatus getNextStatusTime(double current_time, int id) {
    return {NodeStatus::Crashed, std::numeric_limits<double>::infinity()};
  };

 private:
  std::unordered_map<int, std::deque<NodeStatus>> id_status_map_;
};

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOURS_NODES_BEHAVIOUR_H_
