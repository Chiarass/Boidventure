#ifndef GUI_HPP_839
#define GUI_HPP_839

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <map>
#include <string>

#include "boid.hpp"
#include "sfml.hpp"

// enum class, for panel map attribute
enum class widget_key {
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

// handles positions of gui widgets
class Panel {
 private:
  double widget_width{};
  double widget_height{};
  double widget_distance{};
  double widget_x_position{};
  double widget_y_position{};

 public:
  std::map<widget_key, tgui::Widget::Ptr> widgets;

  // panel constructor
  // Param 1: width of widgets
  // Param 2: height of widgetss
  // Param 3: distance between widgets
  // Param 4: Position of first widget on x axis
  // Param 5: Position of first widget on y axis
  Panel(double, double, double, double, double);

  // adds a widget.
  // Param 1: the widget
  // Param 2: enum class object, to retrieve widget from map
  void insert(tgui::Widget::Ptr, widget_key);

  // template method. retrievies pointer to widget object. usese polymorphism
  //  Param 1: widget key
  template <class T>
  std::shared_ptr<T> retrieve(widget_key key) {
    // pointer is not null
    assert(this->widgets[key]);

    return std::dynamic_pointer_cast<T>(this->widgets[key]);
  };
};

// initializes the panel with the various GUI objects
// Param 1: gui, to which widgets get added
// Param 2: panel, where widgets get inserted
// Param 3: bool for quad tree button
// Param 4: bool for range button
// Param 5: bool for separation range button
// Param 6: bool for prey range button
void initialize_panel(tgui::GuiSFML&, Panel&, bool&, bool&, bool&, bool&);

// updates the number of boids from the slider position. returns false if no
// update was necessary, true otherwise
// Param 1: number of boids.
// Param 2: panel where slider is inserted
bool update_boid_number(int&, Panel& panel);

// updates the number of predators from the slider position. returns false if no
// update was necessary, true otherwise
// Param 1: number of predators.
// Param 2: panel where slider is inserted
bool update_predator_number(int&, Panel& panel);

// updates the various values from the slider positions.
//  Param 1: panel where sliders are inserted
//  Param 2: fps value
//  Param 3: coehesion coefficent
//  Param 4: alignment coefficent
//  Param 5: separation coefficent
//  Param 6: range value
//  Param 7: separation range value
//  Param 8: prey range value
void update_from_panel(Panel&, double&, double&, double&, double&, double&,
                       double&, double&);

// displayes the ranges if the buttons are pushed
//  Param 1 range value
//  Param 2 separation range value
//  Param 3 prey range value
//  Param 4 is range displayed
//  Param 5 is separation range displayed
//  Param 6 is prey range displayed
//  Param 7 vector of boids
//  Param 8 window object
void display_ranges(double, double, double, bool, bool, bool,
                    std::vector<Boid>&, sf::RenderWindow&);

}  // namespace boids
#endif