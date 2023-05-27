#include "coesion2.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int width = 800;
int height = 600;
int main() {
  std::cout << "Enter the number of boids" << std::endl;
  int n;
  std::cin >> n;
  // Number of shitty bird
  sf::RenderWindow window(sf::VideoMode(width, height),
                          "Analisi del comportamento di uno stormo");
  sf::CircleShape Bird;
  Bird.setRadius(10.f);
  Bird.setPosition()
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }
  window.clear();
  window.display();
  // sf::Vector2f
}