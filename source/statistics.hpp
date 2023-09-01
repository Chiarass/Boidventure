#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <cmath>
#include <random>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {
    
//write some comments
double calculate_mean_distance(const std::vector<Boid>& boid_vector);
double calculate_mean_speed(const std::vector<Boid>& boid_vector);

double calculate_standard_deviation(const std::vector<double> &, double);
}  // namespace boids

#endif  // STATISTICS_HPP