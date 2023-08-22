#include "statistics.hpp"

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids{
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

double approx_distance(const std::vector<boids::Boid> &boid_vector,
                       double sample_number) {
  return 0.;
}
}  // namespace boids
