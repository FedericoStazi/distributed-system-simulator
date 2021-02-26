//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_

namespace dssim {

template<typename EventType, int duration>
class Accepts : private virtual AcceptingNode<EventType>,
                protected virtual ConnectedNode,
                public virtual Node {
 private:
  virtual Transaction getTransaction(EventType message) override;
 protected:
  virtual void onEvent(EventType message) = 0;
};

template<typename EventType, int duration>
Transaction Accepts<EventType,
                    duration>::getTransaction(EventType message) {
  return Transaction(([=]() { onEvent(message); }), duration);
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
