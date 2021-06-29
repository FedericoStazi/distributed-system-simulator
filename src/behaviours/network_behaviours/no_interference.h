//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_NO_INTERFERENCE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_NO_INTERFERENCE_H_

namespace dssim::behaviours {

class NoInterference : public virtual NetworkBehaviour {
  virtual void applyInterference(Message& message) {}
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_NO_INTERFERENCE_H_
