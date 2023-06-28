#include <SFML/Graphics.hpp>
#include <random>

#include "swarm.hpp"

//-----costants-------
constexpr float mean_position = 750;
constexpr float mean_velocity = 0;
constexpr float position_std = 300;  // standard deviation of normal distribution
constexpr float velocity_std = 1;
constexpr float d_coeff =
    100;  // maximum distance for swarm effects to kick in (Elove method)
constexpr float s_coeff = 30;  // separation coefficent
constexpr int swarm_number = 100;
constexpr float delta_t =
    0.01;  // time increment. higher number, lower speeds.


// random generator of values from gaussian distribution, for randomly
// generating position of boids
// is it trully distributed as a gaussian? redefining
// marsenne twister seems to reset
// pseudo random number generator
float gaussian(double u, double o) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::normal_distribution<double> normal{u, o};
  return normal(eng);
}


int main() {
  std::vector<Boid> boid_vector; //maybe initialize size of vector?
  Point boid_position;
  Point boid_velocity;

  // for loop fills boid vector
  for (int i = 0; i < swarm_number; ++i) {
    // initalizing random boid
    boid_position = Point(gaussian(mean_position, position_std),
                          gaussian(mean_position, position_std));
    boid_velocity = Point(gaussian(mean_velocity, velocity_std),
                          gaussian(mean_velocity, velocity_std));
    boid_vector.push_back(Boid{boid_position, boid_velocity});
  }
  Swarm boid_swarm = Swarm(boid_vector);

  // makes the window and specifies its size and title
  sf::RenderWindow window;
  window.create(sf::VideoMode(1500, 1500), "boids!", sf::Style::Default);

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
    boid_swarm.update(delta_t);

    // here goes the code that updates the boids poisitions, once it will be
    // ready

    // and here goes the code that updates the vertex positions

    // and here goes the code to align the vertex of a triangle with boid
    // direction of motion

    window.draw(boid_swarm.get_vertices());
    window.display();
  }
}