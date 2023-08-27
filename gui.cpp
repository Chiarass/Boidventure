#include "gui.hpp"

#include <memory>

#include "constants.hpp"
namespace boids {
Panel::Panel(double p_slider_size, double p_button_size,
             double p_widget_distance, double p_element_x_position,
             double p_element_y_position)
    : slider_size{p_slider_size},
      button_size{p_button_size},
      widget_distance{p_widget_distance},
      element_x_position{p_element_x_position},
      element_y_position{p_element_y_position} {};

void Panel::insert(tgui::Widget::Ptr element, Element_key key) {
  assert(element);
  element->setPosition(element_x_position, element_y_position);
  element->setSize(200., 25.);
  element_y_position += widget_distance;
  elements[key] = element;
}

void initialize_panel(tgui::GuiSFML& gui, Panel& panel, bool& display_tree,
                      bool& display_range, bool& display_separation_range,
                      bool& display_prey_range) {
  tgui::Label::Ptr fps_text = tgui::Label::create();  // Set the text to display
  fps_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(fps_text);
  panel.insert(fps_text, Element_key::fps_text);

  tgui::Button::Ptr cell_button = tgui::Button::create();
  cell_button->setText("Show Cells");
  cell_button->onPress([&display_tree] { display_tree = !display_tree; });
  gui.add(cell_button);
  panel.insert(cell_button, Element_key::cell_button);

  tgui::Label::Ptr cohesion_strength_text = tgui::Label::create();
  cohesion_strength_text->setText(
      "Cohesion parameter");  // Set the text to display
  cohesion_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(cohesion_strength_text);
  panel.insert(cohesion_strength_text, Element_key::cohesion_strength_text);

  tgui::Slider::Ptr cohesion_slider = tgui::Slider::create();
  gui.add(cohesion_slider);
  panel.insert(cohesion_slider, Element_key::cohesion_strength_slider);

  tgui::Label::Ptr alignment_strength_text = tgui::Label::create();
  alignment_strength_text->setText(
      "Alignment parameter");  // Set the text to display
  alignment_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(alignment_strength_text);
  panel.insert(alignment_strength_text, Element_key::alignment_strength_text);

  tgui::Slider::Ptr aligment_slider = tgui::Slider::create();
  gui.add(aligment_slider);
  panel.insert(aligment_slider, Element_key::alignment_strength_slider);

  tgui::Label::Ptr separation_strength_text = tgui::Label::create();
  separation_strength_text->setText(
      "Separation parameter");  // Set the text to display
  separation_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(separation_strength_text);
  panel.insert(separation_strength_text, Element_key::separation_strength_text);

  tgui::Slider::Ptr separation_slider = tgui::Slider::create();
  gui.add(separation_slider);
  panel.insert(separation_slider, Element_key::separation_strength_slider);

  tgui::Label::Ptr boid_number_text = tgui::Label::create();
  boid_number_text->setText("Number of boids");  // Set the text to display
  boid_number_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(boid_number_text);
  panel.insert(boid_number_text, Element_key::boid_number_text);

  tgui::Slider::Ptr boid_number_slider = tgui::Slider::create();
  boid_number_slider->setMaximum(constants::max_boid_number);
  gui.add(boid_number_slider);
  panel.insert(boid_number_slider, Element_key::boid_number_slider);

  tgui::Label::Ptr range_text = tgui::Label::create();
  range_text->setText("Range");  // Set the text to display
  range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(range_text);
  panel.insert(range_text, Element_key::range_label);

  tgui::Slider::Ptr range_slider = tgui::Slider::create();
  gui.add(range_slider);
  panel.insert(range_slider, Element_key::range_slider);

  tgui::Button::Ptr range_button = tgui::Button::create();
  range_button->setText("Display Range");
  range_button->onPress([&display_range] { display_range = !display_range; });
  gui.add(range_button);
  panel.insert(range_button, Element_key::range_button);

  tgui::Label::Ptr separation_range_text = tgui::Label::create();
  separation_range_text->setText(
      "separation range");  // Set the text to display
  separation_range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(separation_range_text);
  panel.insert(separation_range_text, Element_key::separation_range_text);

  tgui::Slider::Ptr separation_range_slider = tgui::Slider::create();
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
  predator_number_text->setText("predator number");  // Set the text to display
  predator_number_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(predator_number_text);
  panel.insert(predator_number_text, Element_key::predator_number_text);

  tgui::Slider::Ptr predator_number_slider = tgui::Slider::create();
  gui.add(predator_number_slider);
  panel.insert(predator_number_slider, Element_key::predator_number_slider);

  tgui::Label::Ptr prey_range_text = tgui::Label::create();
  prey_range_text->setText("prey range");  // Set the text to display
  prey_range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(prey_range_text);
  panel.insert(prey_range_text, Element_key::prey_range_text);

  tgui::Slider::Ptr prey_range_slider = tgui::Slider::create();
  gui.add(prey_range_slider);
  panel.insert(prey_range_slider, Element_key::prey_range_slider);

  tgui::Button::Ptr separation_prey_button = tgui::Button::create();
  separation_prey_button->setText("Display Prey Range");
  separation_prey_button->onPress(
      [&display_prey_range] { display_prey_range = !display_prey_range; });
  gui.add(separation_prey_button);
  panel.insert(separation_prey_button, Element_key::separation_range_button);
};

void update_from_panel(Panel& panel, double& fps, double& cohesion_coefficent,
                       double& alignment_coefficent,
                       double& separation_coefficent, double& range,
                       double& separation_range, double& prey_range) {
  std::dynamic_pointer_cast<tgui::Label>(panel.elements[Element_key::fps_text])
      ->setText("fps: " + std::to_string(fps));

  // todo: is it really the max value?
  cohesion_coefficent =
      constants::max_cohesion_strength *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::cohesion_strength_slider])
           ->getValue());
  alignment_coefficent =
      constants::max_alignment_strength *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::alignment_strength_slider])
           ->getValue());
  separation_coefficent =
      constants::max_separation_strength *
      (std::dynamic_pointer_cast<tgui::Slider>(
           panel.elements[Element_key::separation_strength_slider])
           ->getValue());
  range = constants::max_range * (std::dynamic_pointer_cast<tgui::Slider>(
                                      panel.elements[Element_key::range_slider])
                                      ->getValue());
  separation_range = constants::max_separation_range *
                     (std::dynamic_pointer_cast<tgui::Slider>(
                          panel.elements[Element_key::separation_range_slider])
                          ->getValue());
  prey_range = constants::max_prey_range *
               (std::dynamic_pointer_cast<tgui::Slider>(
                    panel.elements[Element_key::prey_range_slider])
                    ->getValue());
}
}  // namespace boids