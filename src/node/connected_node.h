//
// Created by federico on 25/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__CONNECTED_NODE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__CONNECTED_NODE_H_

namespace dssim {

class Message;

class ConnectedNode : public virtual Node {
 public:
  class uninitializedConnectionError : public std::exception {};
  [[nodiscard]] inline int getID() const {
    return id_;
  }
 protected:
  template<typename T>
  void sendMessage(T message, int receiver_id) const;
  template<typename T>
  void sendSelfMessage(T message) const;
  template<typename T>
  void broadcastMessage(T message) const;
  template<typename T>
  void startTimer(double time, T message) const;
 private:
  void initializeConnection(std::shared_ptr<Network> network, int id) override;
  std::shared_ptr<Network> network_;
  int id_ = -1;
};

template<typename T>
void ConnectedNode::sendMessage(T message, int receiver) const {
  static_assert(std::is_base_of<Message, T>::value,
                "sendMessage expects a subclass of Message");
  if (network_) {
    message.header_ = {getID(), receiver, false};
    network_->sendMessage(message);
  } else {
    throw uninitializedConnectionError();
  }
}

template<typename T>
void ConnectedNode::sendSelfMessage(T message) const {
  sendMessage(message, getID());
}

template<typename T>
void ConnectedNode::broadcastMessage(T message) const {
  static_assert(std::is_base_of<Message, T>::value,
                "broadcastMessage expects a subclass of Message");
  if (network_) {
    message.header_ = {getID(), 0, true};
    network_->sendMessage(message);
  } else {
    throw uninitializedConnectionError();
  }
}

template<typename T>
void ConnectedNode::startTimer(double duration, T message) const {
  static_assert(std::is_base_of<Message, T>::value,
                "startTimer expects a subclass of Message");
  if (network_) {
    message.header_ = {getID(), getID(), false};
    network_->startTimer(duration, message);
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
