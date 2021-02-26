//
// Created by federico on 25/02/2021.
//

#include <unordered_set>
#include <set>
#include "../include/dssim.h"

struct RequestsTimeout : public dssim::Timer {};

struct WorkerReady : public dssim::Message {};

struct SchedulerReady : public dssim::Message {};

struct JobRequest : public dssim::Message {
  explicit JobRequest(int id) : job_id(id) {}
  int job_id = -1;
};

struct JobCompleted : public dssim::Message {
  explicit JobCompleted(int id) : job_id(id) {}
  int job_id = -1;
};

class Worker : public dssim::Accepts<JobRequest, 100> {
 public:
  void onStart() override {
    broadcastMessage(WorkerReady());
  }
  void onEvent(JobRequest message) override {
    sendMessage(JobCompleted(message.job_id), message.getSender());
  }
};

class Scheduler : public dssim::Accepts<WorkerReady, 1>,
                  dssim::Accepts<JobRequest, 1>,
                  dssim::Accepts<JobCompleted, 1> {
 public:
  void onStart() override {
    broadcastMessage(SchedulerReady());
  }
  void onEvent(WorkerReady message) override {
    free_workers.insert(message.getSender());
  }
  void onEvent(JobRequest message) override {
    int client = message.getSender();
    if (!free_workers.empty()) {
      int worker = *free_workers.begin();
      free_workers.erase(worker);
      busy_workers.insert(worker);
      workers_client_map[worker] = client;
      sendMessage(JobRequest(message.job_id), worker);
    }
  }
  void onEvent(JobCompleted message) override {
    free_workers.insert(message.getSender());
    int client = workers_client_map[message.getSender()];
    sendMessage(JobCompleted(message.job_id), client);
  }
 private:
  std::unordered_set<int> free_workers;
  std::unordered_set<int> busy_workers;
  std::unordered_map<int, int> workers_client_map;
};

class Client : public dssim::Accepts<SchedulerReady, 1>,
               dssim::AcceptsTrace<JobCompleted, 1>,
               dssim::AcceptsTrace<RequestsTimeout, 1000> {
 public:
  void onStart() override {}
  void onEvent(SchedulerReady message) override {
    scheduler_id = message.getSender();
    for (int i = 0; i < jobs_count; i++) {
      sendMessage(JobRequest(i), scheduler_id);
    }
    startTimer(RequestsTimeout());
  }
  void onEvent(JobCompleted message) override {
    completed_jobs.insert(message.job_id);
    std::cout << "Job " << message.job_id << " completed" <<std::endl;
  }
  void onEvent(RequestsTimeout timer) override {
    for (int i = 0; i < jobs_count; i++) {
      if (!completed_jobs.count(i)) {
        std::cout << "Job " << i << " was dropped, retrying..." << std::endl;
        sendMessage(JobRequest(i), scheduler_id);
      }
    }
    if (max_retries > 0 and completed_jobs.size() < jobs_count) {
      max_retries--;
      startTimer(RequestsTimeout());
    }
  }
 private:
  int max_retries = 5;
  int scheduler_id;
  const int jobs_count = 10;
  std::set<int> completed_jobs;
};

int main() {
  dssim::Network network;

  network.addNode(std::make_unique<Worker>());
  network.addNode(std::make_unique<Worker>());
  network.addNode(std::make_unique<Worker>());
  network.addNode(std::make_unique<Scheduler>());
  network.addNode(std::make_unique<Client>());

  network.start();
}
