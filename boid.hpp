#ifndef SWARM_HPP
#define SWARM_HPP

#include <vector>

#include "constants.hpp"
#include "point.hpp"


namespace boids {
class Bird {
 protected:
  Point m_pos{};  // position
  Point m_vel{};  // velocity
  Point turn_around();

 public:
  Bird(Point& pos, Point& vel);

  Point pos() const;
  Point vel() const;

  void repel(const Point& click_position);
};

class Boid;
class Predator : public Bird {
 public:
  using Bird::Bird;
  void update_predator(double delta_t, const std::vector<Boid>&);
};
class Boid : public Bird {
  protected:
  Point separation(const std::vector<Boid*>& in_range, double, double);
  Point cohesion(const std::vector<Boid*>& in_range, double);
  Point alignment(const std::vector<Boid*>& in_range, double);

  public:
  using Bird::Bird;
  void update_boid(double, const std::vector<Boid*>&, double, double, double, double);
  void escape_predator(const Predator&, double, double);
};


}  // namespace boids
#endif