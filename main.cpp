#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <random>

#include "boid.hpp"
#include "constants.hpp"
#include "quadtree.hpp"

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

int main() {
  std::vector<boids::Boid> boid_vector;
  boids::Quad_tree tree{constants::cell_capacity,
                        boids::Rectangle{constants::window_width / 2.,
                                         constants::window_height / 2.,
                                         constants::window_width / 2.,
                                         constants::window_height / 2.}};
  sf::VertexArray swarm_vertex{
      sf::Triangles, static_cast<size_t>(3 * constants::swarm_number)};

  // for loop fills boid vector
  for (int i = 0; i < constants::swarm_number; ++i) {
    // initalizing random boid
    // the parameters in uniform set the maximum and minimum
    // coordinates the position/velocity can be generated with
    // the minimum in boid_position is margin_width because we
    // want the boid to be generated within that distance
    // from the edge of the window, and the upper left corner
    // of the window has position (0,0).
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
    boid_vector.push_back(boids::Boid{boid_position, boid_velocity});

    swarm_vertex[i * 3].color = constants::boid_color;
    swarm_vertex[i * 3 + 1].color = constants::boid_color;
    swarm_vertex[i * 3 + 2].color = constants::boid_color;
  }

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

      if (constants::swarm_number != number_slider->getValue()) {
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
    window.display();
  }
}