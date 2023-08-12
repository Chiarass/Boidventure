#ifndef BOIDS_HPP
#define BOIDS_HPP
#include <cmath>
#include <vector>

class point {
  float m_x{};
  float m_y{};

 public:
  point(float = 0, float = 0);

  float x() const;
  float y() const;

  float distance() const;
};

point operator+(point &, point &);
point operator-(point &, point &);
point operator*(float, point &);  // should float be passed by refence?

struct Boid {
  point r;  // position
  point v;  // velocity
  Boid(point, point);
};

class Swarm {
  std::vector<Boid> m_boids;

 public:
  Swarm(std::vector<Boid>);
  void cohesion(int, std::vector<Boid>);
  /* void Evolve(float, float, float, float, float); */
};

#endif