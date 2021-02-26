//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__CONNECTED_NODE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__CONNECTED_NODE_H_

namespace dssim {

class Message;
class Timer;

class ConnectedNode : public virtual Node {
 public:
  class uninitializedConnectionError : public std::exception {};
  [[nodiscard]] inline int getID() const {
    return id_;
  }
 protected:
  template<typename MessageType>
  void sendMessage(MessageType message, int receiver_id) const;
  template<typename MessageType>
  void broadcastMessage(MessageType message) const;
  template<typename TimerType>
  void startTimer(TimerType timer) const;
 private:
  void initializeConnection(std::shared_ptr<Network> network, int id) override;
  std::shared_ptr<Network> network_;
  int id_ = -1;
};

template<typename MessageType>
void ConnectedNode::sendMessage(MessageType message, int receiver) const {
  static_assert(std::is_base_of<Message, MessageType>::value,
                "sendMessage expects a subclass of Message");
  if (network_) {
    message.header_ = {getID(), receiver, false};
    network_->sendMessage(message);
  } else {
    throw uninitializedConnectionError();
  }
}

template<typename MessageType>
void ConnectedNode::broadcastMessage(MessageType message) const {
  static_assert(std::is_base_of<Message, MessageType>::value,
                "broadcastMessage expects a subclass of Message");
  if (network_) {
    message.header_ = {getID(), 0, true};
    network_->sendMessage(message);
  } else {
    throw uninitializedConnectionError();
  }
}

template<typename TimerType>
void ConnectedNode::startTimer(TimerType timer) const {
  static_assert(std::is_base_of<Timer, TimerType>::value,
                "startTimer expects a subclass of Timer");
  if (network_) {
    timer.header_ = {getID()};
    network_->startTimer(timer);
  } else {
    throw uninitializedConnectionError();
  }
}

void ConnectedNode::initializeConnection(std::shared_ptr<Network> network,
                                         int id) {
  network_ = std::move(network);
  id_ = id;
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__CONNECTED_NODE_H_
