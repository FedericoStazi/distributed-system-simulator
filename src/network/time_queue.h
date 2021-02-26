//
// Created by federico on 06/01/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__TIME_QUEUE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__TIME_QUEUE_H_

#include <utility>
#include <map>

namespace dssim {

template<typename T>
class TimeQueue {
 public:
  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;
  void insert(double time, T t);
  std::pair<double, T> pop();
  // Add k to all times in the queue
  void add(double k);
  // Expand further away from t by a factor k
  // i.e. x' = t + (x - t) * k
  void multiply(double t, double k);
 private:
  std::multimap<double, T> elements_;
};

template<typename T>
bool TimeQueue<T>::empty() const {
  return elements_.empty();
}

template<typename T>
size_t TimeQueue<T>::size() const {
  return elements_.size();
}

template<typename T>
void TimeQueue<T>::insert(double time, T t) {
  elements_.insert({time, t});
}

template<typename T>
std::pair<double, T> TimeQueue<T>::pop() {
  auto top = elements_.begin();
  auto top_copy = *top;
  elements_.erase(top);
  return top_copy;
}

template<typename T>
void TimeQueue<T>::add(double k) {
  std::multimap<double, T> elements_copy;
  elements_.clear();
  for (auto &[time, element] : elements_copy) {
    element[time + k] = element;
  }
}

template<typename T>
void TimeQueue<T>::multiply(double t, double k) {
  std::multimap<double, T> elements_copy;
  elements_.clear();
  for (auto &[time, element] : elements_copy) {
    element[t + (time - t) * k] = element;
  }
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__TIME_QUEUE_H_
