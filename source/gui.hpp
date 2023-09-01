#ifndef GUI_HPP_839
#define GUI_HPP_839

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <map>
#include <string>

#include "boid.hpp"
#include "sfml.hpp"

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
class Panel {
 private:
  double widget_width{};
  double widget_height{};
  double widget_distance{};
  double element_x_position{};
  double element_y_position{};

 public:
  // todo: is such a pointer implementation safe? when does it get deleted?
  std::map<Element_key, tgui::Widget::Ptr> elements;

  Panel(double, double, double, double, double);

  void insert(tgui::Widget::Ptr, Element_key);
};

void initialize_panel(tgui::GuiSFML&, Panel&, bool&, bool&, bool&, bool&);

bool update_boid_number(int&, Panel& panel);

bool update_predator_number(int&, Panel& panel);

void update_from_panel(Panel&, double&, double&, double&, double&, double&,
                       double&, double&, int&, int&);
void display_ranges(double, double, double, bool, bool, bool,
                      std::vector<Boid>&, sf::RenderWindow&);

}  // namespace boids

#endif