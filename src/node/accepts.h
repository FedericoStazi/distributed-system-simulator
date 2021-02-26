//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_

namespace dssim {

template<typename MessageType, int duration>
class Accepts : private virtual AcceptingNode<MessageType>,
                protected virtual ConnectedNode,
                public virtual Node {
 private:
  virtual Transaction getTransaction(MessageType message) override;
 protected:
  virtual void onMessage(MessageType message) = 0;
};

template<typename MessageType, int duration>
Transaction Accepts<MessageType,
                    duration>::getTransaction(MessageType message) {
  return Transaction(([=]() { onMessage(message); }), duration);
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
