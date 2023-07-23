#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace constants {
inline constexpr double delta_t{
    0.0035};  // time increment. higher number, higher speeds. added 1 zeros
inline constexpr double window_height{1500};
inline constexpr double window_width{2000};

// distance between window extremes and margin.
// boids will be generated inside of margin
// and will be pushed back if they go beyond
// margin.
inline constexpr double margin_width{10};
inline constexpr double min_rand_velocity{
    -30};  // minimum initial velocity component generated randomly
inline constexpr double max_rand_velocity{
    30};   // maximum initial velocity component generated randomly
inline constexpr int swarm_number{100};
inline constexpr double boid_size{
    25};  // the size of the sfml object representing the boid

inline constexpr double separation_coefficent{0.05};  // separation coefficent
inline constexpr double cohesion_coefficent{0.005};
inline constexpr double alignment_coefficent{0.01};
inline constexpr double turn_coefficent{10};
// minimum distance for cohesion and alignmnent to have effect
inline constexpr double distance_coefficent{200};
// minimum distance for separation to have effect
inline constexpr double separation_distance{50};

// maximum length of velocity vector
inline constexpr double max_velocity{100};
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