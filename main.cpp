#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <random>

#include "boid.hpp"
#include "constants.hpp"
#include "quadtree.hpp"
#include "statistics.hpp"

// random generator of values from uniform distribution, for randomly
// generating position of boids
// is it trully distributed as a uniform? redefining
// marsenne twister seems to reset
// pseudo random number generator

// to change
inline std::mt19937 eng{};

namespace boids {

double uniform(double a, double b) {
  // commented out for debugging
  // std::random_device rd;
  std::uniform_real_distribution<double> unif{a, b};
  return unif(eng);
}

// updates the triangle object associated with the passed boid. It
// moves it to the same poistion on the screen as the boid
// and rotates it toward the direction of motion
void vertex_update(sf::VertexArray& swarm_vertex, const Boid& boid, int index) {
  // should convert implicitly to double anyway, so no
  // need to static cast it i think.
  Point forward_vertex{0., 0.};
  if ((boid.vel()).distance() != 0)
    forward_vertex =
        (constants::boid_size / boid.vel().distance()) * (boid.vel());

  swarm_vertex[3 * index].position =
      // i have added the *2 so the front is longer,
      // and we can distinguish it.
      sf::Vector2f((boid.pos() + 2 * forward_vertex).x(),
                   (boid.pos() + 2 * forward_vertex).y());

  // rotate by 120
  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 1].position = sf::Vector2f(
      (boid.pos() + forward_vertex).x(), (boid.pos() + forward_vertex).y());

  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 2].position = sf::Vector2f(
      (boid.pos() + forward_vertex).x(), (boid.pos() + forward_vertex).y());
}
}  // namespace boids

// method to calculate standard deviation (to be moved elsewhere in a hpp)
/*double calculate_standard_deviation(const std::vector<double>& data,
                                    double average) {
  double sum_squared_differences = 0.0;
  for (double value : data) {
    double difference = value - average;
    sum_squared_differences += difference * difference;
  }
  double variance = sum_squared_differences / data.size();
  return std::sqrt(variance);
};*/

int main() {
  std::vector<boids::Boid> boid_vector;
  boids::Quad_tree tree{constants::cell_capacity,
                        boids::Rectangle{constants::window_width / 2.,
                                         constants::window_height / 2.,
                                         constants::window_width / 2.,
                                         constants::window_height / 2.}};
  sf::VertexArray swarm_vertex{
      sf::Triangles, static_cast<size_t>(3 * constants::swarm_number)};

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
  cohesion_slider->setValue(1);
  gui.add(cohesion_slider);

  // slider for alignment
  tgui::Slider::Ptr alignment_slider = tgui::Slider::create();
  alignment_slider->setPosition(10., 60.);
  alignment_slider->setValue(2);
  gui.add(alignment_slider);

  // slider for separation
  tgui::Slider::Ptr separation_slider = tgui::Slider::create();
  separation_slider->setPosition(10., 80.);
  separation_slider->setValue(3);
  gui.add(separation_slider);

  // slider for number of boids
  tgui::Slider::Ptr number_slider = tgui::Slider::create();
  number_slider->setPosition(10., 100.);
  number_slider->setMinimum(1);
  // to replace with constant
  number_slider->setMaximum(1500);
  number_slider->setValue(constants::swarm_number);
  gui.add(number_slider);

  bool lock_click{false};

  // create 4 vectors to store distances, velocity, average distances and
  // velocities
  std::vector<double> distances;
  std::vector<double> velocities;
  std::vector<double> average_distances;
  std::vector<double> average_velocities;

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
      if (number_slider->getValue() != constants::swarm_number) {
        constants::swarm_number = static_cast<int>(number_slider->getValue());
        boid_vector.clear();
        swarm_vertex.clear();
        for (int i = 0; i < constants::swarm_number; ++i) {
          auto boid_position = boids::Point{
              boids::uniform(constants::margin_width,
                             constants::window_width - constants::margin_width),
              boids::uniform(
                  constants::margin_width,
                  constants::window_height - constants::margin_width)};
          auto boid_velocity =
              boids::Point{boids::uniform(constants::min_rand_velocity,
                                          constants::max_rand_velocity),
                           boids::uniform(constants::min_rand_velocity,
                                          constants::max_rand_velocity)};
          boid_vector.push_back(boids::Boid{boid_position, boid_velocity});

          // Append each vertex to the swarm_vertex array
          sf::Vertex v1(sf::Vector2f(boid_position.x(), boid_position.y()),
                        constants::boid_color);
          sf::Vertex v2(sf::Vector2f(boid_position.x(), boid_position.y()),
                        constants::boid_color);
          sf::Vertex v3(sf::Vector2f(boid_position.x(), boid_position.y()),
                        constants::boid_color);
          swarm_vertex.append(v1);
          swarm_vertex.append(v2);
          swarm_vertex.append(v3);
        }
      }
    }
    distances.clear();
    velocities.clear();
    // Calculating stats about the flock
    double total_distance = 0.0;
    double total_velocity = 0.0;
    for (const auto& boid : boid_vector) {
      double boid_distance = 0.0;
      for (const auto& other_boid : boid_vector) {
        boid_distance += (boid.pos() - other_boid.pos()).distance();
      }
      total_distance += boid_distance;
      total_velocity += boid.vel().distance();
      // Aggiungi boidDistance e boidVelocity ai vettori distances e velocities
      distances.push_back(boid_distance);
      velocities.push_back(boid.vel().distance());
    }

    // Calculate average distances and velocities
    total_distance = std::accumulate(distances.begin(), distances.end(), 0.0);
    total_velocity = std::accumulate(velocities.begin(), velocities.end(), 0.0);

    double average_distance =
        total_distance / (boid_vector.size() * boid_vector.size());
    double average_velocity = total_velocity / boid_vector.size();

    // Calculate deviation distances and velocities
    double deviation_distance =
        calculate_standard_deviation(distances, average_distance);
    double deviation_velocity =
        calculate_standard_deviation(velocities, average_velocity);

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
      boid_vector[i].update(constants::delta_t, in_range);
      boids::vertex_update(swarm_vertex, boid_vector[i], i);
    }

    constants::cohesion_coefficent = 0.1 * (cohesion_slider->getValue());
    constants::alignment_coefficent = 0.1 * (alignment_slider->getValue());
    constants::separation_coefficent = 0.1 * (separation_slider->getValue());
    if (display_tree) tree.display(window);
    tree.delete_tree();
    window.draw(swarm_vertex);
    gui.draw();
    // displaying all the stats
    sf::Font font;
    font.loadFromFile("aAreaKilometer50.ttf");
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(10);
    text.setPosition(constants::window_width - 150, 10);
    text.setString("Avg Distance: " + std::to_string(average_distance) +
                   "\nAvg Velocity: " + std::to_string(average_velocity) +
                   "\nDeviation Dist: " + std::to_string(deviation_distance) +
                   "\nDeviation Vel: " + std::to_string(deviation_velocity));
    window.draw(text);
    window.display();
  }
}