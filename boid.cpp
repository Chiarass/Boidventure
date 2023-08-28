#include "boid.hpp"

#include <algorithm>  //for for_each
#include <cassert>
#include <cmath>  //for isnan

namespace boids {

// Bird methods
Bird::Bird(const Point& pos, const Point& vel) : m_pos{pos}, m_vel{vel} {}

Point Bird::pos() const { return m_pos; }

Point Bird::vel() const { return m_vel; }

Point Bird::turn_around() {
  Point added_velocity{0., 0.};
  if (m_pos.x() > constants::window_width - constants::margin_size)
    added_velocity = added_velocity + Point{-constants::turn_coefficent, 0.};
  if (m_pos.x() < constants::margin_size + constants::controls_width)
    added_velocity = added_velocity + Point{constants::turn_coefficent, 0.};
  if (m_pos.y() > constants::window_height - constants::margin_size)
    added_velocity = added_velocity + Point{0., -constants::turn_coefficent};
  if (m_pos.y() < constants::margin_size)
    added_velocity = added_velocity + Point{0., constants::turn_coefficent};
  return added_velocity;
}

void Bird::repel(const Point& click_position) {
  // if handels division by zero (click position = bird position)
  if (m_pos.x() != click_position.x() || m_pos.y() != click_position.y()) {
    auto add_vel =
        (1. / (m_pos - click_position).distance()) * (m_pos - click_position);
    m_vel = m_vel + constants::repel_coefficent * add_vel;
  }

  assert(!std::isnan(m_vel.x()));
  assert(!std::isnan(m_vel.y()));
}

// Boid methods
Point Boid::separation(const std::vector<Boid*>& in_range,
                       double separation_distance, double separation_coeff) {
  assert(separation_distance >= 0.);
  assert(separation_coeff >= 0.);

  Point added_velocity{0., 0.};

  std::for_each(
      in_range.begin(), in_range.end(),
      [&added_velocity, this, separation_distance](Boid* other_boid_ptr) {
        if ((m_pos - other_boid_ptr->pos()).distance() < separation_distance) {
          added_velocity = added_velocity + (m_pos - other_boid_ptr->pos());
        }
      });

  return (separation_coeff)*added_velocity;
}

Point Boid::cohesion(const std::vector<Boid*>& in_range,
                     double cohesion_coeff) {
  assert(cohesion_coeff >= 0.);

  Point added_velocity{0., 0.};

  if (in_range.empty()) {
    return added_velocity;
  }

  std::for_each(in_range.begin(), in_range.end(),
                [&added_velocity](Boid* other_boid_ptr) {
                  added_velocity = added_velocity + (other_boid_ptr->pos());
                });

  added_velocity =
      cohesion_coeff * ((1. / in_range.size()) * added_velocity - m_pos);
  return added_velocity;
}

Point Boid::alignment(const std::vector<Boid*>& in_range,
                      double alignment_coeff) {
  assert(alignment_coeff >= 0.);

  Point added_velocity{0., 0.};

  if (in_range.empty()) {
    return added_velocity;
  }

  std::for_each(in_range.begin(), in_range.end(),
                [&added_velocity](Boid* other_boid_ptr) {
                  added_velocity = added_velocity + (other_boid_ptr->vel());
                });

  added_velocity =
      alignment_coeff * ((1. / in_range.size()) * added_velocity - m_vel);
  return added_velocity;
}

void Boid::update_boid(double delta_t, const std::vector<Boid*>& in_range,
                       double separation_distance, double separation_coeff,
                       double cohesion_coeff, double alignment_coeff) {
  assert(delta_t >= 0.);

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

void Boid::escape_predator(const Predator& predator, double prey_range,
                           double avoidance_coeff) {
  assert(prey_range >= 0.);
  assert(avoidance_coeff >= 0.);

  if ((pos() - predator.pos()).distance() < prey_range) {
    m_vel = m_vel + avoidance_coeff * (pos() - predator.pos());
  }
}

// predator methods

void Predator::update_predator(double delta_t, double predator_range,
                               const std::vector<Boid>& boid_vec) {
  assert(predator_range >= 0.);
  assert(delta_t >= 0.);

  if (m_vel.distance() < constants::max_velocity) {
    // todo: check if boids remain const
    std::vector<Point> in_range_vector{};

    // finding boids in range
    for (const auto& boid : boid_vec) {
      if ((m_pos - boid.pos()).distance() < predator_range) {
        in_range_vector.push_back(boid.pos());
      }
    }

    // sort to get closest boid
    std::sort(in_range_vector.begin(), in_range_vector.end(),
              [this](const Point& a, const Point& b) {
                return ((m_pos - a).distance() < (m_pos - b).distance());
              });

    // add velocity to move towards closest boid
    if (in_range_vector.size() != 0) {
      m_vel = m_vel +
              constants::predator_hunting_coeff * (in_range_vector[0] - m_pos);
    }

    m_vel = m_vel + turn_around();

  }

  else {
    m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                  constants::velocity_reduction_coefficent * (m_vel.y())};
  }
  m_pos = delta_t * (m_vel) + (m_pos);
}
}  // namespace boids