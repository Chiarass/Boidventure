#ifndef BOIDS_HPP
#define BOIDS_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Point {
 double m_x{};
 double m_y{};

 public:
 Point(double = 0, double = 0);

//returns m_x, m_y
 double x() const; //is it necessary for const to go here?
 double y() const; //is it necessary for const to go here?

 double distance() const;
};

Point operator+(const Point &, const Point &);  //should they stay inside or outside?
Point operator-(const Point &, const Point&);
Point operator*(double,const Point&);

struct Boid {
  Point r; //position
  Point v; //velocity
  Boid(Point, Point);
};

class Swarm {
  std::vector<Boid> m_boids;
  sf::VertexArray m_vertices;


 public:

  // swarm constructor.parameter gets assignet to m_boid. also:
  // initializes a vector of vertex arrays. They each store the vertexes of one
  // boid's triangle. the size is three times that of the number of boids
  // because each boid is associated with a triangle (three vertexes)

  Swarm(std::vector<Boid>);
  void update(double);
  
// updates vertex position. They form an equilateral triangle,
// boid is positioned in the middle of the base.
// x and y parameters represent boid position
  void vertex_update();

  sf::VertexArray get_vertices();
};

#endif