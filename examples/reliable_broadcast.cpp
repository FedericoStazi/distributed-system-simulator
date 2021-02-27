//
// Created by federico on 25/02/2021.
//

#include <unordered_set>
#include <set>
#include <random>
#include <utility>
#include "../dssim.h"
#include "example_behaviours.h"
#include "../utils/reliable_broadcast.h"

struct TestMessage : dssim::MessageUID {};

class Node : public dssim::utils::ReliableBroadcastNode<TestMessage>,
             public dssim::AcceptsTrace<dssim::utils::ReliableBroadcastSuccess<
                 TestMessage>, 1> {
 public:
  explicit Node(int nodes_count, bool is_sender) : is_sender_(is_sender),
                                                   dssim::utils::ReliableBroadcastNode<
                                                       TestMessage>(nodes_count) {}
  void onStart() override {
    if (is_sender_) {
      reliableBroadcastMessage(TestMessage());
      reliableBroadcastMessage(TestMessage());
    }
  }
  void onMessage(dssim::utils::ReliableBroadcastSuccess<TestMessage> message) override {
    if (repeat_count_ > 0) {
      reliableBroadcastMessage(TestMessage());
      repeat_count_--;
    }
  }
 private:
  bool is_sender_ = false;
  int repeat_count_ = 10;
};

int main() {
  dssim::Network network;

  // Add nodes to the network
  int nodes_count = 100;
  std::vector<int> nodes;
  nodes.push_back(network.addNode(std::make_unique<Node>(nodes_count, true)));
  for (int i = 0; i < nodes_count - 1; i++) {
    nodes.push_back(network.addNode(std::make_unique<Node>(nodes_count,
                                                           false)));
  }

  // Set network behaviour
  auto network_behaviour =
      std::make_unique<LinearLoss_LinearLatency_Location>(0.8, 10.0);
  for (int node : nodes) {
    network_behaviour->addRandomLocation(node);
  }
  network.setNetworkBehaviour(std::move(network_behaviour));

  network.start();
}
