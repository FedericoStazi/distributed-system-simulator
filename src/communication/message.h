#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__MESSAGE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__MESSAGE_H_

namespace dssim {

class Message {
 public:
  virtual ~Message() = default;
  [[nodiscard]] inline int getSender() const {
    return header_.sender_id;
  }
  [[nodiscard]] inline int getReceiver() const {
    return header_.receiver_id;
  }
  [[nodiscard]] inline bool isBroadcast() const {
    return header_.is_broadcast;
  }
  void inline setBroadcastReceiver(int receiver) {
    if (isBroadcast()) {
      header_.is_broadcast = false;
      header_.receiver_id = receiver;
    }
  }
 private:
  friend ConnectedNode;
  struct MessageHeader {
    int sender_id = 0;
    int receiver_id = 0;
    bool is_broadcast = false;
  } header_;
};

class MessageID : public dssim::Message {
 public:
  virtual int getID() const = 0;
};

class MessageUID : public dssim::MessageID {
 public:
  MessageUID() {
    id_ = next_message_id_;
    next_message_id_++;
  }
  [[nodiscard]] virtual int getID() const final {
    return id_;
  }
 private:
  static inline int next_message_id_ = 0;
  int id_ = -1;
};

}
#endif //DISTRIBUTED_SYSTEM_SIMULATOR__MESSAGE_H_
