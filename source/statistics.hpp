#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {

// todo: change to something passed from main. (for some techincal reason it
// didn't work when i tried)
inline std::mt19937 eng{};

// random generator of values from uniform distribution, for randomly
// generating position of boids. takes two doulbes, the minimum and maximum
//of the nrange of samples it generates.
double uniform(double, double);

//write some comments
double calculate_mean_distance(const std::vector<Boid>& boid_vector);
double calculate_mean_speed(const std::vector<Boid>& boid_vector);

double calculate_standard_deviation(const std::vector<double> &, double);

// function approximates average distance, using method from Barhum, Goldreich,
// and Shraibman paper (section 1.3).
double approx_distance(const std::vector<boids::Boid> &, int);
}  // namespace boids

#endif  // STATISTICS_HPP