#include <random>
#include "../dssim.h"

class Poisson_CrashOrAlive : public dssim::NodesBehaviour {
 public:
  Poisson_CrashOrAlive(const double mean_crash_time, const double mean_recovery_time)
      : mean_crash_time_(mean_crash_time), mean_recovery_time_(mean_recovery_time) {}
 protected:
  dssim::NodeStatus getNextStatus(const dssim::NodeStatus& current_status, int id) override {
    if (current_status.status == dssim::NodeStatus::Alive) {
      return {dssim::NodeStatus::Crashed, current_status.time + std::poisson_distribution(mean_crash_time_)(gen_)};
    } else {
      return {dssim::NodeStatus::Alive, current_status.time + std::poisson_distribution(mean_recovery_time_)(gen_)};
    }
  }
 private:
  std::default_random_engine gen_{RANDOM_SEED};
  const double mean_crash_time_;
  const double mean_recovery_time_;
};

class Poisson_CrashOrAlive_ClientAlwaysAlive : public Poisson_CrashOrAlive {
 public:
  Poisson_CrashOrAlive_ClientAlwaysAlive(const double mean_crash_time, const double mean_recovery_time, bool client_id)
      : Poisson_CrashOrAlive(mean_crash_time, mean_recovery_time), client_id_(client_id) {}
 protected:
    dssim::NodeStatus getNextStatus(const dssim::NodeStatus& current_status, int id) override {
    if (id == client_id_) {
      return {dssim::NodeStatus::Crashed, std::numeric_limits<double>::infinity()};
    } else {
      return Poisson_CrashOrAlive::getNextStatus(current_status, id);
    }
  }
 private:
  const int client_id_;
};

struct Query : public dssim::Message {
  Query(int goal, int client) : goal(goal), client(client) {}
  int goal;
  int client;
};

struct Success : public dssim::Message {};

struct QueryTimer : public dssim::Message {};

struct FinalTimer : public dssim::Message {};

template<int num_queries_, int dht_size_>
class Client : public dssim::Accepts<QueryTimer, dht_size_ + 2>,
               public dssim::Accepts<FinalTimer, 1>,
               public dssim::Accepts<Success, 1> {
 public:
  explicit Client(const int dht_coordinator) : dht_coordinator_(dht_coordinator) {}
  void onStart() override {
    startTimer(QueryTimer(), 1);
  }
  void onMessage(QueryTimer message) override {
    sendMessage(Query(std::uniform_int_distribution(dht_coordinator_, dht_coordinator_ + dht_size_ - 1)(gen_), getID()),
                dht_coordinator_);
    if (queries_++ < num_queries_) {
      startTimer(message, 1);
    } else {
      startTimer(FinalTimer(), 1);
    }
  }
  void onMessage(FinalTimer message) override {
    std::cout << "Success rate: " << success_count_ << " / " << num_queries_ << " ("
              << 100.0 * success_count_ / num_queries_ << "%)" << std::endl;
  }
  void onMessage(Success message) override {
    success_count_++;
  }
 private:
  int success_count_ = 0;
  int queries_ = 0;
  const int dht_coordinator_;
  std::default_random_engine gen_{RANDOM_SEED};
};

class DHTWorker : public dssim::Accepts<Query, 1> {
 public:
  DHTWorker() = default;
  void onStart() override {}
  void onMessage(Query message) override {
    if (message.goal == getID()) {
      sendMessage(Success(), message.client);
    } else {
      sendMessage(message, getID() + 1);
    }
  }
};

int main() {
  constexpr int dht_size = 10;
  constexpr int num_queries = 100000;
  for (int test = 0; test < 20; test++) {
    double mean_crash_time = 10.0 * (test + 1);
    std::cout << "Mean time between crashes: " << mean_crash_time << " ms" << std::endl;
    dssim::Network network;
    // Add nodes to the network
    auto nodes = network.emplaceMultipleNodes<DHTWorker>(dht_size);
    nodes.push_back(network.emplaceNode<Client<num_queries, dht_size>>(nodes[0]));
    // Set nodes behaviour
    network.setNodesBehaviour(std::make_unique<Poisson_CrashOrAlive_ClientAlwaysAlive>(mean_crash_time, 10.0, nodes.back()));
    network.start();
    std::cout << std::endl;
  }
}
