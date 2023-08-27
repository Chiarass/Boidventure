#include "quadtree.hpp"

#include <cassert>
#include <iostream>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {
bool Rectangle::contains(const Point& p) {
  return (p.x() >= x - w && p.x() <= x + w && p.y() <= y + h && p.y() >= y - h);
}

Quad_tree::Quad_tree(int capacity, const Rectangle& boundary)
    : m_capacity{capacity}, m_boundary{boundary} {}

Quad_tree::~Quad_tree(){
  this->delete_tree();
}

void Quad_tree::subdivide() {
  // ne stands for north east
  // the division by two happens because w and h represent half
  // of the width and height respectively. the center of child
  // quad tree is displace  one fourth of the width and height
  // from the parent cell.
  Rectangle ne = Rectangle{m_boundary.x + m_boundary.w / 2.,
                           m_boundary.y + m_boundary.h / 2., m_boundary.w / 2.,
                           m_boundary.h / 2.};
  northeast = new Quad_tree{m_capacity, ne};

  Rectangle nw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                           m_boundary.y + m_boundary.h / 2., m_boundary.w / 2.,
                           m_boundary.h / 2.};
  northwest = new Quad_tree{m_capacity, nw};

  Rectangle se = Rectangle{m_boundary.x + m_boundary.w / 2.,
                           m_boundary.y - m_boundary.h / 2., m_boundary.w / 2.,
                           m_boundary.h / 2.};
  southeast = new Quad_tree{m_capacity, se};

  Rectangle sw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                           m_boundary.y - m_boundary.h / 2., m_boundary.w / 2.,
                           m_boundary.h / 2.};
  southwest = new Quad_tree{m_capacity, sw};
  m_divided = true;
}

void Quad_tree::insert(Boid* boid_ptr) {
  if (boid_ptr && m_boundary.contains(boid_ptr->pos())) {
    if ((static_cast<int>(boids_ptr.size()) < m_capacity && !m_divided)) {
      boids_ptr.push_back(boid_ptr);
    } else {
      if (!m_divided) {
        subdivide();
        for (auto& inserted_boids_ptr : boids_ptr) {
          northeast->insert(inserted_boids_ptr);
          northwest->insert(inserted_boids_ptr);
          southeast->insert(inserted_boids_ptr);
          southwest->insert(inserted_boids_ptr);
        }
        boids_ptr.clear();
      }
      northeast->insert(boid_ptr);
      northwest->insert(boid_ptr);
      southeast->insert(boid_ptr);
      southwest->insert(boid_ptr);
    }
  }
}

bool Quad_tree::square_collide(double range, const Boid& boid) const {
  if (boid.pos().x() + range < m_boundary.x - m_boundary.w ||
      boid.pos().x() - range > m_boundary.x + m_boundary.w ||
      boid.pos().y() + range < m_boundary.y - m_boundary.h ||
      boid.pos().y() - range > m_boundary.y + m_boundary.h) {
    return false;
  }
  return true;
}

void Quad_tree::query(double range, const Boid& boid,
                      std::vector<Boid*>& in_range) const {
  if (!square_collide(range, boid)) {
    return;
  }

  for (auto other_boid : boids_ptr) {
    if ((other_boid->pos() - boid.pos()).distance() < range) {
      if (&boid != other_boid) {
        in_range.push_back(other_boid);
      }
    }
  }

  if (m_divided) {
    northeast->query(range, boid, in_range);
    northwest->query(range, boid, in_range);
    southeast->query(range, boid, in_range);
    southwest->query(range, boid, in_range);
  }
}
void Quad_tree::display(sf::RenderWindow& window) {
  sf::RectangleShape rect;
  // color/thikness should be constat
  rect.setOutlineColor(sf::Color::Green);
  rect.setOutlineThickness(1);
  rect.setFillColor(sf::Color(0, 255, 0, 0));
  sf::Vector2f pos;

  pos = sf::Vector2f(m_boundary.x - m_boundary.w, m_boundary.y - m_boundary.h);
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

// todo: test for memory leaks.
void Quad_tree::delete_tree() {
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
    m_divided = false;
  }
  boids_ptr.clear();
}
}  // namespace boids