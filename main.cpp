#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <random>

#include "boid.hpp"
#include "constants.hpp"
#include "quadtree.hpp"
#include "statistics.hpp"
#include "sfml.hpp"

void initialize_boids(std::vector<boids::Boid>& boid_vec, sf::VertexArray& vertices, double swarm_n) {
  boid_vec.clear();
  vertices.clear();
  for (int i = 0; i < swarm_n; ++i) {
    auto boid_position = boids::Point{
        boids::uniform(constants::margin_width,
                       constants::window_width - constants::margin_width),
        boids::uniform(constants::margin_width,
                       constants::window_height - constants::margin_width)};
    auto boid_velocity =
        boids::Point{boids::uniform(constants::min_rand_velocity,
                                    constants::max_rand_velocity),
                     boids::uniform(constants::min_rand_velocity,
                                    constants::max_rand_velocity)};
    boid_vec.push_back(boids::Boid{boid_position, boid_velocity});

    // Append each vertex to the swarm_vertex array
    sf::Vertex v1(sf::Vector2f(boid_position.x(), boid_position.y()),
                  constants::boid_color);
    sf::Vertex v2(sf::Vector2f(boid_position.x(), boid_position.y()),
                  constants::boid_color);
    sf::Vertex v3(sf::Vector2f(boid_position.x(), boid_position.y()),
                  constants::boid_color);
    vertices.append(v1);
    vertices.append(v2);
    vertices.append(v3);
  }
}

int main() {
  std::vector<boids::Boid> boid_vector;
  boids::Quad_tree tree{constants::cell_capacity,
                        boids::Rectangle{constants::window_width / 2.,
                                         constants::window_height / 2.,
                                         constants::window_width / 2.,
                                         constants::window_height / 2.}};
  sf::VertexArray swarm_vertex{sf::Triangles};

  // makes the window and specifies its size and title
  sf::RenderWindow window;
  window.create(
      sf::VideoMode(constants::window_width, constants::window_height),
      "boids!", sf::Style::Default);

  tgui::GuiSFML gui{window};
  bool display_tree{false};

  // button for quad tree
  tgui::Button::Ptr cell_button = tgui::Button::create();
  cell_button->setPosition(10., 10.);
  cell_button->setSize(70., 20.);
  cell_button->setText("show cells");
  cell_button->onPress([&display_tree] { display_tree = !display_tree; });
  gui.add(cell_button);

  // slider for cohesion
  tgui::Slider::Ptr cohesion_slider = tgui::Slider::create();
  cohesion_slider->setPosition(10., 40.);
  cohesion_slider->setValue(constants::init_cohesion_coeff);
  gui.add(cohesion_slider);

  // slider for alignment
  tgui::Slider::Ptr alignment_slider = tgui::Slider::create();
  alignment_slider->setPosition(10., 60.);
  alignment_slider->setValue(constants::init_alignment_coeff);
  gui.add(alignment_slider);

  // slider for separation
  tgui::Slider::Ptr separation_slider = tgui::Slider::create();
  separation_slider->setPosition(10., 80.);
  separation_slider->setValue(constants::init_separation_coeff);
  gui.add(separation_slider);

  // slider for number of boids
  tgui::Slider::Ptr number_slider = tgui::Slider::create();
  number_slider->setPosition(10., 100.);
  number_slider->setMinimum(1);
  //todo: replace with constant
  number_slider->setMaximum(1500);
  number_slider->setValue(constants::init_swarm_number);
  gui.add(number_slider);

  bool lock_click{false};

  // create 4 vectors to store distances, velocity, average distances and
  // velocities
  std::vector<double> distances;
  std::vector<double> velocities;
  std::vector<double> average_distances;
  std::vector<double> average_velocities;

  double separation_coefficent{constants::init_separation_coeff};
  double cohesion_coefficent{constants::init_cohesion_coeff};
  double alignment_coefficent{constants::init_alignment_coeff};
  //initialize with absurd number so it automatically initializes boids
  int swarm_number{1};

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
        lock_click = true;
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        lock_click = false;
      }
    }

    if (static_cast<int>(number_slider->getValue()) != swarm_number) {
      swarm_number = static_cast<int>(number_slider->getValue());
      initialize_boids(boid_vector, swarm_vertex, swarm_number);
    }

    distances.clear();
    velocities.clear();
    // Calculating stats about the flock
    double total_distance = 0.0;
    double total_velocity = 0.0;
    /*
    /////////////todo: delete, too slow////////////////////////
    for (const auto& boid : boid_vector) {
      double boid_distance = 0.0;
      for (const auto& other_boid : boid_vector) {
        boid_distance += (boid.pos() - other_boid.pos()).distance();
      }
      total_distance += boid_distance;
      total_velocity += boid.vel().distance();

      distances.push_back(boid_distance);
      velocities.push_back(boid.vel().distance());
    }
    */
    ///////////////////////////////////////////////////////////
    ////////////test///////////////////////////
    double app_distance = boids::approx_distance(boid_vector, 100);
    ///////////////////////////////////////////
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

    if (lock_click) {
      boids::Point mouse_position(sf::Mouse::getPosition(window).x,
                                  sf::Mouse::getPosition(window).y);
      for (auto& boid : boid_vector) {
        if ((boid.pos() - mouse_position).distance() < constants::repel_range)
          boid.repel(mouse_position);
      }
    }

    for (int i = 0; i != static_cast<int>(boid_vector.size()); ++i) {
      std::vector<boids::Boid*> in_range;
      tree.query(constants::range, boid_vector[i], in_range);
      boid_vector[i].update(
          constants::delta_t, in_range, constants::separation_distance,
          separation_coefficent, cohesion_coefficent,
          alignment_coefficent);
      boids::vertex_update(swarm_vertex, boid_vector[i], i);
    }

    cohesion_coefficent = 0.1 * (cohesion_slider->getValue());
    alignment_coefficent = 0.1 * (alignment_slider->getValue());
    separation_coefficent = 0.1 * (separation_slider->getValue());
    if (display_tree) tree.display(window);
    tree.delete_tree();
    window.draw(swarm_vertex);
    gui.draw();
    // displaying all the stats
    sf::Font font;
    //todo: load font from ./ also if it fails
    font.loadFromFile("./aAreaKilometer50.ttf");
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(10);
    text.setPosition(constants::window_width - 150, 10);
    text.setString(
        "Avg Distance: " + std::to_string(average_distance) +
        "\nApp Distance: " + std::to_string(app_distance) +
        "\nError: " + std::to_string(average_distance - app_distance) +
        "\nAvg Velocity: " + std::to_string(average_velocity) +
        "\nDeviation Dist: " + std::to_string(deviation_distance) +
        "\nDeviation Vel: " + std::to_string(deviation_velocity));
    window.draw(text);
    window.display();
  }
}