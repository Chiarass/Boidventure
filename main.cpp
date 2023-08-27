#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <random>

#include "boid.hpp"
#include "constants.hpp"
#include "gui.hpp"
#include "quadtree.hpp"
#include "sfml.hpp"
#include "statistics.hpp"

// template, to take both predators and boid types
template <class Bird_type>
void initialize_boids(std::vector<Bird_type>& bird_vec,
                      sf::VertexArray& vertices, double swarm_n,
                      sf::Color bird_color) {
  bird_vec.clear();
  vertices.clear();
  for (int i = 0; i < swarm_n; ++i) {
    // initializes boid within a margin from screen border and control panel
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

    // append each vertex to the boid_vertex array
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

  // quad tree object, partitions space improving performance
  boids::Quad_tree tree{
      constants::cell_capacity,
      boids::Rectangle{
          (constants::window_width + constants::controls_width) / 2.,
          constants::window_height / 2.,
          (constants::window_width - constants::controls_width) / 2.,
          constants::window_height / 2.}};

  // array of vertices of triangle of a boid.
  // for each boid three vertices
  sf::VertexArray boid_vertex{sf::Triangles};

  // array of vertices of triangle of a predator
  // for each predator three vertices
  sf::VertexArray predator_vertex{sf::Triangles};

  // makes the window and specifies it's size and title
  sf::RenderWindow window;
  window.create(
      sf::VideoMode(constants::window_width, constants::window_height),
      "boids!", sf::Style::Default);

  // lock framerate to 60 fps
  window.setFramerateLimit(60);

  tgui::GuiSFML gui{window};

  // clock for fps calculation
  sf::Clock clock;

  // booleans for gui buttons
  bool display_tree{false};
  bool display_range{false};
  bool display_separation_range{false};
  bool display_prey_range{false};

  // panel object, it manages the tgui sliders, labels and buttons
  // todo: replace with constants
  boids::Panel panel(constants::widget_width, constants::widget_height,
                     constants::gui_element_distance, 10., 10.);
  initialize_panel(gui, panel, display_tree, display_range,
                   display_separation_range, display_prey_range);

  // bool for tracking if moused is pressed, for boid repulsion
  bool is_mouse_pressed{false};

  // declaring boid parameters
  double separation_coefficent{};
  double cohesion_coefficent{};
  double alignment_coefficent{};
  double range{};
  double separation_range{};
  double prey_range{};

  // initialize with absurd number so it automatically initializes boids
  int boid_number{-1};
  int predator_number{};

  // SFML loop. After each loop the window is updated
  while (window.isOpen()) {
    // fps calculation
    auto current_time = clock.restart().asSeconds();
    double fps = 1. / (current_time);

    sf::Event event;
    while (window.pollEvent(event)) {
      gui.handleEvent(event);
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::MouseButtonPressed &&
          // if gui.handleEvent(event) is true (ex. a button is pressed) no
          // repulsion occours
          !gui.handleEvent(event)) {
        is_mouse_pressed = true;
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        is_mouse_pressed = false;
      }
    }

    // makes the window return black
    window.clear(sf::Color::Black);

    // todo: find a way to move to update_from_panel function

    // if the value of the slider is changed, change number of boids
    // todo: add check (or assert) for dynamic cast does returning null pointer
    if (static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
                             panel.elements[Element_key::boid_number_slider])
                             ->getValue()) != boid_number) {
      boid_number =
          static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
                               panel.elements[Element_key::boid_number_slider])
                               ->getValue());
      initialize_boids(boid_vector, boid_vertex, boid_number,
                       constants::boid_color);
    }

    // if the value of the slider is changed, change number of predators
    if (static_cast<int>(
            std::dynamic_pointer_cast<tgui::Slider>(
                panel.elements[Element_key::predator_number_slider])
                ->getValue()) != predator_number) {
      predator_number = static_cast<int>(
          std::dynamic_pointer_cast<tgui::Slider>(
              panel.elements[Element_key::predator_number_slider])
              ->getValue());
      initialize_boids(predator_vector, predator_vertex, predator_number,
                       constants::predator_color);
    }

    for (auto& boid : boid_vector) {
      tree.insert(&boid);
    }

    // handles boid/predator repulsion
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

    // todo: replace with algorithm?
    // updates the predator positions
    for (int i = 0; i != static_cast<int>(predator_vector.size()); ++i) {
      predator_vector[i].update_predator(constants::delta_t_predator,
                                         boid_vector);
      boids::vertex_update(predator_vertex, predator_vector[i], i,
                           constants::predator_size);
    }

    // updates the boid positions
    for (int i = 0; i != static_cast<int>(boid_vector.size()); ++i) {
      std::vector<boids::Boid*> in_range;
      tree.query(range, boid_vector[i], in_range);
      boid_vector[i].update_boid(constants::delta_t_boid, in_range,
                                 separation_range, separation_coefficent,
                                 cohesion_coefficent, alignment_coefficent);
      for (auto& predator : predator_vector) {
        boid_vector[i].escape_predator(predator, prey_range,
                                       constants::predator_avoidance_coeff);
      }
      boids::vertex_update(boid_vertex, boid_vector[i], i,
                           constants::boid_size);
    }

    // update the value of boid parameters based on the slider values
    boids::update_from_panel(panel, fps, cohesion_coefficent,
                             alignment_coefficent, separation_coefficent, range,
                             separation_range, prey_range);

    // if corresponding button is pressed, displays the ranges of the first boid
    // in the vector
    boids::display_ranges(range, separation_range, prey_range, display_range,
                          display_separation_range, display_prey_range,
                          boid_vector, window);

    // if the show cells button is pressed the tree object is displayed
    if (display_tree) tree.display(window);

    // deletes the dinamically allocated objects in quadtree
    tree.delete_tree();

    window.draw(boid_vertex);
    window.draw(predator_vertex);

    gui.draw();

    window.display();
  }
}