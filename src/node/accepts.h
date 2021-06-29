//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_

namespace dssim {

template<typename T, int duration>
class Accepts : private virtual AcceptingNode<T>, protected virtual ConnectedNode, public virtual Node {
 private:
  virtual Transaction getTransaction(T message) override;
 protected:
  virtual void onMessage(T message) = 0;
};

template<typename T, int duration>
Transaction Accepts<T, duration>::getTransaction(T message) {
  return Transaction(([=]() { onMessage(message); }), duration);
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_H_
