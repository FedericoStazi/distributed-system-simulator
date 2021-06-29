#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_

namespace dssim {

template<typename T>
class AcceptingNode : public virtual Node {
 public:
  virtual Transaction getTransaction(T message) = 0;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTING_NODE_H_
