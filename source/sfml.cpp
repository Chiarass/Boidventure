#include "sfml.hpp"

#include <SFML/Graphics.hpp>

namespace boids {
void display_circle(sf::RenderWindow& window, double radius, Boid& boid,
                    sf::Color color) {
  sf::CircleShape circle(radius);
  circle.setOutlineColor(color);
  
  //todo: replace with constant
  circle.setOutlineThickness(1);

  // makes the circle internally transparent
  circle.setFillColor(sf::Color::Transparent);

  // sets the position centered on the boid
  circle.setPosition(boid.pos().x() - radius, boid.pos().y() - radius);

  window.draw(circle);
}
}  // namespace boids