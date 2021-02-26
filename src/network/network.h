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
class NetworkBehaviour;

class Network {
 public:
  int addNode(std::unique_ptr<Node> node);
  void setNetworkBehaviour(std::unique_ptr<NetworkBehaviour> network_behaviour);
  void start();
  template<typename MessageType>
  void sendMessage(const MessageType message);
  template<typename TimerType>
  void startTimer(TimerType timer);
 private:
  std::map<int, std::unique_ptr<Node>> nodes_;
  TimeQueue<Transaction> events_queue_;
  unsigned int next_id_ = 0;
  double current_time_ = 0;
  std::unique_ptr<NetworkBehaviour> network_behaviour_;
};

int Network::addNode(std::unique_ptr<Node> node) {
  next_id_++;
  nodes_[next_id_] = std::move(node);
  return next_id_;
}

void Network::setNetworkBehaviour(std::unique_ptr<NetworkBehaviour> network_behaviour) {
  network_behaviour_ = std::move(network_behaviour);
}

void Network::start() {
  for (const auto&[id, node] : nodes_) {
    node->start(std::shared_ptr<Network>(this), id);
  }
  while (!events_queue_.empty()) {
    const auto&[next_time, transaction] = events_queue_.pop();
    current_time_ = std::max(current_time_, next_time);
    transaction.runTrace(current_time_);
    transaction.runTransaction();
  }
}

template<typename MessageType>
void Network::sendMessage(const MessageType message) {
  static_assert(std::is_base_of<Message, MessageType>::value,
                "sendMessage expects a subclass of Message");
  if (message.isBroadcast()) {
    for (const auto&[id, _] : nodes_) {
      auto message_copy = message;
      message_copy.setBroadcastReceiver(id);
      sendMessage(message_copy);
    }
  } else {
    try {  // Non-silent fail would be a covert channel
      if (nodes_.count(message.getReceiver())) {
        auto &node =
            dynamic_cast<AcceptingNode<MessageType> &>(*nodes_[message.getReceiver()]);
        std::vector<double> latencies = {0.0};
        if (network_behaviour_) {
          latencies = network_behaviour_->getLatencies(message);
        }
        for (double latency : latencies) {
          auto message_copy = message;
          if (network_behaviour_) {
            network_behaviour_->getInterference(message_copy);
          }
          auto transaction = node.getTransaction(message_copy);
          double time = current_time_ + transaction.getDuration() + latency;
          events_queue_.insert(time, transaction);
        }
      }
    } catch (std::bad_cast &e) {}
  }
}

template<typename TimerType>
void Network::startTimer(TimerType timer) {
  static_assert(std::is_base_of<Timer, TimerType>::value,
                "startTimer expects a subclass of Timer");
  try {  // Non-silent fail would be a covert channel
    auto &node =
        dynamic_cast<AcceptingNode<TimerType> &>(*nodes_[timer.getOwner()]);
    auto transaction = node.getTransaction(timer);
    double time = current_time_ + transaction.getDuration();
    events_queue_.insert(time, transaction);
  } catch (std::bad_cast &e) {}
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_
