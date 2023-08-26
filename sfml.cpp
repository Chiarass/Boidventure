#include "sfml.hpp"

#include <SFML/Graphics.hpp>

namespace boids {
// todo: is it better to keep the circle alive? how much does it cost in terms
// of performance
void display_circle(sf::RenderWindow& window, double radius, Boid& boid, sf::Color color) {
  sf::CircleShape circle(radius);
  circle.setOutlineColor(color);
  circle.setOutlineThickness(1);
  //makes the circle internally transparent
  circle.setFillColor(sf::Color::Transparent);
  circle.setPosition(boid.pos().x() - radius, boid.pos().y() - radius);
  window.draw(circle);
  //circle.setFillColor(sf::Color(0, 255, 0, 0));
}
}  // namespace boids