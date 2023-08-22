#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {
void vertex_update(sf::VertexArray& swarm_vertex, const Boid& boid, int index) {
  // should convert implicitly to double anyway, so no
  // need to static cast it i think.
  Point forward_vertex{0., 0.};
  if ((boid.vel()).distance() != 0)
    forward_vertex =
        (constants::boid_size / boid.vel().distance()) * (boid.vel());

  swarm_vertex[3 * index].position =
      // i have added the *2 so the front is longer,
      // and we can distinguish it.
      sf::Vector2f((boid.pos() + 2 * forward_vertex).x(),
                   (boid.pos() + 2 * forward_vertex).y());

  // rotate by 120
  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 1].position = sf::Vector2f(
      (boid.pos() + forward_vertex).x(), (boid.pos() + forward_vertex).y());

  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 2].position = sf::Vector2f(
      (boid.pos() + forward_vertex).x(), (boid.pos() + forward_vertex).y());
}
}  // namespace boids