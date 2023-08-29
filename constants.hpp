#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace constants {
inline constexpr double delta_t_boid{
    1};  // time increment. higher number, lower speeds.
inline constexpr double delta_t_predator{
    1};  // time increment. higher number, lower speeds.

// window constants ///////////////////////////////////////
inline constexpr double window_height{700};
inline constexpr double window_width{1000};

// distance between window extremes and margin.
// boids will be generated inside of margin
// and will be pushed back if they go beyond
// margin.
inline constexpr double margin_size{20};

// width of the space dedicated to sliders and buttons
inline constexpr double controls_width{230};
///////////////////////////////////////////////////////////

// birds constants ////////////////////////////////////////
// the following represent the maximum values achievable through sliders
inline constexpr double max_cohesion_strength{0.02};
inline constexpr double max_separation_strength{0.5};
inline constexpr double max_alignment_strength{0.2};

inline constexpr int max_boid_number{2500};
inline constexpr int max_predator_number{10};

inline constexpr double max_range{40};
inline constexpr double max_separation_range{15};
inline constexpr double max_prey_range{60};

// the following represents the initial position of the slider, with 0
// indicating the slider all the way to the left, and 10 indicating the slider
// all the way to the right.
inline constexpr double init_separation_coeff{3};
inline constexpr double init_cohesion_coeff{3};
inline constexpr double init_alignment_coeff{7};

inline constexpr int init_boid_number{300};
inline constexpr int init_predator_number{0};

// minimum randomly generated velocity component of randomly generated boid
inline constexpr double min_rand_velocity{-3};
// maximum randomly generated velocity component of randomly generated boid
inline constexpr double max_rand_velocity{3};

///////////////////////////////////////////////////////////

// gui constants //////////////////////////////////////////
// height and width of sliders, buttons and text
inline constexpr double widget_width{200.};
inline constexpr double widget_height{25.};

// the position of the first element of the panel
inline constexpr double first_element_position{10.};

// the distance between panel elements
inline constexpr double gui_element_distance{30.};
///////////////////////////////////////////////////////////

inline constexpr double boid_size{
    5};  // the size of the sfml object representing the boid
inline constexpr double predator_size{8};
// the size of the sfml object representing the predator

// todo: solve predators push boids out of bounds problem
inline constexpr double turn_coefficent{0.5};

// todo: make slider
inline constexpr double predator_avoidance_coeff{0.2};
inline constexpr double predator_hunting_coeff{0.05};

inline constexpr double repel_coefficent{2};
inline constexpr double repel_range{100};

inline constexpr double init_range{
    6};  // minimum distance for cohesion and alignmnent
inline constexpr double init_separation_range{6};
inline constexpr double init_prey_range{5};

// the range for the predator objects is the prey range multiplied
// by this coefficent
inline constexpr double prey_to_predator_coeff{1.5};

// maximum length of velocity vector
inline constexpr double max_velocity{3};
// when boid goes velocity exceeds maximum velocity,
// the velocity vector gets scaled by this coefficent
inline constexpr double velocity_reduction_coefficent{0.9};

inline constexpr double pi = 3.14159265358979;
inline const sf::Color boid_color{sf::Color::Green};
inline const sf::Color predator_color{sf::Color::Red};

// capacity of quad_tree cell
// subdivides if excedeed
inline constexpr int cell_capacity{10};

// coefficent for sample size in approx distance.
inline constexpr int sample_size_coeff{5};
}  // namespace constants
#endif