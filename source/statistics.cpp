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
  //todo: handle division by zero   
  //todo: add tests
  double sum_squared_differences = std::accumulate(data.begin(), data.end(), 0., [=](double sum, double value){
      double difference = value - average;
      sum += difference * difference;
      return sum;
  });

  double variance = sum_squared_differences / data.size();
  return std::sqrt(variance);
}

double calculate_mean_distance(const std::vector<Boid>& boid_vector) {
  
  std::vector<double> distances;
  //nested for loop, makes program lag
  //todo: delete, or find a way to approximate
  /*
  */
  std::for_each(boid_vector.begin(), boid_vector.end(), [&](const Boid& boid1) {
    std::transform(boid_vector.begin(), boid_vector.end(), std::back_inserter(distances), [&](const Boid& boid2) {
      return (boid1.pos() - boid2.pos()).distance();
    });
  });

  double mean_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
  return mean_distance;
}

double calculate_mean_speed(const std::vector<Boid>& boid_vector) {
  std::vector<double> speeds;
  std::transform(boid_vector.begin(), boid_vector.end(), std::back_inserter(speeds), [](const Boid& boid) {
    return boid.vel().distance();
  });

  double mean_speed = std::accumulate(speeds.begin(), speeds.end(), 0.0) / speeds.size();
  return mean_speed;
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
