#ifndef SWARM_HPP
#define SWARM_HPP

#include <vector>

#include "constants.hpp"
#include "point.hpp"

namespace boids {
class Boid {
 protected:
  Point m_pos{};  // position
  Point m_vel{};  // velocity

  Point separation(const std::vector<Boid*>& in_range, double, double);
  Point cohesion(const std::vector<Boid*>& in_range, double);
  Point alignment(const std::vector<Boid*>& in_range, double);
  Point turn_around();

 public:
  Boid(Point& pos, Point& vel);

  Point pos() const;
  Point vel() const;

  void repel(const Point& click_position);

  virtual void update(double delta_t, const std::vector<Boid*>& in_range, double, double, double, double);
};

/*class Predator : public Boid {
 public:
  using Boid::Boid;

  void update(double delta_t, const std::vector<Boid*>& in_range) override;
};*/

}  // namespace boids
#endif