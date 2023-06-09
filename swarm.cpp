#include "swarm.hpp"

//definies + for points
float operator+(point a, point b){
point sum;
sum.x = a.x +b.x;
sum.y = a.y + b.y;
}

//boid constructor
Boid::Boid(point a, point b): m_x{a}, m_v{b} {};

//swarm constructor
Swarm::Swarm(std::vector <Boid> boids) : m_boids{boids} {};
//here go the updating functions
