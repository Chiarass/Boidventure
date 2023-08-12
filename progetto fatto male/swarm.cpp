#include "swarm.hpp"

#include <cmath>

// point constructor
point::point(float x, float y) : m_x{x}, m_y{y} {};

// functions to access x and y coordinates
float point::x() const { return m_x; }

float point::y() const { return m_y; }

// distance of point from origin
float point::distance() const { return sqrt(m_x * m_x + m_y * m_y); }

// definies point (that is mathematical 2-vector) operations
point operator+(point& a, point& b) {
  point sum(a.x() + b.x(), a.y() + b.y());
  return sum;
}

point operator-(point& a, point& b) {
  point diff(a.x() - b.x(), a.y() - b.y());
  return diff;
}

point operator*(float c, point& a) {  // float by reference?
  point res(c * a.x(), c * a.y());
  return res;
}

// boid constructor
Boid::Boid(point a, point b) : r{a}, v{b} {};

// swarm constructor
Swarm::Swarm(std::vector<Boid> boids) : m_boids{boids} {};
Swarm::cohesion(int n, std::vector<Boid>) {
  float sum_x = 0;
  float sum_y = 0;
  for (int j = 0; j <= n; ++j) {
    for (int i = 0; i <= n; ++i) {
      if (j != i) {
        sum_x +=
      }
    }
  }
  float x_cm = 1 / (n - 1)
}
/*
//here go the updating functions
void Swarm::Evolve(float time, float d, float d_s, float s, float a){
    for(int i = 0; i < m_boids.size(); ++i){
        for(int j = 0; j < m_boids.size()-1; ++j){
                if((m_boids[i].r - m_boids[j].r).distance() < d){

                }
        }
    }
}; */