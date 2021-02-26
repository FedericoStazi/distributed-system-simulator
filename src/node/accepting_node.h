//

// Created by federico on 06/01/2021.
//
#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_

namespace dssim {

template<typename MessageType>
class AcceptingNode : public virtual Node {
 public:
  virtual Transaction getTransaction(MessageType message) = 0;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_
