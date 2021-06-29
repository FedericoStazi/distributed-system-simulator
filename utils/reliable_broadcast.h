//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_UTILS_RELIABLE_BROADCAST_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_UTILS_RELIABLE_BROADCAST_H_

#include "../dssim.h"

namespace dssim::utils {

template<typename T>
struct ReliableBroadcastMessage : public dssim::Message {
  explicit ReliableBroadcastMessage(T message) : message(std::move(message)) {}
  const T message;
  std::unordered_set<int> acks{};
};

template<typename T>
struct ReliableBroadcastTimeout : public dssim::Message {
  explicit ReliableBroadcastTimeout(T message) : message(std::move(message)) {}
  const T message;
};

template<typename T>
struct ReliableBroadcastSuccess : public dssim::Message {
  explicit ReliableBroadcastSuccess(T message) : message(std::move(message)) {}
  const T message;
};

template<typename T>
class ReliableBroadcastNode :
    public dssim::Accepts<ReliableBroadcastMessage<T>, 1>,
    public dssim::Accepts<ReliableBroadcastTimeout<T>, 1> {
 public:
  explicit ReliableBroadcastNode(int nodes_count) : nodes_count_(nodes_count) {}
  void reliableBroadcastMessage(const T& message) {
    const int message_id = message.getID();
    if (status_[message_id] == IDLE) {
      status_[message_id] = SOURCE;
    }
    acks_[message_id].insert(this->getID());
    ReliableBroadcastMessage<T> broadcast(message);
    broadcast.acks.insert(acks_[message_id].begin(), acks_[message_id].end());
    this->broadcastMessage(broadcast);
    this->startTimer(ReliableBroadcastTimeout<T>(message), 10);
  }
  void onMessage(ReliableBroadcastMessage<T> broadcast) override {
    const int message_id = broadcast.message.getID();
    if (status_[message_id] != COMPLETED) {
      acks_[message_id].insert(broadcast.acks.begin(), broadcast.acks.end());
      if (status_[message_id] == IDLE) {
        status_[message_id] = REPEATER;
        reliableBroadcastMessage(broadcast.message);
      } else if (nodes_count_ <= acks_[message_id].size()) {
        if (status_[message_id] == SOURCE) {
          this->sendSelfMessage(ReliableBroadcastSuccess(broadcast.message));
        }
        status_[message_id] = COMPLETED;
        acks_[message_id].clear();
      }
    }
  }
  void onMessage(ReliableBroadcastTimeout<T> broadcast) override {
    const int message_id = broadcast.message.getID();
    if (status_[message_id] != COMPLETED) {
      reliableBroadcastMessage(broadcast.message);
    }
  }
  const int nodes_count_;
  enum Status { IDLE = 0, SOURCE, REPEATER, COMPLETED };
  std::unordered_map<int, Status> status_;
  std::unordered_map<int, std::unordered_set<int>> acks_{};
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_UTILS_RELIABLE_BROADCAST_H_
