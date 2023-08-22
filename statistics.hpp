#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <cmath>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {

double calculate_standard_deviation(const std::vector<double> &, double);

// function approximates average distance, using method from Barhum, Goldreich,
// and Shraibman paper (section 1.3).
double approx_distance(const std::vector<boids::Boid> &, double);
}  // namespace boids

#endif  // STATISTICS_HPP