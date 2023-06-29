#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
//-----costants---------

namespace constants{
    inline constexpr double delta_t{0.05};  // time increment. higher number, lower speeds.
    inline constexpr double window_height{1500};
    inline constexpr double window_width{2000};

    //distance between window extremes and margin.
    //boids will be generated inside of margin
    //and will be pushed back id they go beyond
    //margin.
    inline constexpr double margin_width{10}; 
    inline constexpr double min_rand_velocity{-30};  //minimum velocity component generated randomly
    inline constexpr double max_rand_velocity{30};  //maximum velocity component generated randomly
    inline constexpr int swarm_number{100};
    inline constexpr double boid_size{20};  // the size of the sfml object representing the boid

    inline constexpr double separation_coefficent{0.05};  // separation coefficent
    inline constexpr double cohesion_coefficent{0.005};
    inline constexpr double alignment_coefficent{0.01};
    inline constexpr double turn_coefficent{10};

    inline constexpr double distance_coefficent{200}; //minimum distance for cohesion and alignmnent
    inline constexpr double separation_distance{50};

    //maximum length of velocity vector
    inline constexpr double max_velocity{300};
    //when boid goes velocity exceeds maximum velocity,
    //the velocity vector gets scaled by this coefficent
    inline constexpr double velocity_reduction_coefficent{0.9};
}
#endif