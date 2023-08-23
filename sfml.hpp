//i used a more specific name than SFML_HPP because it may not be
//so rare for files to be called SFML
#ifndef SFML_HPP_118
#define SFML_HPP_118
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

//todo: check if implementation of template is correct
namespace boids {
template<class Bird_type>
void vertex_update(sf::VertexArray& swarm_vertex, const Bird_type& bird, int index, double size) {
  // should convert implicitly to double anyway, so no
  // need to static cast it i think.
  Point forward_vertex{0., 0.};
  if ((bird.vel()).distance() != 0)
    forward_vertex =
        (size / bird.vel().distance()) * (bird.vel());

  swarm_vertex[3 * index].position =
      // i have added the *2 so the front is longer,
      // and we can distinguish it.
      sf::Vector2f((bird.pos() + 2 * forward_vertex).x(),
                   (bird.pos() + 2 * forward_vertex).y());

  // rotate by 120
  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 1].position = sf::Vector2f(
      (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());

  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 2].position = sf::Vector2f(
      (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());
}
}
#endif