#include "sfml.hpp"

#include <SFML/Graphics.hpp>

#include "constants.hpp"

namespace boids {
void display_circle(sf::RenderWindow& window, double radius, Boid& boid,
                    sf::Color color) {
  assert(radius >= 0.);
  sf::CircleShape circle(radius);
  circle.setOutlineColor(color);
  circle.setOutlineThickness(constants::range_thickness);

  // makes the circle internally transparent
  circle.setFillColor(sf::Color::Transparent);

  // sets the position centered on the boid
  circle.setPosition(boid.pos().x() - radius, boid.pos().y() - radius);

  window.draw(circle);
}
}  // namespace boids