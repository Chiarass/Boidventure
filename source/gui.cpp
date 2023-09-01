#include "gui.hpp"

#include <memory>

#include "boid.hpp"
#include "constants.hpp"
#include "sfml.hpp"
namespace boids {
Panel::Panel(double p_widget_width, double p_widget_height,
             double p_widget_distance, double p_element_x_position,
             double p_element_y_position)
    : widget_width{p_widget_width},
      widget_height{p_widget_height},
      widget_distance{p_widget_distance},
      element_x_position{p_element_x_position},
      element_y_position{p_element_y_position} {};

void Panel::insert(tgui::Widget::Ptr element, Element_key key) {
  assert(element);
  element->setPosition(element_x_position, element_y_position);
  // todo: replace with constants
  element->setSize(widget_width, widget_height);
  element_y_position += widget_distance;
  elements[key] = element;
}

void initialize_panel(tgui::GuiSFML& gui, Panel& panel, bool& display_tree,
                      bool& display_range, bool& display_separation_range,
                      bool& display_prey_range) {
  tgui::Label::Ptr fps_text = tgui::Label::create();
  fps_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(fps_text);
  panel.insert(fps_text, Element_key::fps_text);

  tgui::Button::Ptr cell_button = tgui::Button::create();
  cell_button->setText("Show Cells");
  cell_button->onPress([&display_tree] { display_tree = !display_tree; });
  gui.add(cell_button);
  panel.insert(cell_button, Element_key::cell_button);

  tgui::Label::Ptr cohesion_strength_text = tgui::Label::create();
  cohesion_strength_text->setText("Cohesion Parameter");
  cohesion_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(cohesion_strength_text);
  panel.insert(cohesion_strength_text, Element_key::cohesion_strength_text);

  tgui::Slider::Ptr cohesion_strength_slider = tgui::Slider::create();
  cohesion_strength_slider->setValue(constants::init_cohesion_strength);
  gui.add(cohesion_strength_slider);
  panel.insert(cohesion_strength_slider, Element_key::cohesion_strength_slider);

  tgui::Label::Ptr alignment_strength_text = tgui::Label::create();
  alignment_strength_text->setText("Alignment Parameter");
  alignment_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(alignment_strength_text);
  panel.insert(alignment_strength_text, Element_key::alignment_strength_text);

  tgui::Slider::Ptr aligment_strength_slider = tgui::Slider::create();
  aligment_strength_slider->setValue(constants::init_alignment_strength);
  gui.add(aligment_strength_slider);
  panel.insert(aligment_strength_slider,
               Element_key::alignment_strength_slider);

  tgui::Label::Ptr separation_strength_text = tgui::Label::create();
  separation_strength_text->setText("Separation Parameter");
  separation_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(separation_strength_text);
  panel.insert(separation_strength_text, Element_key::separation_strength_text);

  tgui::Slider::Ptr separation_strength_slider = tgui::Slider::create();
  separation_strength_slider->setValue(constants::init_separation_strength);
  gui.add(separation_strength_slider);
  panel.insert(separation_strength_slider,
               Element_key::separation_strength_slider);

  tgui::Label::Ptr boid_number_text = tgui::Label::create();
  boid_number_text->setText("Number of Boids");
  boid_number_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(boid_number_text);
  panel.insert(boid_number_text, Element_key::boid_number_text);

  tgui::Slider::Ptr boid_number_slider = tgui::Slider::create();
  boid_number_slider->setMaximum(constants::max_boid_number);
  boid_number_slider->setValue(constants::init_boid_number);
  gui.add(boid_number_slider);
  panel.insert(boid_number_slider, Element_key::boid_number_slider);

  tgui::Label::Ptr range_text = tgui::Label::create();
  range_text->setText("Range");
  range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(range_text);
  panel.insert(range_text, Element_key::range_label);

  tgui::Slider::Ptr range_slider = tgui::Slider::create();
  range_slider->setValue(constants::init_range);
  gui.add(range_slider);
  panel.insert(range_slider, Element_key::range_slider);

  tgui::Button::Ptr range_button = tgui::Button::create();
  range_button->setText("Display Range");
  range_button->onPress([&display_range] { display_range = !display_range; });
  gui.add(range_button);
  panel.insert(range_button, Element_key::range_button);

  tgui::Label::Ptr separation_range_text = tgui::Label::create();
  separation_range_text->setText("Separation Range");
  separation_range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(separation_range_text);
  panel.insert(separation_range_text, Element_key::separation_range_text);

  tgui::Slider::Ptr separation_range_slider = tgui::Slider::create();
  separation_range_slider->setValue(constants::init_separation_range);
  gui.add(separation_range_slider);
  panel.insert(separation_range_slider, Element_key::separation_range_slider);

  tgui::Button::Ptr separation_range_button = tgui::Button::create();
  separation_range_button->setText("Display Separation Range");
  separation_range_button->onPress([&display_separation_range] {
    display_separation_range = !display_separation_range;
  });
  gui.add(separation_range_button);
  panel.insert(separation_range_button, Element_key::separation_range_button);

  tgui::Label::Ptr predator_number_text = tgui::Label::create();
  predator_number_text->setText("Number of Predators");
  predator_number_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(predator_number_text);
  panel.insert(predator_number_text, Element_key::predator_number_text);

  tgui::Slider::Ptr predator_number_slider = tgui::Slider::create();
  predator_number_slider->setValue(constants::init_predator_number);
  predator_number_slider->setMaximum(constants::max_predator_number);
  gui.add(predator_number_slider);
  panel.insert(predator_number_slider, Element_key::predator_number_slider);

  tgui::Label::Ptr prey_range_text = tgui::Label::create();
  prey_range_text->setText("Range of Prey");
  prey_range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(prey_range_text);
  panel.insert(prey_range_text, Element_key::prey_range_text);

  tgui::Slider::Ptr prey_range_slider = tgui::Slider::create();
  prey_range_slider->setValue(constants::init_prey_range);
  gui.add(prey_range_slider);
  panel.insert(prey_range_slider, Element_key::prey_range_slider);

  tgui::Button::Ptr separation_prey_button = tgui::Button::create();
  separation_prey_button->setText("Display Prey Range");
  separation_prey_button->onPress(
      [&display_prey_range] { display_prey_range = !display_prey_range; });
  gui.add(separation_prey_button);
  panel.insert(separation_prey_button, Element_key::separation_range_button);
};

bool update_boid_number(int& boid_number, Panel& panel) {
  if (static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
                           panel.elements[Element_key::boid_number_slider])
                           ->getValue()) != boid_number) {
    boid_number =
        static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
                             panel.elements[Element_key::boid_number_slider])
                             ->getValue());
    return true;
  }
  return false;
}

bool update_predator_number(int& predator_number, Panel& panel) {
  if (static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
                           panel.elements[Element_key::predator_number_slider])
                           ->getValue()) != predator_number) {
    predator_number = static_cast<int>(
        std::dynamic_pointer_cast<tgui::Slider>(
            panel.elements[Element_key::predator_number_slider])
            ->getValue());
    return true;
  }
  return false;
}

// todo: break into multiple functions
void update_from_panel(Panel& panel, double& fps, double& cohesion_coefficent,
                       double& alignment_coefficent,
                       double& separation_coefficent, double& range,
                       double& separation_range, double& prey_range,
                       int& boid_number, int& predator_number) {
  std::dynamic_pointer_cast<tgui::Label>(panel.elements[Element_key::fps_text])
      ->setText("fps: " + std::to_string(fps));

  // the 0.1 coefficent is needed because the max value of panel by deafult is
  // 10, so multiplyng it  0.1 rescales it to 1
  cohesion_coefficent =
      constants::max_cohesion_strength * 0.1 *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::cohesion_strength_slider])
           ->getValue());

  alignment_coefficent =
      constants::max_alignment_strength * 0.1 *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::alignment_strength_slider])
           ->getValue());

  separation_coefficent =
      constants::max_separation_strength * 0.1 *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::separation_strength_slider])
           ->getValue());

  range = constants::max_range * 0.1 *
          (std::dynamic_pointer_cast<tgui::Slider>(
               panel.elements[Element_key::range_slider])
               ->getValue());

  separation_range = constants::max_separation_range * 0.1 *
                     (std::dynamic_pointer_cast<tgui::Slider>(
                          panel.elements[Element_key::separation_range_slider])
                          ->getValue());

  prey_range = constants::max_prey_range * 0.1 *
               (std::dynamic_pointer_cast<tgui::Slider>(
                    panel.elements[Element_key::prey_range_slider])
                    ->getValue());

  // if the value of the slider is changed, change number of boids
  // if (static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
  //                          panel.elements[Element_key::boid_number_slider])
  //                          ->getValue()) != boid_number) {
  //   boid_number =
  //       static_cast<int>(std::dynamic_pointer_cast<tgui::Slider>(
  //                            panel.elements[Element_key::boid_number_slider])
  //                            ->getValue());
  //   initialize_birds(boid_vector, boid_vertex, boid_number,
  //                    constants::boid_color);
  // }

  std::dynamic_pointer_cast<tgui::Label>(
      panel.elements[Element_key::boid_number_text])
      ->setText("Number of Boids: " + std::to_string(boid_number));
  std::dynamic_pointer_cast<tgui::Label>(
      panel.elements[Element_key::predator_number_text])
      ->setText("Number of Predators: " + std::to_string(predator_number));
}

void display_ranges(double range, double separation_range, double prey_range,
                    bool display_range, bool display_separation_range,
                    bool display_prey_range, std::vector<Boid>& boid_vector,
                    sf::RenderWindow& window) {
  // if corresponding button is pressed, displays the ranges of the first boid
  // in the vector;
  if (!boid_vector.empty()) {
    if (display_range)
      display_circle(window, range, boid_vector[0], constants::range_color);

    if (display_separation_range)
      display_circle(window, separation_range, boid_vector[0],
                     constants::separation_range_color);

    if (display_prey_range)
      display_circle(window, prey_range, boid_vector[0],
                     constants::prey_range_color);
  }
}
}  // namespace boids