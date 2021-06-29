#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_

namespace dssim::behaviours {

template<typename EdgeData>
class Graph : public virtual NetworkBehaviour {
 public:
  // Add a directed edge from a to b.
  void addDirectedEdge(int a, int b, EdgeData edge_data) {
    edges[a][b] = edge_data;
  }

  // Add a bidirectional edge between a and b.
  void addBidirectionalEdge(int a, int b, EdgeData edge_data) {
    addDirectedEdge(a, b, edge_data);
    addDirectedEdge(b, a, edge_data);
  }

  int getDuplication(const Message& message) override {
    return edges.count(message.getSender()) &&
        edges[message.getSender()].count(message.getReceiver()) &&
        !getEdgeLoss(edges[message.getSender()][message.getReceiver()]);
  }

  double getLatency(const Message& message, int duplicate_index) override {
    return getEdgeLatency(edges[message.getSender()][message.getReceiver()]);
  }

 protected:
  // Get the latency of a message travelling over an edge with data edge_data.
  virtual double getEdgeLatency(EdgeData edge_data) = 0;
  // Determine if a message travelling over an edge with data edge_data is dropped.
  virtual bool getEdgeLoss(EdgeData edge_data) = 0;

 private:
  std::unordered_map<int, std::unordered_map<int, EdgeData>> edges;
};

}

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_SRC_BEHAVIOUR_ASSUMPTIONS_NETWORK_BEHAVIOURS_GRAPH_H_
