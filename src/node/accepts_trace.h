//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_

#include <iostream>
#include <iomanip>

namespace dssim {

// This is a temporary solution. If future compilers will support it, use:
// template<typename T, int duration, std::string traceName>
template<typename T, int duration>
class AcceptsTrace : public Accepts<T, duration> {
 private:
  virtual Transaction getTransaction(T message) override;
 private:
  void getTrace(double time) const;
};

template<typename T, int duration>
Transaction AcceptsTrace<T,
                         duration>::getTransaction(T message) {
  return Transaction(([=]() { this->onMessage(message); }),
                     ([=](double time) { getTrace(time); }),
                     duration);
}

template<typename T, int duration>
void AcceptsTrace<T, duration>::getTrace(double time) const {
  std::cout
      << "****************************************************************\n"
      << "Time: " << std::to_string(time) << "\t\t" <<
      "Node id: " << std::to_string(ConnectedNode::getID()) << "\t\t" <<
      typeid(*this).name() << " on " << typeid(T).name() << std::endl;
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__ACCEPTS_TRACE_H_
