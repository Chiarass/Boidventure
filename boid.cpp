#include "boid.hpp"

namespace boids {

Boid::Boid(Point& pos, Point& vel) : m_pos{pos}, m_vel{vel} {}

Point Boid::pos() const { return m_pos; }

Point Boid::vel() const { return m_vel; }

Point Boid::separation(const std::vector<Boid*>& in_range) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    if ((m_pos - other_boid->pos()).distance() <
        constants::separation_distance) {
      added_velocity = added_velocity + (m_pos - other_boid->pos());
    }
  }
  return (constants::separation_coefficent)*added_velocity;
}

Point Boid::cohesion(const std::vector<Boid*>& in_range) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    added_velocity = added_velocity + (other_boid->pos());
  }
  if (!in_range.empty()) {
    added_velocity = constants::cohesion_coefficent *
                     ((1. / in_range.size()) * added_velocity - m_pos);
  }
  return added_velocity;
}

Point Boid::alignment(const std::vector<Boid*>& in_range) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    added_velocity = added_velocity + (other_boid->vel());
  }
  if (!in_range.empty()) {
    added_velocity = constants::alignment_coefficent *
                     ((1. / in_range.size()) * added_velocity - m_vel);
  }
  return added_velocity;
}

Point Boid::turn_around() {
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

void Boid::repel(const Point& click_position) {
  auto add_vel =
      (1. / (m_pos - click_position).distance()) * (m_pos - click_position);
  m_vel = m_vel + constants::repel_coefficent * add_vel;
}

void Boid::update(double delta_t, const std::vector<Boid*>& in_range) {
  if (m_vel.distance() < constants::max_velocity) {
    m_vel = m_vel + separation(in_range) + cohesion(in_range) +
            alignment(in_range) + turn_around();
  } else {
    m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                  constants::velocity_reduction_coefficent * (m_vel.y())};
  }
  m_pos = delta_t * (m_vel) + (m_pos);
}

/*void Predator::update(double delta_t, const std::vector<Boid*>& in_range) {
  if (m_vel.distance() < constants::max_velocity) {
    m_vel = m_vel + separation(in_range) + cohesion(in_range) +
            alignment(in_range) + turn_around();
  } else {
    m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                  constants::velocity_reduction_coefficent * (m_vel.y())};
  }
  // Add predator behavior: eat other boids
  in_range.erase(std::remove_if(in_range.begin(), in_range.end(),
                                 [this](Boid* boid) {
                                   return (m_pos - boid->pos()).distance() < constants::repel_range;
                                 }),
                 in_range.end());

  m_pos = delta_t * (m_vel) + (m_pos);
}
*/
}  // namespace boids