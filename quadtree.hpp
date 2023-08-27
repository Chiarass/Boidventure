// implementation of space partitioning by using quad tree.
#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cassert>
#include <iostream>
#include <vector>

#include "point.hpp"
#include "boid.hpp"

namespace boids{
struct Rectangle {
  // position of center of rectangle
  double x{};
  double y{};
  // they represent the distance from the center to the
  // left/right and upper/lower sides respectively
  double w{};
  double h{};

  // needed to check if boid is contained in quad-tree cell
  bool contains(const Point&);
};

class Quad_tree {
  const int m_capacity{};
  Rectangle m_boundary{};
  bool m_divided = false;

 public:
  std::vector<Boid*> boids_ptr;
  // children of quad tree. Dynamically allocated.
  Quad_tree* northeast;
  Quad_tree* northwest;
  Quad_tree* southeast;
  Quad_tree* southwest;

  Quad_tree(int, const Rectangle&);

  ~Quad_tree();

  void subdivide();

  void insert(Boid*);

  bool square_collide(double, const Boid&) const;

  void query(double, const Boid&, std::vector<Boid*>&) const;

  void display(sf::RenderWindow&);

  // to add test for memory leaks.
  void delete_tree();
};
}  // namespace boids
#endif