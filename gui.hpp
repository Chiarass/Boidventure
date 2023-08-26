#ifndef GUI_HPP_839
#define GUI_HPP_839

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <map>
#include <string>

enum class Element_key {
  fps_text,

  cell_button,

  boid_number_text,
  boid_number_slider,

  cohesion_strength_text,
  cohesion_strength_slider,

  alignment_strength_text,
  alignment_strength_slider,

  separation_strength_text,
  separation_strength_slider,

  range_label,
  range_slider,
  range_button,

  predator_number_text,
  predator_number_slider,

  separation_range_text,
  separation_range_slider,
  separation_range_button,

  prey_range_text,
  prey_range_slider,
};

namespace boids {
struct Panel {
  // todo: is such a pointer implementation safe? does it work?
  // todo: replace std::string with enum class?
  // todo: maybe make a class?
  std::map<Element_key, tgui::Widget::Ptr> elements;
  double slider_size{};
  double button_size{};
  double widget_distance{};
  double element_x_position{};
  double element_y_position{};

  Panel(double, double, double, double, double);

  void insert(tgui::Widget::Ptr, Element_key);
};

void initialize_panel(tgui::GuiSFML&, Panel&, bool&, bool&, bool&, bool&);

}  // namespace boids

#endif