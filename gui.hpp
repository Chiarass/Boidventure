#ifndef GUI_HPP_839
#define GUI_HPP_839

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>


namespace boids {
    struct Panel{
        //todo: is such a pointer implementation safe? does it work?
        //todo: replace std::string with enum class?
        //todo: maybe make a class?
        std::map<std::string, tgui::Widget::Ptr> elements;
        double slider_size{};
        double button_size{};
        double widget_distance{};
        double element_x_position{};
        double element_y_position{};

        Panel(double, double, double, double, double);

        void insert(tgui::Widget::Ptr);
    };
}
#endif