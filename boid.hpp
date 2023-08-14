#ifndef SWARM_HPP
#define SWARM_HPP

#include <cmath>
#include <iterator>
#include <vector>

#include "constants.hpp"
#include "point.hpp"

namespace boids {
class Boid {
 private:
  Point m_pos{};  // position
  Point m_vel{};  // velocity

  Point separation(const std::vector<Boid*>& in_range) {
    Point added_velocity{0., 0.};
    for (auto other_boid : in_range) {
      if ((m_pos - other_boid->pos()).distance() <
          constants::separation_distance) {
        added_velocity = added_velocity + (m_pos - other_boid->pos());
      }
    }
    return (constants::separation_coefficent)*added_velocity;
  }

  Point cohesion(const std::vector<Boid*>& in_range) {
    Point added_velocity{0., 0.};
    for (auto other_boid : in_range) {
      added_velocity = added_velocity + (other_boid->pos());
    }
    if (in_range.size() != 0) {
      added_velocity = constants::cohesion_coefficent *
                       ((1. / (in_range.size())) * added_velocity - (m_pos));
    }
    return added_velocity;
  }

  Point alignment(const std::vector<Boid*>& in_range) {
    Point added_velocity{0., 0.};
    for (auto other_boid : in_range) {
      added_velocity = added_velocity + (other_boid->vel());
    }
    if (in_range.size() != 0) {
      added_velocity = constants::alignment_coefficent *
                       ((1. / (in_range.size())) * added_velocity - (m_vel));
    }
    return added_velocity;
  }

  // makes the boid turn around if too close to the edge of the window.
  // the upper left edge of the window has poistion (0,0).
  // also the y axis is upside down.

  Point turn_around() {
    if (m_pos.x() > constants::window_width - constants::margin_width)
      return {-constants::turn_coefficent, 0.};
    if (m_pos.x() < constants::margin_width)
      return {constants::turn_coefficent, 0.};
    if (m_pos.y() > constants::window_height - constants::margin_width)
      return {0., -constants::turn_coefficent};
    if (m_pos.y() < constants::margin_width)
      return {0., constants::turn_coefficent};
    return {0., 0.};
  }
  
 public:
  Boid(Point& pos, Point& vel) : m_pos{pos}, m_vel{vel} {};

  Point pos() const { return m_pos; }

  Point vel() const { return m_vel; }


  void repel(const Point& click_position) {
    // normalized vector connecting point and boid
    auto add_vel =
        (1 / (m_pos - click_position).distance()) * (m_pos - click_position);
    m_vel = m_vel + constants::repel_coefficent * add_vel;
  }

  void update(double delta_t, const std::vector<Boid*>& in_range) {
    if (m_vel.distance() < constants::max_velocity) {
      m_vel = m_vel + separation(in_range) + cohesion(in_range) +
              alignment(in_range) + turn_around();
    } else {
      // is using an unidentified object point bad practice?
      m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                    constants::velocity_reduction_coefficent * (m_vel.y())};
    }
    m_pos = delta_t * (m_vel) + (m_pos);
  }
};

}  // namespace boids
#endif