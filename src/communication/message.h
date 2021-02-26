//
// Created by federico on 07/01/2021.
//

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
 private:
  friend ConnectedNode;
  struct MessageHeader {
    int sender_id = 0;
    int receiver_id = 0;
    bool is_broadcast = false;
  } header_;
};

}
#endif //DISTRIBUTED_SYSTEM_SIMULATOR__MESSAGE_H_
