//
// Created by federico on 06/01/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_

#include <map>
#include <memory>

namespace dssim {

class Node;
class Message;
class Timer;

class Network {
 public:
  void addNode(std::unique_ptr<dssim::Node> node);
  void start();
  template<typename MessageType>
  void sendMessage(MessageType message, int destination_id);
  template<typename MessageType>
  void broadcastMessage(MessageType message);
  template<typename TimerType>
  void startTimer(TimerType timer);
 private:
  std::map<int, std::unique_ptr<dssim::Node>> nodes_;
  TimeQueue<Transaction> events_queue_;
  unsigned int next_id_ = 0;
  double time_ = 0;
};

void dssim::Network::addNode(std::unique_ptr<dssim::Node> node) {
  nodes_[++next_id_] = std::move(node);
}

void dssim::Network::start() {
  for (const auto&[id, node] : nodes_) {
    node->start(std::shared_ptr<Network>(this), id);
  }
  while (!events_queue_.empty()) {
    const auto&[next_time, transaction] = events_queue_.pop();
    time_ = std::max(time_, next_time);
    transaction.runTrace(time_);
    transaction.runTransaction();
  }
}

template<typename MessageType>
void dssim::Network::sendMessage(MessageType message, int destination_id) {
  static_assert(std::is_base_of<Message, MessageType>::value,
                "sendMessage expects a subclass of Message");
  try {  // Non-silent fail would be a covert channel
    if (nodes_.count(destination_id)) {
      auto &node = dynamic_cast<AcceptingNode<MessageType> &>(*nodes_[destination_id]);
      auto transaction = node.getTransaction(message);
      double time = time_ + transaction.getDuration();
      events_queue_.insert(time, transaction);
    }
  } catch (std::bad_cast &e) {}
}

template<typename MessageType>
void dssim::Network::broadcastMessage(MessageType message) {
  static_assert(std::is_base_of<Message, MessageType>::value,
                "broadcastMessage expects a subclass of Message");
  for (const auto&[id, _] : nodes_) {
    sendMessage(message, id);
  }
}
template<typename TimerType>
void Network::startTimer(TimerType timer) {
  static_assert(std::is_base_of<Timer, TimerType>::value,
                "startTimer expects a subclass of Timer");
  try {  // Non-silent fail would be a covert channel
    auto &node = dynamic_cast<AcceptingNode<TimerType> &>(*nodes_[timer.getOwner()]);
    auto transaction = node.getTransaction(timer);
    double time = time_ + transaction.getDuration();
    events_queue_.insert(time, transaction);
  } catch (std::bad_cast &e) {}
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_
