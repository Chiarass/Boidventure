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
  /* std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_real_distribution<float> PositionDist(0.0f, width);*/
  std::cout << "Enter the number of boids" << std::endl;
  int n;
  std::cin >> n;
  // Number of birds
  sf::RenderWindow window(sf::VideoMode(width, height),
                          "Analisi del comportamento di uno stormo");
  sf::CircleShape bird(30);
  bird.setPosition(100.f, 100.f);
  bird.setFillColor(sf::Color::Cyan);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
      window.clear();
  window.draw(bird);
  window.display();
  // sf::Vector2f

  }
}