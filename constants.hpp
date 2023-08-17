#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace constants {
inline constexpr double delta_t{
    0.2};  // time increment. higher number, lower speeds. //0.2
inline constexpr double window_height{800};
inline constexpr double window_width{1000};

// distance between window extremes and margin.
// boids will be generated inside of margin
// and will be pushed back id they go beyond
// margin.
inline constexpr double margin_width{10};
inline constexpr double min_rand_velocity{
    -30};  // minimum velocity component generated randomly
inline constexpr double max_rand_velocity{
    30};   // maximum velocity component generated randomly
inline int swarm_number{1500};
inline constexpr double boid_size{
    7};  // the size of the sfml object representing the boid

inline double separation_coefficent{0.2};  // separation coefficent
inline double cohesion_coefficent{0.01};
inline double alignment_coefficent{0.05};
inline constexpr double turn_coefficent{20};

inline constexpr double repel_coefficent{30};
inline constexpr double repel_range{100};

inline constexpr double range{
    35};  // minimum distance for cohesion and alignmnent
inline constexpr double separation_distance{20};

// maximum length of velocity vector
inline constexpr double max_velocity{10};
// when boid goes velocity exceeds maximum velocity,
// the velocity vector gets scaled by this coefficent
inline constexpr double velocity_reduction_coefficent{0.9};

inline constexpr double pi = 3.14159265358979;
inline const sf::Color boid_color{sf::Color::Green};

// capacity of quad_tree cell
// subdivides if excedeed
inline constexpr int cell_capacity{5};
}  // namespace constants
#endif