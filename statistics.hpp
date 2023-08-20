#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <cmath>
#include <vector>

// Funzione per il calcolo della deviazione standard
double calculate_standard_deviation(const std::vector<double>& data, double average) {
  double sum_squared_differences = 0.0;
  for (double value : data) {
    double difference = value - average;
    sum_squared_differences += difference * difference;
  }
  double variance = sum_squared_differences / data.size();
  return std::sqrt(variance);
}

#endif // STATISTICS_HPP