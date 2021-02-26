//
// Created by federico on 06/01/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_

#include <map>
#include <memory>

namespace dssim {

class Node;

class Network {
 public:
  void addNode(std::unique_ptr<dssim::Node> node);
  void start();
  template<typename T>
  void sendMessage(T message, int destination_id);
  template<typename T>
  void broadcastMessage(T message);
 private:
  std::map<int, std::unique_ptr<dssim::Node>> nodes_;
  TimeQueue<Transaction> time_queue_;
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
  while (!time_queue_.empty()) {
    const auto&[next_time, transaction] = time_queue_.pop();
    time_ = std::max(time_, next_time);
    transaction.runTrace(time_);
    transaction.runTransaction();
  }
}

template<typename T>
void dssim::Network::sendMessage(T message, int destination_id) {
  try {  // Non-silent fail would be a covert channel
    if (nodes_.count(destination_id)) {
      auto &node = dynamic_cast<AcceptingNode<T> &>(*nodes_[destination_id]);
      auto transaction = node.getTransaction(message);
      double time = time_ + transaction.getDuration();
      time_queue_.insert(time, transaction);
    }
  } catch (std::bad_cast &e) {}
}

template<typename T>
void dssim::Network::broadcastMessage(T message) {
  for (const auto&[id, _] : nodes_) {
    sendMessage(message, id);
  }
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__NETWORK_H_
