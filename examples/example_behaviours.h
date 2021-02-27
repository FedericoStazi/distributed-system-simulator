//
// Created by federico on 26/02/2021.
//

#ifndef DISTRIBUTED_SYSTEM_SIMULATOR_EXAMPLES_EXAMPLE_BEHAVIOURS_H_
#define DISTRIBUTED_SYSTEM_SIMULATOR_EXAMPLES_EXAMPLE_BEHAVIOURS_H_

struct LossLatencyEdge {
  double loss;
  double latency;
};

class BernoulliLoss_ExponentialLatency_Graph :
    public dssim::behaviours::NoInterference,
    public dssim::behaviours::Graph<LossLatencyEdge> {
 private:
  std::vector<double> getEdgeLatencies(LossLatencyEdge edge) override {
    if (std::bernoulli_distribution(edge.loss)(gen_)) {
      return {};
    }
    return {std::exponential_distribution(edge.latency)(gen_)};
  }
  std::default_random_engine gen_{RANDOM_SEED};
};

class LinearLoss_LinearLatency_Location :
    public dssim::behaviours::NoInterference,
    public dssim::behaviours::LocationBased {
 public:
  LinearLoss_LinearLatency_Location(double loss, double latency) :
      loss_(loss), latency_(latency) {}
 private:
  std::vector<double> getEdgeLatencies(double distance) override {
    if (std::bernoulli_distribution(loss_ * distance)(gen_)) {
      return {};
    }
    return {latency_ * distance};
  }
  std::default_random_engine gen_{RANDOM_SEED};
  double loss_;
  double latency_;
};

#endif //DISTRIBUTED_SYSTEM_SIMULATOR_EXAMPLES_EXAMPLE_BEHAVIOURS_H_
