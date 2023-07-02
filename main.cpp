#include "swarm.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <random>

// random generator of values from uniform distribution, for randomly
// generating position of boids
// is it trully distributed as a uniform? redefining
// marsenne twister seems to reset
// pseudo random number generator
double uniform(double a, double b) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_real_distribution<double> unif{a, b};
  return unif(eng);
}


int main() {
  std::vector<boids::Boid> boid_vector(constants::swarm_number);
  boids::Point boid_position;
  boids::Point boid_velocity;

  // for loop fills boid vector
  for (int i = 0; i < constants::swarm_number; ++i) {
    // initalizing random boid
    // the parameters in uniform set the maximum and minimum
    // coordinates the position/velocity can be generated with
    // the minimum in boid_position is margin_width because we
    // want the boid to be generated within that distance
    // from the edge of the window, and the upper left corner
    // of the window has position (0,0).
    boid_position = boids::Point {uniform(constants::margin_width, constants::window_width - constants::margin_width),
                         uniform(constants::margin_width, constants::window_height - constants::margin_width)};
    boid_velocity = boids::Point {uniform(constants::min_rand_velocity, constants::max_rand_velocity),
                         uniform(constants::min_rand_velocity, constants::max_rand_velocity)};
    boid_vector[i] = boids::Boid{boid_position, boid_velocity};
  }
  boids::Swarm boid_swarm = boids::Swarm(boid_vector);

  // makes the window and specifies its size and title
  sf::RenderWindow window;
  window.create(sf::VideoMode(constants::window_width, constants::window_height), "boids!", sf::Style::Default);

  // SFML loop. After each loop the window is updated
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // makes the window return black
    window.clear(sf::Color::Black);
    
    // makes boids move in straight lines with
    // assigned velocities.
    boid_swarm.update(constants::delta_t);
    
    window.draw(boid_swarm.get_vertices());
    window.display();
  }
}