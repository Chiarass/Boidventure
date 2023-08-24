#include "boid.hpp"

namespace boids {

// Bird methods
Bird::Bird(Point& pos, Point& vel) : m_pos{pos}, m_vel{vel} {}

Point Bird::pos() const { return m_pos; }

Point Bird::vel() const { return m_vel; }

Point Bird::turn_around() {
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
void Bird::repel(const Point& click_position) {
  auto add_vel =
      (1. / (m_pos - click_position).distance()) * (m_pos - click_position);
  m_vel = m_vel + constants::repel_coefficent * add_vel;
}

// Boid methods
Point Boid::separation(const std::vector<Boid*>& in_range,
                       double separation_distance, double separation_coeff) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    if ((m_pos - other_boid->pos()).distance() < separation_distance) {
      added_velocity = added_velocity + (m_pos - other_boid->pos());
    }
  }
  return (separation_coeff)*added_velocity;
}

Point Boid::cohesion(const std::vector<Boid*>& in_range,
                     double cohesion_coeff) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    added_velocity = added_velocity + (other_boid->pos());
  }
  if (!in_range.empty()) {
    added_velocity =
        cohesion_coeff * ((1. / in_range.size()) * added_velocity - m_pos);
  }
  return added_velocity;
}

Point Boid::alignment(const std::vector<Boid*>& in_range,
                      double alignment_coeff) {
  Point added_velocity{0., 0.};
  for (auto other_boid : in_range) {
    added_velocity = added_velocity + (other_boid->vel());
  }
  if (!in_range.empty()) {
    added_velocity =
        alignment_coeff * ((1. / in_range.size()) * added_velocity - m_vel);
  }
  return added_velocity;
}

void Boid::update_boid(double delta_t, const std::vector<Boid*>& in_range,
                       double separation_distance, double separation_coeff,
                       double cohesion_coeff, double alignment_coeff) {
  if (m_vel.distance() < constants::max_velocity) {
    m_vel = m_vel +
            separation(in_range, separation_distance, separation_coeff) +
            cohesion(in_range, cohesion_coeff) +
            alignment(in_range, alignment_coeff) + turn_around();
  } else {
    m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                  constants::velocity_reduction_coefficent * (m_vel.y())};
  }
  m_pos = delta_t * (m_vel) + (m_pos);
}

void Boid::escape_predator(const Predator& predator, double predator_range,
                           double avoidance_coeff) {
  if ((pos() - predator.pos()).distance() < predator_range) {
    m_vel = m_vel + avoidance_coeff * (pos() - predator.pos());
  }
}

// predator methods
// todo: fargli seguire i boid (come???)
void Predator::update_predator(double delta_t,
                               const std::vector<Boid>& boid_vec) {
  if (m_vel.distance() < constants::max_velocity) {
    // todo: check if boids remain const
    std::vector<Point> in_range_vector{};

    for (const auto& boid : boid_vec) {
      if ((m_pos - boid.pos()).distance() < constants::init_predator_range) {
        in_range_vector.push_back(boid.pos());
      }

      // follow closest boid
      std::sort(in_range_vector.begin(), in_range_vector.end(),
                [this](const Point& a, const Point& b) {
                  return ((m_pos - a).distance() < (m_pos - b).distance());
                });
    }

    if (in_range_vector.size() != 0)
      m_vel = m_vel +
              constants::predator_hunting_coeff * (in_range_vector[0] - m_pos);
    m_vel = m_vel + turn_around();
  } else {
    m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                  constants::velocity_reduction_coefficent * (m_vel.y())};
  }
  m_pos = delta_t * (m_vel) + (m_pos);
}
}  // namespace boids