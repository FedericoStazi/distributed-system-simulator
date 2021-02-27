//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_

namespace dssim::behaviours {

class LocationBased : public virtual NetworkBehaviour {
 public:
  void addLocation(int id, double x, double y) {
    position[id] = std::make_pair(x, y);
  }
  void addRandomLocation(int id) {
    addLocation(id,
                std::uniform_real_distribution(0.0, 1.0)(gen_),
                std::uniform_real_distribution(0.0, 1.0)(gen_));
  }
  std::vector<double> getLatencies(const Message &message) override {
    if (position.count(message.getSender())) {
      if (position.count(message.getReceiver())) {
        double dx = position[message.getSender()].first -
                    position[message.getReceiver()].first;
        double dy = position[message.getSender()].second -
                    position[message.getReceiver()].second;
        return getEdgeLatencies(sqrt(dx * dx + dy * dy));
      }
    }
    return {};
  }
 protected:
  virtual std::vector<double> getEdgeLatencies(double distance) = 0;
 private:
  std::unordered_map<int, std::pair<double, double>> position;
  std::default_random_engine gen_{RANDOM_SEED};
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_LOCATION_BASED_H_
