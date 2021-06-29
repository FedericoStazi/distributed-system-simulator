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
  // Get the number of copies of the message that will be received.
  // Can be 0 if the message is lost.
  virtual int getDuplication(const Message& message) {
    return 1;
  };

  // Get the latency of the message in ms.
  // duplicate_index is used to distinguish between the duplicates of the message.
  virtual double getLatency(const Message& message, int duplicate_index) {
    return 0.0;
  };

  // Apply interference (i.e. modifications) to the message
  virtual void applyInterference(Message& message) {};
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOUR_H_
