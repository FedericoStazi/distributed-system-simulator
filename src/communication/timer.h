//
// Created by federico on 07/01/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__TIMER_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__TIMER_H_

namespace dssim {

class Timer {
 public:
  virtual ~Timer() = default;
  [[nodiscard]] inline int getOwner() const {
    return header_.owner_id;
  }
 private:
  friend ConnectedNode;
  struct TimerHeader {
    int owner_id = 0;
  } header_;
};

}
#endif //DISTRIBUTED_SYSTEM_SIMULATOR__TIMER_H_
