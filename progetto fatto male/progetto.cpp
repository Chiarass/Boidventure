#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

const int width = 800;
const int height = 600;
const int numBoids = 20;
const float separationFactor = 0.2f;
const float alignmentFactor = 0.8f;
const float cohesionFactor = 0.8f;
const float separationDistance = 20.0f;
const float maxSpeed = 7.0f;

struct Boid {
  sf::Vector2f position;
  sf::Vector2f velocity;
};

float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
  sf::Vector2f diff = v1 - v2;
  return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void updateBoids(std::vector<Boid>& boids) {
  std::vector<sf::Vector2f> updatedVelocities(boids.size());
  sf::Vector2f centerOfMass(0.0f, 0.0f);
  int numNeighbors = 0;

  for (int i = 0; i < boids.size(); ++i) {
    sf::Vector2f separation(0.0f, 0.0f);
    sf::Vector2f alignment(0.0f, 0.0f);
    sf::Vector2f cohesion(0.0f, 0.0f);
    numNeighbors = 0;

    for (int j = 0; j < boids.size(); ++j) {
      if (i != j) {
        float dist = distance(boids[i].position, boids[j].position);

        if (dist < separationDistance) {
          sf::Vector2f diff = boids[i].position - boids[j].position;
          separation += diff / dist;
        }

        if (dist < separationDistance * 2.0f) {
          alignment += boids[j].velocity;
          ++numNeighbors;
        }

        cohesion += boids[j].position;
      }
    }

    if (numNeighbors > 0) {
      alignment /= static_cast<float>(numNeighbors);
      alignment -= boids[i].velocity;
      alignment *= alignmentFactor;
    }

    if (numNeighbors > 0) {
      cohesion /= static_cast<float>(numNeighbors);
      cohesion -= boids[i].position;
      cohesion *= cohesionFactor;
    }

    updatedVelocities[i] = boids[i].velocity + separation * separationFactor +
                           alignment + cohesion;
    float speed = std::sqrt(updatedVelocities[i].x * updatedVelocities[i].x +
                            updatedVelocities[i].y * updatedVelocities[i].y);

    if (speed > maxSpeed) {
      updatedVelocities[i] *= maxSpeed / speed;
    }

    centerOfMass += boids[i].position;
  }

  centerOfMass /= static_cast<float>(boids.size());

  for (int i = 0; i < boids.size(); ++i) {
    boids[i].velocity = updatedVelocities[i];
    boids[i].position += boids[i].velocity;

    // Implementazione del comportamento ai bordi
    if (boids[i].position.x < 0.0f) {
      boids[i].position.x = width;
    } else if (boids[i].position.x > width) {
      boids[i].position.x = 0.0f;
    }

    if (boids[i].position.y < 0.0f) {
      boids[i].position.y = height;
    } else if (boids[i].position.y > height) {
      boids[i].position.y = 0.0f;
    }
  }
}

int main() {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_real_distribution<float> positionDist(0.0f, width);
  std::uniform_real_distribution<float> velocityDist(-1.0f, 1.0f);

  sf::RenderWindow window(sf::VideoMode(width, height), "Flocking Birds");

  std::vector<Boid> boids(numBoids);

  for (auto& boid : boids) {
    boid.position.x = positionDist(engine);
    boid.position.y = positionDist(engine);
    boid.velocity.x = velocityDist(engine);
    boid.velocity.y = velocityDist(engine);
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    updateBoids(boids);

    window.clear();

    for (const auto& boid : boids) {
      sf::CircleShape shape(5.0f);
      shape.setPosition(boid.position);
      shape.setFillColor(sf::Color::White);
      window.draw(shape);
    }

    window.display();
  }

  return 0;
}