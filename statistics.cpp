#include "statistics.hpp"

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids{

// random generator of values from uniform distribution, for randomly
// generating position of boids.
double uniform(double a, double b) {
  //commented out for debugging
  //std::random_device rd;
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

/*   double app_dist{0.};

  for(int i = 0; i != sample_number; ++i){
    //flattening to one dimension
  } */
 return 0.;
}
}  // namespace boids
