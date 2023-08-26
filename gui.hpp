#ifndef GUI_HPP_839
#define GUI_HPP_839

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <map>
#include <string>

namespace boids {
class gui_element {
 private:
  struct {
    double x;
    double y;
  } position;

  struct {
    double x;
    double y;
  } size;

 public:
  gui_element(double x, double y, double width, double height);

  void set_size(tgui::Widget::Ptr element);
};
class slider_element : public gui_element {
 public:
  slider_element(double x, double y);
};

class button_element : public gui_element {
 public:
  button_element(double x, double y);
};

class label_element : public gui_element {
 public:
  label_element(double x, double y);
};

struct Panel {
  // todo: is such a pointer implementation safe? does it work?
  // todo: replace std::string with enum class?
  // todo: maybe make a class?
  std::map<std::string, tgui::Widget::Ptr> elements;
  double slider_size{};
  double button_size{};
  double widget_distance{};
  double element_x_position{};
  double element_y_position{};

  Panel(double, double, double, double, double);

  void insert(tgui::Widget::Ptr, gui_element&);
};

}  // namespace boids

#endif