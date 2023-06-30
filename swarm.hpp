#ifndef BOIDS_HPP
#define BOIDS_HPP
#include "point.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct Boid {
  Point r{}; //position
  Point v{}; //velocity
  //Boid(Point, Point);
};

class Swarm {
  std::vector<Boid> m_boids;
  sf::VertexArray m_vertices;


 public:

  // swarm constructor. Parameter gets assignet to m_boid. also:
  // initializes a vector of vertex arrays. They each store the vertexes of one
  // boid's triangle. the size is three times that of the number of boids
  // because each boid is associated with a triangle (three vertexes)
  Swarm(const std::vector<Boid>&);


  Point separation(std::vector<Boid>::iterator);
  Point alignment(std::vector<Boid>::iterator);
  Point cohesion(std::vector<Boid>::iterator);
  Point turn_around(std::vector<Boid>::iterator);

  
  // updates vertex position. They form an equilateral triangle,
  // boid is positioned in the middle of the base.
  // x and y parameters represent boid position
  void vertex_update(std::vector<Boid>::iterator);
  void update(double);

  sf::VertexArray get_vertices();
};

#endif