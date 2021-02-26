//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOUR_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOUR_H_

#include <vector>

namespace dssim {

class Message;

class NetworkBehaviour {
 public:
  virtual std::vector<double> getLatencies(const Message& message) = 0;
  virtual void getInterference(Message& message) = 0;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOUR_H_
