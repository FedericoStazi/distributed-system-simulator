//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_

#include <iostream>

namespace dssim {

// This is a temporary solution. If future compilers will support it, use:
// template<typename EventType, int duration, std::string traceName>
template<typename EventType, int duration>
class AcceptsTrace : public Accepts<EventType, duration> {
 private:
  virtual Transaction getTransaction(EventType message) override;
 private:
  void getTrace(int time) const;
};

template<typename EventType, int duration>
Transaction AcceptsTrace<EventType,
                         duration>::getTransaction(EventType message) {
  return Transaction(([=]() { this->onEvent(message); }),
                     ([=](int time) { getTrace(time); }),
                     duration);
}

template<typename EventType, int duration>
void AcceptsTrace<EventType, duration>::getTrace(int time) const {
  std::cout
      << "****************************************************************\n"
      << "Time: " << std::to_string(time) << "\t\t" <<
      "Node id: " << std::to_string(ConnectedNode::getID()) << "\t\t" <<
      typeid(*this).name() << " on " << typeid(EventType).name() << std::endl;
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
