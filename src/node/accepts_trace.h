//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_

#include <iostream>

namespace dssim {

// This is a temporary solution. If future compilers will support it, use:
// template<typename MessageType, int duration, std::string traceName>
template<typename MessageType, int duration>
class AcceptsTrace : public Accepts<MessageType, duration> {
 private:
  virtual Transaction getTransaction(MessageType message) override;
 private:
  void getTrace(int time) const;
};

template<typename MessageType, int duration>
Transaction AcceptsTrace<MessageType,
                         duration>::getTransaction(MessageType message) {
  return Transaction(([=]() { this->onMessage(message); }),
                     ([=](int time) { getTrace(time); }),
                     duration);
}

template<typename MessageType, int duration>
void AcceptsTrace<MessageType, duration>::getTrace(int time) const {
  std::cout
      << "****************************************************************\n"
         "Time: " << std::to_string(time) << "\t\t" <<
      "Node id: " << std::to_string(ConnectedNode::getID()) << "\t\t" <<
      typeid(*this).name() << " on " << typeid(MessageType).name() << std::endl;
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
