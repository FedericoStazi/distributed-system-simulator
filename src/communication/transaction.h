#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__TRANSACTION_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__TRANSACTION_H_

#include <functional>
#include <utility>

namespace dssim {

class Transaction {
 public:
  explicit Transaction(std::function<void()> func, int duration) :
      function_(std::move(func)), duration_(duration) {}
  explicit Transaction(std::function<void()> func, std::function<void(double)> trace, int duration) :
      function_(std::move(func)), trace_(trace), duration_(duration), has_trace_(true) {}
  [[nodiscard]] inline int getDuration() const {
    return duration_;
  }
  void inline runTransaction() const {
    function_();
  }
  void inline runTrace(double time) const {
    if (has_trace_) {
      trace_(time);
    }
  }
 private:
  const std::function<void()> function_;
  const std::function<void(double)> trace_;
  int duration_;
  bool has_trace_ = false;
};

}
#endif //DISTRIBUTED_SYSTEM_SIMULATOR__TRANSACTION_H_
