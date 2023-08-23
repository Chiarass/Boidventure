#include "statistics.hpp"

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {
//todo: change implementation of engine
// random generator of values from uniform distribution, for randomly
// generating position of boids.
double uniform(double a, double b) {
  // commented out for debugging
  // std::random_device rd;
  std::uniform_real_distribution<double> unif{a, b};
  return unif(eng);
}

double calculate_standard_deviation(const std::vector<double> &data,
                                    double average) {
  double sum_squared_differences = 0.0;
  for (double value : data) {
    double difference = value - average;
    sum_squared_differences += difference * difference;
  }
  double variance = sum_squared_differences / data.size();
  return std::sqrt(variance);
}


double approx_distance(const std::vector<boids::Boid> &boid_vector, int sample_size){
  //todo: assert sample size is > 0
  std::uniform_int_distribution<> unif_int{0, static_cast<int>(boid_vector.size()) - 1};
  double approx_dist{};

  for(int i = 0; i < sample_size; ++i){
    int k = unif_int(eng);
    int j = unif_int(eng);
    approx_dist += (boid_vector[k].pos() - boid_vector[j].pos()).distance();
  }

  return approx_dist/sample_size; 
}
}  // namespace boids
