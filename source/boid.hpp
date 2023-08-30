#ifndef BOID_HPP
#define BOID_HPP

#include <vector>

#include "constants.hpp"
#include "point.hpp"

namespace boids {
// parent class, containes all methods shared by boids and predators
class Bird {
 protected:
  Point m_pos{};  // position
  Point m_vel{};  // velocity

  // adds a velocity vector to the boids, if they exit the boundary.
  // the boundary is defined by the window, margin and control panel
  // constants in constants.hpp. the size of the vector is determined
  // by constants::turn_coefficent
  Point turn_around();

 public:
  Bird(const Point& pos = Point{}, const Point& vel = Point{});

  // returns m_pos
  Point pos() const;
  // returns m_vel
  Point vel() const;

  // adds a velocity vector to the boids, pointing radially outward from a
  // specified point, if in range.
  //Param 1: the point.
  //Param 2: the range.
  //Param 3: a coefficent of the force.
  // the size of the vector is determined by constants::repel_coefficent
  void repel(const Point&, double, double);
};

class Boid;
class Predator : public Bird {
 public:
  using Bird::Bird;

  // updates the position of a predator object. a velocity vector is added to
  // the object's speed, pointing towards the nearest boid within the provided
  // boid vector and the specified range. if predator exceeds
  // constants::max_velocity it slows down the predator.
  // Param 1: delta_t, time step in the equation of motion, affecting the speed
  // of the object.
  // Param 2: the range of the predator's vision.
  // Param 3: vector of boids.
  void update(double, double, const std::vector<Boid>&);
};

class Boid : public Bird {
 protected:
  // implements separation force on boid.
  // Param 1: vector containing the boids in a range > than the separation
  // range (itself excluded).
  // Param 2: the separation range
  // Param 3: a coefficent that determines the strength of the force
  Point separation(const std::vector<const Boid*>&, double, double);

  // implements cohesion force on boid.
  // Param 1: vector containing the boids in the cohesion range (itself
  // excluded).
  // Param 2: a coefficent that determines the strength of the force;
  Point cohesion(const std::vector<const Boid*>&, double);

  // implements alignment force on boid. see https://www.red3d.com/cwr/boids/
  // for more
  // Param 1: vector containing the boids in the alignment range (itself
  // excluded).
  // Param 2: a coefficent that determines the strength of the force;
  Point alignment(const std::vector<const Boid*>&, double);

 public:
  using Bird::Bird;

  // updates position of boid. to do this it calls separation, cohesion,
  // alignment and turn_around methods. if boid exceeds constants::max_velocity
  // it slows down the boid.
  // Param 1: delta_t, time step in the equation of motion, affecting the speed
  // of the object.
  // Param 2: vector containing the boids in the alignment/cohesion range
  // (itself excluded).
  // Param 3: range to pass as parameter 2 of separation
  // Param 4: a coefficent to pass as parameter 3 of separation
  // Param 5: ge to pass as parameter 2 of cohesion
  // Param 6: ge to pass as parameter 2 of alignment
  void update(double, const std::vector<const Boid*>&, double, double,
                   double, double);
};

}  // namespace boids
#endif