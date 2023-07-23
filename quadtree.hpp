// implementation of space partitioning by using quad tree.
#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>
#include <vector>

#include "point.hpp"
#include "boid.hpp"

namespace boids {
struct Rectangle {
  // position of center of rectangle
  double x{};
  double y{};
  // they represent the distance from the center to the
  // left/right and upper/lower sides respectively
  double w{};
  double h{};

  // needed to check if boid is contained in quad-tree cell
  bool contains(const Point& p) {
    return (p.x() >= x - w && p.x() <= x + w && p.y() <= y + h &&
            p.y() >= y - h);
  }
};

class Quad_tree {
  const int m_capacity{};
  Rectangle m_boundary{};
  bool m_divided = false;

 public:
  std::vector<Boid*> boids;
  // children of quad tree. Dynamically allocated.
  Quad_tree* northeast;
  Quad_tree* northwest;
  Quad_tree* southeast;
  Quad_tree* southwest;

  Quad_tree(int capacity, const Rectangle& boundary)
      : m_capacity{capacity}, m_boundary{boundary} {};

  void subdivide() {
    // ne stands for north east
    // the division by two happens because w and h represent half
    // of the width and height respectively. the center of child
    // quad tree is displace  one fourth of the width and height
    // from the mother.
    Rectangle ne = Rectangle{m_boundary.x + m_boundary.w / 2.,
                             m_boundary.y + m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    northeast = new Quad_tree{m_capacity, ne};

    Rectangle nw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                             m_boundary.y + m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    northwest = new Quad_tree{m_capacity, nw};

    Rectangle se = Rectangle{m_boundary.x + m_boundary.w / 2.,
                             m_boundary.y - m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    southeast = new Quad_tree{m_capacity, se};

    Rectangle sw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                             m_boundary.y - m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    southwest = new Quad_tree{m_capacity, sw};
    m_divided = true;
  }

  void insert(Boid* boid_ptr) {
    // what happens if passed null pointer?
    if (boid_ptr && m_boundary.contains(boid_ptr->pos())) {
      if ((static_cast<int>(boids.size()) < m_capacity && !m_divided)) {
        boids.push_back(boid_ptr);
      } else {

        if (!m_divided) {
          subdivide();
          for (auto& inserted_boids : boids) {
            northeast->insert(inserted_boids);
            northwest->insert(inserted_boids);
            southeast->insert(inserted_boids);
            southwest->insert(inserted_boids);
          }
          boids.clear();
        }
        northeast->insert(boid_ptr);
        northwest->insert(boid_ptr);
        southeast->insert(boid_ptr);
        southwest->insert(boid_ptr);
      }
    }
  }

  void display(sf::RenderWindow& window) {
    sf::RectangleShape rect;
    //color/thikness should be constat
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    rect.setFillColor(sf::Color(0, 255, 0, 0));
    sf::Vector2f pos;

    pos =
        sf::Vector2f(m_boundary.x - m_boundary.w, m_boundary.y - m_boundary.h);
    rect.setPosition(pos);
    rect.setSize(sf::Vector2f(m_boundary.w * 2, m_boundary.h * 2));
    window.draw(rect);
    if (m_divided) {
      northwest->display(window);
      northeast->display(window);
      southeast->display(window);
      southwest->display(window);
      return;
    }
  }
 

 //to delete later
  void print_tree() {
    for (auto& boid_ptr : boids) {
      std::cout << (boid_ptr->pos()).x() << " , ";
    }
    std::cout << '\n';
    if (m_divided) {
      northeast->print_tree();
      northwest->print_tree();
      southeast->print_tree();
      southwest->print_tree();
    }
  }

  //to test for memory leaks.
  void delete_tree() {
    if (m_divided) {
      northeast->delete_tree();
      northwest->delete_tree();
      southeast->delete_tree();
      southwest->delete_tree();
      delete northeast;
      northeast = nullptr;
      delete northwest;
      northwest = nullptr;
      delete southeast;
      southeast = nullptr;
      delete southwest;
      southwest = nullptr;
      boids.empty();
      m_divided = false;
    }
  }
};
}  // namespace boids
#endif