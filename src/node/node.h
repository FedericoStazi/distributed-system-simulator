//
// Created by federico on 06/01/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR__NODE_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR__NODE_H_

#include <memory>

namespace dssim {

class Network;

class Node {
 public:
  void start(std::shared_ptr<Network> network, int id);
 private:
  virtual void onStart() = 0;
  virtual void initializeConnection(std::shared_ptr<Network>, int) = 0;
};

void Node::start(std::shared_ptr<Network> network, int id) {
  initializeConnection(std::move(network), id);
  onStart();
}

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR__NODE_H_
