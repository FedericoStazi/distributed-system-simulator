//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_

namespace dssim::behaviours {

class LocationBased : public virtual NetworkBehaviour {
 public:
  // Set the location of node id to be (x,y).
  void addLocation(int id, double x, double y) {
    position[id] = std::make_pair(x, y);
  }

  // Set a random location for node id.
  void addRandomLocation(int id) {
    addLocation(id,
                std::uniform_real_distribution(0.0, 1.0)(gen_),
                std::uniform_real_distribution(0.0, 1.0)(gen_));
  }

  int getDuplication(const Message& message) override {
    if (!position.count(message.getSender()) || !position.count(message.getReceiver())) {
      return 0;
    }
    double dx = position[message.getSender()].first - position[message.getReceiver()].first;
    double dy = position[message.getSender()].second - position[message.getReceiver()].second;
    return !isDropped(sqrt(dx * dx + dy * dy));
  }

  double getLatency(const Message& message, int duplicate_index) override {
    double dx = position[message.getSender()].first - position[message.getReceiver()].first;
    double dy = position[message.getSender()].second - position[message.getReceiver()].second;
    return getEdgeLatency(sqrt(dx * dx + dy * dy));
  }

 protected:
  // Get the latency of a message travelling over a certain distance.
  virtual double getEdgeLatency(double distance) = 0;
  // Determine if a message travelling over a certain distance is dropped.
  virtual bool isDropped(double distance) = 0;

 private:
  std::unordered_map<int, std::pair<double, double>> position;
  std::default_random_engine gen_{RANDOM_SEED};
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_
