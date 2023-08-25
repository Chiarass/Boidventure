// i used a more specific name than SFML_HPP because it may not be
// so rare for files to be called SFML
#ifndef SFML_HPP_118
#define SFML_HPP_118
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

// todo: check if implementation of template is correct
namespace boids {
template <class Bird_type>
void vertex_update(sf::VertexArray& swarm_vertex, const Bird_type& bird,
                   int index, double size) {
  // should convert implicitly to double anyway, so no
  // need to static cast it i think.
  Point forward_vertex{0., 0.};
  if ((bird.vel()).distance() != 0)
    forward_vertex = (size / bird.vel().distance()) * (bird.vel());

  swarm_vertex[3 * index].position =
      // i have added the *2 so the front is longer,
      // and we can distinguish it.
      sf::Vector2f((bird.pos() + 2 * forward_vertex).x(),
                   (bird.pos() + 2 * forward_vertex).y());

  // rotate by 120
  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 1].position = sf::Vector2f(
      (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());

  forward_vertex.rotate(2. / 3 * constants::pi);

  swarm_vertex[(3 * index) + 2].position = sf::Vector2f(
      (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());
}

// struct Panel_element{
// tgui::Slider* slider;
// tgui::Text* text;
// tgui::Button* button;

// //todo: is this type of overloading ok?
// Panel_element(std::string text_string, double slider_size, sf::Font font){
//     if(slider_size > 0){
//         slider = new tgui::Slider::create();
//     }
// }
// };

// struct Panel{
//     std::map<std::string, Panel_element> elements;
//     double slider_size;
//     double text_size;
//     double element_spacing;
//     double first_element_pos;

//     //todo: move to sfml.cpp
//     Panel(double slider_s,double text_s,double element_s, double f_element_p): slider_size{slider_s}, text_size{text_s}, element_spacing{element_s}, first_element_pos{f_element_p} {};

//     //todo: is there a better way than passing by value?
//     void insert_element(std::string key, Panel_element element){
//         //todo: is this the right way of using map?
//         elements[key] = element;
//     }
// };

//todo: move to sfml.cpp
void initialize_gui(tgui::GuiSFML& gui, tgui::Button::Ptr& cell_button, tgui::Button::Ptr& range_button, tgui::Button::Ptr& separation_range_button, tgui::Slider::Ptr& boid_number_slider, tgui::Slider::Ptr& boid_number_text){
  
  bool display_tree{false};
  bool display_range{false};
  bool display_separation_range{false};

  // button for quad tree
  tgui::Button::Ptr cell_button = tgui::Button::create();
  cell_button->setPosition(10., 10.);
  cell_button->setSize(70., 20.);
  cell_button->setText("show cells");
  cell_button->onPress([&display_tree] { display_tree = !display_tree; });
  gui.add(cell_button);

  // button for range
  tgui::Button::Ptr range_button = tgui::Button::create();
  range_button->setPosition(80., 10.);
  range_button->setSize(70., 20.);
  range_button->setText("show cells");
  range_button->onPress([&display_range] { display_range = !display_range; });
  gui.add(range_button);

  // button for separation_range
  tgui::Button::Ptr separation_range_button = tgui::Button::create();
  separation_range_button->setPosition(160., 10.);
  separation_range_button->setSize(70., 20.);
  separation_range_button->setText("show cells");
  separation_range_button->onPress([&display_separation_range] {
    display_separation_range = !display_separation_range;
  });
  gui.add(separation_range_button);

  // boid number //////////////////////////////////////////////
  tgui::Slider::Ptr boid_number_slider = tgui::Slider::create();
  boid_number_slider->setPosition(10., 100.);
  boid_number_slider->setMinimum(1);
  // todo: replace with constant
  boid_number_slider->setMaximum(2500);
  boid_number_slider->setValue(constants::init_boid_number);
  gui.add(boid_number_slider);

  sf::Text boid_number_text;
  boid_number_text.setFont(font);
  boid_number_text.setFillColor(sf::Color::White);
  boid_number_text.setCharacterSize(20);
  boid_number_text.setPosition(10., 90.);
  /////////////////////////////////////////////////////////////

  // cohesion /////////////////////////////////////////////////
  tgui::Slider::Ptr cohesion_slider = tgui::Slider::create();
  cohesion_slider->setPosition(10., 40.);
  cohesion_slider->setValue(constants::init_cohesion_coeff);
  gui.add(cohesion_slider);

  sf::Text cohesion_strength_text;
  cohesion_strength_text.setFont(font);
  cohesion_strength_text.setFillColor(sf::Color::White);
  cohesion_strength_text.setCharacterSize(20);
  cohesion_strength_text.setPosition(10., 90.);
  /////////////////////////////////////////////////////////////

  // alignment ////////////////////////////////////////////////
  tgui::Slider::Ptr alignment_slider = tgui::Slider::create();
  alignment_slider->setPosition(10., 60.);
  alignment_slider->setValue(constants::init_alignment_coeff);
  gui.add(alignment_slider);

  sf::Text alignment_strength_text;
  alignment_strength_text.setFont(font);
  alignment_strength_text.setFillColor(sf::Color::White);
  alignment_strength_text.setCharacterSize(20);
  alignment_strength_text.setPosition(10., 90.);
  /////////////////////////////////////////////////////////////

  // range ////////////////////////////////////////////////////
  tgui::Slider::Ptr range_slider = tgui::Slider::create();
  range_slider->setPosition(10., 100.);
  range_slider->setValue(constants::init_range);
  gui.add(range_slider);

  sf::Text range_text;
  range_text.setFont(font);
  range_text.setFillColor(sf::Color::White);
  range_text.setCharacterSize(20);
  range_text.setPosition(10., 90.);
  /////////////////////////////////////////////////////////////

  // predator number //////////////////////////////////////////
  tgui::Slider::Ptr predator_number_slider = tgui::Slider::create();
  predator_number_slider->setPosition(10., 120.);
  predator_number_slider->setMinimum(0);
  // todo: replace with constant
  predator_number_slider->setMaximum(10);
  predator_number_slider->setValue(constants::init_predator_number);
  gui.add(predator_number_slider);
  /////////////////////////////////////////////////////////////

  // separation ///////////////////////////////////////////////
  tgui::Slider::Ptr separation_slider = tgui::Slider::create();
  separation_slider->setPosition(10., 80.);
  separation_slider->setValue(constants::init_separation_coeff);
  gui.add(separation_slider);


  sf::Text separation_text;
  separation_text.setFont(font);
  separation_text.setFillColor(sf::Color::White);
  separation_text.setCharacterSize(20);
  separation_text.setPosition(10., 90.);
  /////////////////////////////////////////////////////////////

  // distance sliders /////////////////////////////////////////
  // range slider

  // slider for separation range
  tgui::Slider::Ptr separation_range_slider = tgui::Slider::create();
  separation_range_slider->setPosition(10., 120.);
  separation_range_slider->setValue(constants::init_separation_range);
  gui.add(separation_range_slider);

  // slider for predator detection range
  // pray range ///////////////////////////////////////////////
  tgui::Slider::Ptr prey_range_slider = tgui::Slider::create();
  prey_range_slider->setPosition(10., 140.);
  prey_range_slider->setValue(constants::init_prey_range);
  gui.add(prey_range_slider);
  /////////////////////////////////////////////////////////////

  // speed sliders ////////////////////////////////////////////
  //  slider for boid speed
  tgui::Slider::Ptr boid_speed_slider = tgui::Slider::create();
  boid_speed_slider->setPosition(10., 40.);
  // todo: add time boid constants
  boid_speed_slider->setValue(constants::init_cohesion_coeff);
  gui.add(boid_speed_slider);

  // slider for predator speed
  tgui::Slider::Ptr predator_speed_slider = tgui::Slider::create();
  predator_speed_slider->setPosition(10., 60.);
  // todo: add time predator constants
  predator_speed_slider->setValue(constants::init_alignment_coeff);
  gui.add(predator_speed_slider);
  /////////////////////////////////////////////////////////////
}

}  // namespace boids
#endif