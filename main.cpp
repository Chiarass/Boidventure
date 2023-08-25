#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <random>

#include "boid.hpp"
#include "constants.hpp"
#include "quadtree.hpp"
#include "sfml.hpp"
#include "statistics.hpp"

template <class Bird_type>
void initialize_boids(std::vector<Bird_type>& bird_vec,
                      sf::VertexArray& vertices, double swarm_n,
                      sf::Color bird_color) {
  bird_vec.clear();
  vertices.clear();
  for (int i = 0; i < swarm_n; ++i) {
    auto boid_position = boids::Point{
        boids::uniform(constants::margin_size + constants::controls_width,
                       constants::window_width - constants::margin_size),
        boids::uniform(constants::margin_size,
                       constants::window_height - constants::margin_size)};
    auto boid_velocity =
        boids::Point{boids::uniform(constants::min_rand_velocity,
                                    constants::max_rand_velocity),
                     boids::uniform(constants::min_rand_velocity,
                                    constants::max_rand_velocity)};
    bird_vec.push_back(Bird_type{boid_position, boid_velocity});

    // Append each vertex to the boid_vertex array
    sf::Vertex v1(sf::Vector2f(boid_position.x(), boid_position.y()),
                  bird_color);
    sf::Vertex v2(sf::Vector2f(boid_position.x(), boid_position.y()),
                  bird_color);
    sf::Vertex v3(sf::Vector2f(boid_position.x(), boid_position.y()),
                  bird_color);
    vertices.append(v1);
    vertices.append(v2);
    vertices.append(v3);
  }
}

int main() {
  std::vector<boids::Boid> boid_vector;
  std::vector<boids::Predator> predator_vector;

  boids::Quad_tree tree{
      constants::cell_capacity,
      boids::Rectangle{
          (constants::window_width + constants::controls_width) / 2.,
          constants::window_height / 2.,
          (constants::window_width - constants::controls_width) / 2.,
          constants::window_height / 2.}};
  sf::VertexArray boid_vertex{sf::Triangles};
  sf::VertexArray predator_vertex{sf::Triangles};

  // makes the window and specifies its size and title
  sf::RenderWindow window;
  window.create(
      sf::VideoMode(constants::window_width, constants::window_height),
      "boids!", sf::Style::Default);

  tgui::GuiSFML gui{window};

  sf::Font font;
  // added ../ case for running from vscode
  if (!font.loadFromFile("./aAreaKilometer50.ttf"))
    font.loadFromFile("../aAreaKilometer50.ttf");

  // statistics text //////////////////////////////////////////
  sf::Text data_text;
  data_text.setFont(font);
  data_text.setFillColor(sf::Color::White);
  data_text.setCharacterSize(10);
  data_text.setPosition(constants::window_width - 150, 10);
  ////////////////////////////////////////////////////////////

  bool is_mouse_pressed{false};

  // create 4 vectors to store distances, velocity, average distances and
  // velocities
  std::vector<double> distances;
  std::vector<double> velocities;
  std::vector<double> average_distances;
  std::vector<double> average_velocities;

  double separation_coefficent{constants::init_separation_coeff};
  double cohesion_coefficent{constants::init_cohesion_coeff};
  double alignment_coefficent{constants::init_alignment_coeff};

  // initialize with absurd number so it automatically initializes boids
  int boid_number{1};
  int predator_number{0};

  // SFML loop. After each loop the window is updated
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      if (event.type == sf::Event::Closed) window.close();
      // if gui.handleEvent(event) is true (ex. a button is pressed) no
      // repulsion occours
      if (event.type == sf::Event::MouseButtonPressed &&
          !gui.handleEvent(event)) {
        is_mouse_pressed = true;
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        is_mouse_pressed = false;
      }
    }
/* 
    if (static_cast<int>(gui.boid_number_slider->getValue()) != boid_number) {
      boid_number = static_cast<int>(boid_number_slider->getValue());
      initialize_boids(boid_vector, boid_vertex, boid_number,
                       constants::boid_color);
    } */

/*     if (static_cast<int>(predator_number_slider->getValue()) !=
        predator_number) {
      predator_number = static_cast<int>(predator_number_slider->getValue());
      initialize_boids(predator_vector, predator_vertex, predator_number,
                       constants::predator_color);
    } */

    distances.clear();
    velocities.clear();
    // Calculating stats about the flock
    double total_distance = 0.0;
    double total_velocity = 0.0;

    // todo: make it work at low number of boids. to implement after testing of
    // apporx distance
    //  for (const auto& boid : boid_vector) {
    //    double boid_distance = 0.0;
    //    for (const auto& other_boid : boid_vector) {
    //      boid_distance += (boid.pos() - other_boid.pos()).distance();
    //    }
    //    total_distance += boid_distance;
    //    total_velocity += boid.vel().distance();

    //   distances.push_back(boid_distance);
    //   velocities.push_back(boid.vel().distance());
    // }

    // todo: test approx distance
    double app_distance = boids::approx_distance(
        boid_vector,
        constants::sample_size_coeff * static_cast<int>(boid_vector.size()));

    // Calculate average distances and velocities
    total_distance = std::accumulate(distances.begin(), distances.end(), 0.0);
    total_velocity = std::accumulate(velocities.begin(), velocities.end(), 0.0);

    double average_distance =
        total_distance / (boid_vector.size() * boid_vector.size());
    double average_velocity = total_velocity / boid_vector.size();

    // Calculate deviation distances and velocities
    double deviation_distance =
        boids::calculate_standard_deviation(distances, average_distance);
    double deviation_velocity =
        boids::calculate_standard_deviation(velocities, average_velocity);

    // Adding data to the vectors
    average_distances.push_back(average_distance);
    average_velocities.push_back(average_velocity);

    // makes the window return black
    window.clear(sf::Color::Black);

    for (auto& boid : boid_vector) {
      // couldn't i do it with references?
      tree.insert(&boid);
    }

    if (is_mouse_pressed) {
      boids::Point mouse_position(sf::Mouse::getPosition(window).x,
                                  sf::Mouse::getPosition(window).y);
      for (auto& boid : boid_vector) {
        if ((boid.pos() - mouse_position).distance() < constants::repel_range)
          boid.repel(mouse_position);
      }

      for (auto& predator : predator_vector) {
        if ((predator.pos() - mouse_position).distance() <
            constants::repel_range)
          predator.repel(mouse_position);
      }
    }

    for (int i = 0; i != static_cast<int>(predator_vector.size()); ++i) {
      predator_vector[i].update_predator(constants::delta_t_predator,
                                         boid_vector);
      boids::vertex_update(predator_vertex, predator_vector[i], i,
                           constants::predator_size);
    }

    for (int i = 0; i != static_cast<int>(boid_vector.size()); ++i) {
      std::vector<boids::Boid*> in_range;
      tree.query(constants::init_range, boid_vector[i], in_range);
      boid_vector[i].update_boid(
          constants::delta_t_boid, in_range, constants::init_separation_range,
          separation_coefficent, cohesion_coefficent, alignment_coefficent);
      for (auto& predator : predator_vector) {
        boid_vector[i].escape_predator(predator, constants::init_prey_range,
                                       constants::predator_avoidance_coeff);
      }
      boids::vertex_update(boid_vertex, boid_vector[i], i,
                           constants::boid_size);
    }
/* 
    cohesion_coefficent = 0.1 * (cohesion_slider->getValue());
    alignment_coefficent = 0.1 * (alignment_slider->getValue());
    separation_coefficent = 0.1 * (separation_slider->getValue()); 
    */
    if (display_tree) tree.display(window);
    tree.delete_tree();
    window.draw(boid_vertex);
    window.draw(predator_vertex);
    gui.draw();l
    window.display();
  }
}