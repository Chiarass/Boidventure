#include "gui.hpp"

namespace boids{
Panel::Panel(double p_slider_size, double p_button_size,
             double p_widget_distance, double p_element_x_position,
             double p_element_y_position): slider_size{p_slider_size},
    button_size{p_button_size}, widget_distance{p_widget_distance},
    element_x_position{p_element_x_position},
    element_y_position{p_element_y_position} {};

void Panel::insert(tgui::Widget::Ptr element)
{
  element->setPosition(element_x_position, element_y_position);
  element->setSize(10., 10.);
}
}