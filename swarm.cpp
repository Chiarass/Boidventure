#include "swarm.hpp"
#include "point.hpp"
#include "constants.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// swarm constructor. parameter gets assignet to m_boid. also:
// initializes a  vector of vertex arrays. They each store the vertexes of one
// boid's triangle. the size is three times that of the number of boids
// because each boid is associated with a triangle (three vertexes)

Swarm::Swarm(const std::vector<Boid>& boids) : m_boids{boids} {
  // passed by reference to save computation. is it right?

  sf::VertexArray swarm_vertex(sf::Triangles, 3 * m_boids.size());
  m_vertices = swarm_vertex;  // can't brace initialize, don't know why.

  // static_cast only serves the purpose of removing warning
  // because m_boids.size() is not an int

  for (int i = 0; i < static_cast<int>(m_boids.size()); ++i) {
    //maybe make a constant for the color?
    m_vertices[i * 3].color = sf::Color::Green;
    m_vertices[i * 3 + 1].color = sf::Color::Green;
    m_vertices[i * 3 + 2].color = sf::Color::Green;
  }
};


Point Swarm::separation(std::vector<Boid>::iterator iti){ 
  Point added_velocity{0, 0};
  for(auto itj = m_boids.begin(); itj < m_boids.end(); ++itj){
    if(std::distance(itj,iti) != 0 && (iti->r - itj->r).distance() < constants::separation_distance)
    {
      added_velocity = added_velocity + (iti->r - itj->r);
    }
  }
  return (constants::separation_coefficent)*added_velocity;
}


Point Swarm::cohesion(std::vector<Boid>::iterator iti){ 
  Point added_velocity{0, 0};
  int in_range{0};
  for(auto itj = m_boids.begin(); itj < m_boids.end(); ++itj){
    if(std::distance(itj,iti) != 0 && (iti->r - itj->r).distance() < constants::distance_coefficent)
    {
      added_velocity = added_velocity + (itj->r);
      ++in_range;
    }
  }

  if(in_range)
  {
  added_velocity = constants::cohesion_coefficent*((1./(in_range))*added_velocity - (iti->r));
  }
  return added_velocity;
}

Point Swarm::alignment(std::vector<Boid>::iterator iti){
   
  Point added_velocity{0, 0};
  int in_range{0};
  for(auto itj = m_boids.begin(); itj < m_boids.end(); ++itj){
    if(std::distance(itj,iti) != 0 && (iti->r - itj->r).distance() < constants::distance_coefficent)
    {
      added_velocity = added_velocity + (itj->v);
      ++in_range;
    }
  }

  if(in_range)
  {
  added_velocity = constants::alignment_coefficent*((1./(in_range))*added_velocity - (iti->v));
  }
  return added_velocity;
  }


//makes the boid turn around if too close to the edge of the window.
//the upper left edge of the window has poistion (0,0).
//also the y axis is upside down.
Point Swarm::turn_around(std::vector<Boid>::iterator iti){
  if(iti->r.x() > constants::window_width - constants::margin_width) return {-constants::turn_coefficent, 0.};
  if(iti->r.x() < constants::margin_width) return {constants::turn_coefficent, 0.};
  if(iti->r.y() > constants::window_height - constants::margin_width) return {0., -constants::turn_coefficent};
  if(iti->r.y() < constants::margin_width) return {0., constants::turn_coefficent};
  return {0.,0.};
}

// updates vertex position. They form an equilateral triangle,
// boid is positioned in the middle of the base.
// x and y parameters represent boid position

void Swarm::vertex_update(std::vector<Boid>::iterator it) {
  // considering not using iterators, to make
  // code more readable. Also, better to change
  // to an alogtirithm if possible.
  // add comment explanation

    //should convert implicitly to double anyway, so no
    //need to static cast it i think.
    Point forward_vertex{0.,0.};
    if((it->v).distance() != 0)
    forward_vertex = (constants::boid_size/it->v.distance())*(it->v);

    m_vertices[3 *(it-m_boids.begin())].position =
        //i have added the *2 so the front is longer,
        //and we can distinguish it.
        sf::Vector2f((it->r + 2*forward_vertex).x(), (it->r + 2*forward_vertex).y());

    //rotate by 120
    forward_vertex.rotate(2./3*constants::pi);

    m_vertices[(3 * (it-m_boids.begin())) + 1].position =
        sf::Vector2f((it->r + forward_vertex).x(), (it->r + forward_vertex).y());
      
    forward_vertex.rotate(2./3*constants::pi);

    m_vertices[(3 * (it-m_boids.begin())) + 2].position =
        sf::Vector2f((it->r + forward_vertex).x(), (it->r + forward_vertex).y());
}

void Swarm::update(double delta_t) {
  for (auto it = m_boids.begin(); it < m_boids.end(); it++) {
    // for loop, change to algorithm?
    if(it->v.distance() < constants::max_velocity){
    it->v = it->v + separation(it) + cohesion(it) + alignment(it) + turn_around(it);
    }
    else{
      //is using an unidentified object point bad practice?
      it->v = Point{constants::velocity_reduction_coefficent*(it->v.x()), constants::velocity_reduction_coefficent*(it->v.y())};
    }
    it->r = delta_t * (it->v)+ (it->r);
    vertex_update(it);
  }
}

sf::VertexArray Swarm::get_vertices() { return m_vertices; }