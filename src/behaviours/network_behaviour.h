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
  // Get the number of copies of the message and their latencies, e.g.:
  // A message with 1ms latency:  {1.0}
  // A duplicated message:        {1.23, 4.56}
  // A lost message:              {}
  virtual std::vector<double> getLatencies(const Message& message) = 0;
  // Apply interference (i.e. modifications) to the message
  virtual void applyInterference(Message& message) = 0;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOUR_H_
