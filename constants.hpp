#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace constants {
inline constexpr double delta_t{
    0.2};  // time increment. higher number, lower speeds.
inline constexpr double window_height{700};
inline constexpr double window_width{900};

// distance between window extremes and margin.
// boids will be generated inside of margin
// and will be pushed back id they go beyond
// margin.
inline constexpr double margin_width{10};
inline constexpr double min_rand_velocity{
    -30};  // minimum velocity component generated randomly
inline constexpr double max_rand_velocity{
    30};  // maximum velocity component generated randomly
inline constexpr int init_boid_number{700};
inline constexpr int init_predator_number{0};

inline constexpr double boid_size{
    5};  // the size of the sfml object representing the boid
inline constexpr double predator_size{8};
// the size of the sfml object representing the predator

inline constexpr double init_separation_coeff{3};  // separation coefficent
inline constexpr double init_cohesion_coeff{1};
inline constexpr double init_alignment_coeff{2};
inline constexpr double turn_coefficent{30};

//todo: make slider
inline constexpr double predator_avoidance_coeff{1};

inline constexpr double repel_coefficent{30};
inline constexpr double repel_range{100};

inline constexpr double range{
    20};  // minimum distance for cohesion and alignmnent
inline constexpr double separation_distance{10};
inline constexpr double init_predator_range{40};

// maximum length of velocity vector
inline constexpr double max_velocity{10};
// when boid goes velocity exceeds maximum velocity,
// the velocity vector gets scaled by this coefficent
inline constexpr double velocity_reduction_coefficent{0.9};

inline constexpr double pi = 3.14159265358979;
inline const sf::Color boid_color{sf::Color::Green};
inline const sf::Color predator_color{sf::Color::Red};

// capacity of quad_tree cell
// subdivides if excedeed
inline constexpr int cell_capacity{10};

//coefficent for sample size in approx distance.
inline constexpr int sample_size_coeff{5};
}  // namespace constants
#endif