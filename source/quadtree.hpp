// implementation of space partitioning by using quad tree.
#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cassert>
#include <iostream>
#include <vector>
#include <memory> //for unique_ptr

#include "boid.hpp"
#include "point.hpp"

namespace boids {
struct Rectangle {
  // position of center of rectangle
  double x{};
  double y{};
  // they represent the distance from the center to the
  // left/right and upper/lower sides respectively
  double w{};
  double h{};

  // checks if point is contained in rectangle
  // needed to check if boid is contained in quad tree cell
  // Param 1: the point
  bool contains(const Point&) const;
};

class Quad_tree {
  // maximum number of boids in a cell
  // if exceeded, insert() calls subdivide()
  const int m_capacity{};

  // rectangle object representing the mother (biggest) cell
  Rectangle m_boundary{};
  bool m_divided = false;

  // initializes children cells, sets m_divided = true
  void subdivide();

  // boids in cell, gets populated by insert()
  // gets emptied if m_divided = true
  std::vector<const Boid*> m_boids_ptr;

  // children cells. Dynamically allocated.
  std::unique_ptr<Quad_tree> northeast;
  std::unique_ptr<Quad_tree> northwest;
  std::unique_ptr<Quad_tree> southeast;
  std::unique_ptr<Quad_tree> southwest;

 public:
  // Param 1: m_capacity
  // Param 2: m_boundary
  Quad_tree(int, const Rectangle&);

  // calls delete_tree to handle heap allocated children cells
  //~Quad_tree();

  // if boid is inside the cell it pushes back the boid pointer to boids_ptr
  // if m_divided = true then it gets passed to children cells
  // Param 1: the boid to insert
  void insert(const Boid&);

  // checks if the cell collides with the provided range of the provided boid
  // Param 1: the range
  // Param 2: the boid
  bool square_collide(double, const Boid&) const;

  // populates the provided vector of boid pointers with pointers to boids
  // contained within the quad tree that are within the specified range from the
  // given boid.
  // Param 1: the range
  // Param 2: the boid Param 3: the vector of boid pointers
  void query(double, const Boid&, std::vector<const Boid*>&) const;

  // displays the quad tree (children cell included) to the provided window
  // Param 1: the window
  void display(sf::RenderWindow&);
};
}  // namespace boids
#endif