#ifndef COESION_HPP
#define COESION_HPP
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

class Swarm {
 private:
  double d_k;
  double ds_k;
  double a_k;
  double s_k;
  double c_k;

 public:
  Swarm(double, double, double, double, double, double);
  double masscenter(int n);
};

class Boid {
  double x{};
  double y{};
  double v_x{};
  double v_y{};

 public:
  std::vector<Boid> create();
  double sumx(int n);
};

class Points {
 private:
  double x{};
  double y{};
  double v_x{};
  double v_y{};

 public:
  double operator+(std::vector<double>) {
    return s = std::accumulate(v.begin(), v.end(), 0);
  };
};

#endif