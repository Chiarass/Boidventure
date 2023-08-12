#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "swarm.hpp"

//-----costants-----------------
float initial_position = 750;
float initial_velocity = 0;
float position_variance = 300;  // variance of random distribution
float velocity_variance = 10;
float d_coeff =
    100;  // maximum distance for swarm effects to kick in (Elove method)
float s_coeff = 30;       // separation coefficent
int swarm_number = 100;
float boid_size = 30;     // the size of the sfml object representing the boid
float delta_time = 0.01;  // time increment. higher number, lower speeds.

// random generator of values from gaussian distribution, for randomly
// generating position of boids
float gaussian(float u, float o) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::normal_distribution<float> offset{u, o};
  return offset(eng);
}

int main() {
  // makes the window and specifies its size and title
  sf::RenderWindow window;
  window.create(sf::VideoMode(1500, 1500), "boids!", sf::Style::Default);
  // constructing a swarm object, with each boid with random coordinates. Might
  // change name from bee to something else later on
  std::vector<Boid> bee_vector;
  point bee_position;
  point bee_velocity;

  // initializing a vector of vertex arrays. They each store the vertexes of one
  // boid's triangle.
  sf::VertexArray swarm_vertex(sf::Triangles, 3 * swarm_number);

  for (int i = 0; i < swarm_number; ++i) {
    // initalizing random boid
    bee_position = point(gaussian(initial_position, position_variance),
                         gaussian(initial_position, position_variance));
    bee_velocity = point(gaussian(initial_position, position_variance),
                         gaussian(initial_position, position_variance));
    bee_vector.push_back(Boid(bee_position, bee_velocity));

    // add vertexes of boid. They form an equilateral triangle, boid is
    // positioned in the middle of the base.
    swarm_vertex[i * 3].position =
        sf::Vector2f(bee_position.x() + boid_size, bee_position.y());
    swarm_vertex[i * 3 + 1].position =
        sf::Vector2f(bee_position.x() - boid_size, bee_position.y());
    swarm_vertex[i * 3 + 2].position =
        sf::Vector2f(bee_position.x(), bee_position.y() + sqrt(3) * boid_size);

    // colors of triangles. Creates cool gradient. Probably temporary.
    swarm_vertex[i * 3].color = sf::Color::Red;
    swarm_vertex[i * 3 + 1].color = sf::Color::Blue;
    swarm_vertex[i * 3 + 2].color = sf::Color::Green;
  }
  Swarm bee_swarm = Swarm(bee_vector);

  // SFML loop. After each loop the window is updated
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear(sf::Color::Black);  // makes the window return black

    // here goes the code that updates the boids poisitions, once it will be
    // ready

    // and here goes the code that updates the vertex positions

    // and here goes the code to align the vertex of a triangle with boid
    // direction of motion

    window.draw(swarm_vertex);
    window.display();
  }
}