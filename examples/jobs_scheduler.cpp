//
// Created by federico on 25/02/2021.
//

#include <unordered_set>
#include <set>
#include <random>
#include "../dssim.h"
#include "example_behaviours.h"

struct RequestsTimeout : public dssim::Message {};

struct WorkerReady : public dssim::Message {};

struct SchedulerReady : public dssim::Message {};

class JobRequest : public dssim::MessageID {
 public:
  explicit JobRequest(int id) : job_id(id) {}
  [[nodiscard]] int getID() const override {
    return job_id;
  }
 private:
  int job_id = -1;
};

class JobCompleted : public dssim::MessageID {
 public:
  explicit JobCompleted(int id) : job_id(id) {}
  [[nodiscard]] int getID() const override {
    return job_id;
  }
 private:
  int job_id = -1;
};

class Worker : public dssim::Accepts<JobRequest, 100> {
 public:
  void onStart() override {
    broadcastMessage(WorkerReady());
  }
  void onMessage(JobRequest message) override {
    sendMessage(JobCompleted(message.getID()), message.getSender());
  }
};

class Scheduler : public dssim::Accepts<WorkerReady, 1>,
                  public dssim::Accepts<JobRequest, 1>,
                  public dssim::Accepts<JobCompleted, 1> {
 public:
  void onStart() override {
    broadcastMessage(SchedulerReady());
  }
  void onMessage(WorkerReady message) override {
    free_workers.insert(message.getSender());
  }
  void onMessage(JobRequest message) override {
    int client = message.getSender();
    if (!free_workers.empty()) {
      int worker = *free_workers.begin();
      free_workers.erase(worker);
      busy_workers.insert(worker);
      workers_client_map[worker] = client;
      sendMessage(JobRequest(message.getID()), worker);
    }
  }
  void onMessage(JobCompleted message) override {
    free_workers.insert(message.getSender());
    int client = workers_client_map[message.getSender()];
    sendMessage(JobCompleted(message.getID()), client);
  }
 private:
  std::unordered_set<int> free_workers;
  std::unordered_set<int> busy_workers;
  std::unordered_map<int, int> workers_client_map;
};

class Client : public dssim::Accepts<SchedulerReady, 1>,
               public dssim::AcceptsTrace<JobCompleted, 1>,
               public dssim::AcceptsTrace<RequestsTimeout, 1> {
 public:
  void onStart() override {}
  void onMessage(SchedulerReady message) override {
    scheduler_id = message.getSender();
    for (int i = 0; i < jobs_count; i++) {
      sendMessage(JobRequest(i), scheduler_id);
    }
    startTimer(1, RequestsTimeout());
  }
  void onMessage(JobCompleted message) override {
    completed_jobs.insert(message.getID());
    std::cout << "Job " << message.getID() << " completed" << std::endl;
  }
  void onMessage(RequestsTimeout timer) override {
    for (int i = 0; i < jobs_count; i++) {
      if (!completed_jobs.count(i)) {
        std::cout << "Job " << i << " was dropped, retrying..." << std::endl;
        sendMessage(JobRequest(i), scheduler_id);
      }
    }
    if (max_retries > 0 and completed_jobs.size() < jobs_count) {
      max_retries--;
      startTimer(1000, RequestsTimeout());
    }
  }
 private:
  int max_retries = 5;
  int scheduler_id = 0;
  const int jobs_count = 10;
  std::set<int> completed_jobs;
};

int main() {
  dssim::Network network;

  // Add nodes to the network
  auto workers = {
      network.addNode(std::make_unique<Worker>()),
      network.addNode(std::make_unique<Worker>()),
      network.addNode(std::make_unique<Worker>()),
      network.addNode(std::make_unique<Worker>()),
      network.addNode(std::make_unique<Worker>()),
      network.addNode(std::make_unique<Worker>())
  };
  int scheduler = network.addNode(std::make_unique<Scheduler>());
  auto clients = {
      network.addNode(std::make_unique<Client>())
  };

  // Set network behaviour
  auto network_behaviour =
      std::make_unique<BernoulliLoss_ExponentialLatency_Graph>();
  for (int worker : workers) {
    network_behaviour->addBidirectionalEdge(worker, scheduler, {0.2, 0.1});
  }
  for (int client : clients) {
    network_behaviour->addBidirectionalEdge(client, scheduler, {0, 0.1});
  }
  network.setNetworkBehaviour(std::move(network_behaviour));

  network.start();
}
