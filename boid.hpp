#ifndef SWARM_HPP
#define SWARM_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iterator>
#include <vector>

#include "constants.hpp"
#include "point.hpp"

namespace boids {
class Boid {
  // public or private?
 private:
  Point m_pos{};  // position
  Point m_vel{};  // velocity

  // contains vertices of boid triangles
  // sf::VertexArray m_vertices;
  // the size of vertexArray is three times that of the number of boids
  // because each boid is associated with a triangle (three vertices)
 public:

  Boid(Point& pos, Point& vel) : m_pos{pos}, m_vel{vel} {};

  /*     {
      sf::VertexArray swarm_vertex{sf::Triangles, 3 * in_range.size()};
      m_vertices = swarm_vertex;  // can't brace initialize, don't know why.

      for (int i = 0; i < static_cast<int>(in_range.size()); ++i) {
        // maybe make a constant for the color?
        m_vertices[i * 3].color = constants::boid_color;
        m_vertices[i * 3 + 1].color = constants::boid_color;
        m_vertices[i * 3 + 2].color = constants::boid_color;
      } */

  Point pos() const{ return m_pos; }

  Point vel() const{ return m_vel; }

  Point separation(const std::vector<Boid*>& in_range) {
    Point added_velocity{0, 0};
    for (auto& other_boid : in_range) {
      if ((m_pos - other_boid->pos()).distance() < constants::separation_distance) {
        added_velocity = added_velocity + (m_pos - other_boid->pos());
      }
    }
    return (constants::separation_coefficent)*added_velocity;
  }

  Point cohesion(const std::vector<Boid*>& in_range) {
    Point added_velocity{0, 0};
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
    Point added_velocity{0, 0};
    for (auto other_boid : in_range) {
        added_velocity = added_velocity + (other_boid->vel());
      }
    if (in_range.size() == 0) {
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
  
  void update(double delta_t, const std::vector<Boid*>& in_range) {
      if (m_vel.distance() < constants::max_velocity) {
        m_vel = m_vel + separation(in_range) + cohesion(in_range) + alignment(in_range) +
                turn_around();
      } else {
        // is using an unidentified object point bad practice?
        m_vel = Point{constants::velocity_reduction_coefficent * (m_vel.x()),
                      constants::velocity_reduction_coefficent * (m_vel.y())};
      }
      m_pos = delta_t * (m_vel) + (m_pos);
}

};

} // namespace boids
#endif