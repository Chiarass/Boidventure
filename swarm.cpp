#include "swarm.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

//--constants----
// there should be a way to put all constants in one place,
// learncpp talked about it (something about)
// global variables.
constexpr float boid_size =
    30;  // the size of the sfml object representing the boid

// Point constructor
Point::Point(double x, double y) : m_x{x}, m_y{y} {};

// functions to access x and y coordinates
double Point::x() const { return m_x; }

double Point::y() const { return m_y; }

// distance of Point from origin
double Point::distance() const { return sqrt(m_x * m_x + m_y * m_y); }

// defines Point (that is mathematical 2-vector) operations
Point operator+(const Point& a, const Point& b) {
  Point sum(
      a.x() + b.x(),
      a.y() +
          b.y());  // maybe use brace initialization when calling constructor?
  return sum;
}

Point operator-(const Point& a, const Point& b) {
  Point diff(
      a.x() - b.x(),
      a.y() -
          b.y());  // maybe use brace initialization when calling constructor?
  return diff;
}

Point operator*(double c, const Point& a) {
  Point res(
      c * a.x(),
      c * a.y());  // maybe use brace initialization when calling constructor?
  return res;
}

// boid constructor
Boid::Boid(Point a, Point b) : r{a}, v{b} {};


// swarm constructor.parameter gets assignet to m_boid. also:
// initializes a vector of vertex arrays. They each store the vertexes of one
// boid's triangle. the size is three times that of the number of boids
// because each boid is associated with a triangle (three vertexes)

Swarm::Swarm(std::vector<Boid> boids) : m_boids{boids} {
  sf::VertexArray swarm_vertex(sf::Triangles, 3 * m_boids.size());
  m_vertices = swarm_vertex;  // can't brace initialize, don't know why.

  //static_cast only serves the purpose of removing warning
  //because m_boids.size() is not an int  
  
  for (int i = 0; i < static_cast<int>(m_boids.size()); ++i) {
    // colors of triangles. Creates cool gradient. Probably temporary.
    m_vertices[i * 3].color = sf::Color::Red;
    m_vertices[i * 3 + 1].color = sf::Color::Blue;
    m_vertices[i * 3 + 2].color = sf::Color::Green;
  }
};

void Swarm::update(double delta_t) {
  for (auto it = m_boids.begin(); it < m_boids.end(); it++) {
    // for loop, change to algorithm?
    (*it).r = delta_t * (*it).v + (*it).r;
  }
  vertex_update();
}

// updates vertex position. They form an equilateral triangle,
// boid is positioned in the middle of the base.
// x and y parameters represent boid position

void Swarm::vertex_update() {

  // considering not using iterators, to make
  // code more readable. Also, better to change
  // to an alogtirithm if possible.

  for (auto it = m_boids.begin(); it < m_boids.end(); it++) {
    auto index = (it - m_boids.begin());
    auto x = (*it).r.x();
    auto y = (*it).r.y();

    m_vertices[3 * index].position =
        sf::Vector2f(x + static_cast<float>(boid_size), y);

    m_vertices[3 * index + 1].position =
        sf::Vector2f(x - static_cast<float>(boid_size), y);

    m_vertices[3 * index + 2].position =
        sf::Vector2f(x, y + static_cast<float>(sqrt(3) * boid_size));
  }
}

sf::VertexArray Swarm::get_vertices(){
    return m_vertices;
}