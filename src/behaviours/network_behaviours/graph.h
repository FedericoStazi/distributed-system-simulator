//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_

namespace dssim::behaviours {

template<typename EdgeData>
class Graph : public virtual NetworkBehaviour {
 public:
  void addDirectedEdge(int a, int b, EdgeData edge_data) {
    edges[a][b] = edge_data;
  }

  void addBidirectionalEdge(int a, int b, EdgeData edge_data) {
    addDirectedEdge(a, b, edge_data);
    addDirectedEdge(b, a, edge_data);
  }

  std::vector<double> getLatencies(const Message &message) override {
    if (edges.count(message.getSender())) {
      if (edges[message.getSender()].count(message.getReceiver())) {
        return getEdgeLatencies(
            edges[message.getSender()][message.getReceiver()]);
      }
    }
    return {};
  }

 protected:
  virtual std::vector<double> getEdgeLatencies(EdgeData edge_data) = 0;

 private:
  std::unordered_map<int, std::unordered_map<int, EdgeData>> edges;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_
