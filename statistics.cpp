#include "statistics.hpp"

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {

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

// function approximates average distance, using method from Barhum, Goldreich,
// and Shraibman paper (section 1.3).
double approx_distance(const std::vector<boids::Boid> &boid_vector,
                       int sample_number) {
  double app_dist{0.};

  // todo: sample_number must be positive and nonzero (add assert)
  for (int j = 0; j < sample_number; ++j) {
    // flattening to one dimension, on a random 2d vector
    auto angle = uniform(0., 2. * constants::pi);
    std::vector<double> flattened_positions;

    for (const auto &boid : boid_vector) {
      flattened_positions.push_back(cos(angle) * boid.pos().distance());
    }

    std::sort(flattened_positions.begin(), flattened_positions.end());

    // finding one dimensional sum of distances from first point. (sect 1.1 in
    // paper)
    double distance_from_first{
        std::accumulate(flattened_positions.begin(), flattened_positions.end(),
                        0., [&flattened_positions](double a, double b) {
                          return (a + (b - flattened_positions[0]));
                        })};
    app_dist += distance_from_first;

    // finding average distance of flattened points. (implementing formula in
    // section 1.1 of paper)
    for (int i = 0; i < static_cast<int>(flattened_positions.size()) - 1; ++i) {
      distance_from_first +=
          (2. * (i + 1) - static_cast<double>(flattened_positions.size())) *
          (flattened_positions[i + 1] - flattened_positions[i]);
      app_dist += distance_from_first;
    }
  }
  // taking mean to get expected value (eq 4. in paper) and multipling by
  // constant p(d) (see appendix in paper)
  app_dist *= (constants::pi / 2.) / sample_number;
  //dividing by number of points(boids)
  //todo: handle case in which size is 0
  return (app_dist)/(boid_vector.size()*boid_vector.size());
}
}  // namespace boids
