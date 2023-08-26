#include "gui.hpp"

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

void Panel::insert(tgui::Widget::Ptr element, gui_element& gui_element) {
  element->setPosition(element_x_position, element_y_position);
  gui_element.set_size(element);
  element_y_position += widget_distance;
}
}  // namespace boids