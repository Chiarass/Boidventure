#include "gui.hpp"
#include "constants.hpp"
namespace boids {
gui_element::gui_element(double x, double y, double width, double height)
    : position{x, y}, size{width, height} {}

void gui_element::set_size(tgui::Widget::Ptr element) {
  element->setSize(size.x, size.y);
}

slider_element::slider_element(double x, double y)
    : gui_element(x, y, 200, 20) {}

button_element::button_element(double x, double y)
    : gui_element(x, y, 150, 30) {}

label_element::label_element(double x, double y) : gui_element(x, y, 150, 30){};

Panel::Panel(double p_slider_size, double p_button_size,
             double p_widget_distance, double p_element_x_position,
             double p_element_y_position)
    : slider_size{p_slider_size},
      button_size{p_button_size},
      widget_distance{p_widget_distance},
      element_x_position{p_element_x_position},
      element_y_position{p_element_y_position} {};

void Panel::insert(tgui::Widget::Ptr element, gui_element& gui_element, Element_key key) {
  assert(element);
  element->setPosition(element_x_position, element_y_position);
  element->setSize(200., 25.);
  //gui_element->setSize(element);
  element_y_position += widget_distance;
  elements[key] = element;
}

void initialize_panel(tgui::GuiSFML& gui, Panel& panel, bool& display_tree){
  boids::button_element button1(10., 20.);
  tgui::Button::Ptr cell_button = tgui::Button::create();
  cell_button->setText("Show Cells");
  cell_button->onPress([&display_tree] { display_tree = !display_tree; });
  gui.add(cell_button);
  panel.insert(cell_button, button1, Element_key::cell_button);

  // boids::button_element button2(10., 20.);
  // tgui::Button::Ptr range_button = tgui::Button::create();
  // range_button->setText("Display Range");
  // gui.add(range_button);
  // panel.insert(range_button, button2, "button2");

  // boids::button_element button3(10., 20.);
  // tgui::Button::Ptr separation_range_button = tgui::Button::create();
  // separation_range_button->setText("Display Separation Range");
  // gui.add(separation_range_button);
  // panel.insert(separation_range_button, button3, "button3");

  boids::label_element label1(0., 0.);
  tgui::Label::Ptr cohesion_strength_text = tgui::Label::create();
  cohesion_strength_text->setText(
      "Cohesion parameter");  // Set the text to display
  cohesion_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(cohesion_strength_text);
  panel.insert(cohesion_strength_text, label1, Element_key::cohesion_strength_text);

  boids::slider_element slider1(0., 0.);
  tgui::Slider::Ptr cohesion_slider = tgui::Slider::create();
  gui.add(cohesion_slider);
  panel.insert(cohesion_slider, slider1, Element_key::cohesion_strength_slider);

  boids::label_element label2(20., 40.);
  tgui::Label::Ptr alignment_strength_text = tgui::Label::create();
  alignment_strength_text->setText(
      "Alignment parameter");  // Set the text to display
  alignment_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(alignment_strength_text);
  panel.insert(alignment_strength_text, label2, Element_key::alignment_strength_text);

  boids::slider_element slider2(20., 40.);
  tgui::Slider::Ptr aligment_slider = tgui::Slider::create();
  gui.add(aligment_slider);
  panel.insert(aligment_slider, slider2, Element_key::alignment_strength_slider);

  boids::label_element label3(20., 40.);
  tgui::Label::Ptr separation_strength_text = tgui::Label::create();
  separation_strength_text->setText(
      "Separation parameter");  // Set the text to display
  separation_strength_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(separation_strength_text);
  panel.insert(separation_strength_text, label3, Element_key::separation_strength_text);

  boids::slider_element slider3(20., 40.);
  tgui::Slider::Ptr separation_slider = tgui::Slider::create();
  gui.add(separation_slider);
  panel.insert(separation_slider, slider3, Element_key::separation_strength_slider);

  boids::label_element label4(20., 40.);
  tgui::Label::Ptr boid_number_text = tgui::Label::create();
  boid_number_text->setText("Number of boids");  // Set the text to display
  boid_number_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(boid_number_text);
  panel.insert(boid_number_text, label4, Element_key::boid_number_text);
  
  boids::slider_element slider4(20., 40.);
  tgui::Slider::Ptr boid_number_slider = tgui::Slider::create();
  boid_number_slider -> setMaximum(constants::max_boid_number);
  gui.add(boid_number_slider);
  panel.insert(boid_number_slider, slider4, Element_key::boid_number_slider);
  

  boids::label_element label5(20., 40.);
  tgui::Label::Ptr range_text = tgui::Label::create();
  range_text->setText("Range");  // Set the text to display
  range_text->getRenderer()->setTextColor(sf::Color::White);
  gui.add(range_text);
  panel.insert(range_text, label5, Element_key::range_label);

  boids::slider_element slider5(20., 40.);
  tgui::Slider::Ptr range_slider = tgui::Slider::create();
  gui.add(range_slider);
  panel.insert(range_slider, slider5, Element_key::range_slider);

  // boids::slider_element slider6(20., 40.);
  // tgui::Slider::Ptr predator_number_slider = tgui::Slider::create();
  // gui.add(predator_number_slider);
  // panel.insert(predator_number_slider, slider6);

  // boids::slider_element slider7(20., 40.);
  // tgui::Slider::Ptr separation_range_slider = tgui::Slider::create();
  // gui.add(separation_range_slider);
  // panel.insert(separation_range_slider, slider7);

  // boids::slider_element slider8(20., 40.);
  // tgui::Slider::Ptr prey_range_slider = tgui::Slider::create();
  // gui.add(prey_range_slider);
  // panel.insert(prey_range_slider, slider8);
};
}  // namespace boids