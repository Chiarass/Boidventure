#ifndef BOIDS_HPP
#define BOIDS_HPP
#include <vector>

struct point {
  float x{};
  float y{};
};

float operator+(point, point);

class Boid {
  point m_x;
  point m_v;

 public:
  Boid(point, point);
};

class Swarm {
  std::vector<Boid> m_boids;

 public:
  Swarm(std::vector<Boid>);
};

#endif