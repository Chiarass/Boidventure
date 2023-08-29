#include "quadtree.hpp"

#include <algorithm>  //for std::find, std::for_each
#include <cassert>
#include <iostream>
#include <vector>

#include "boid.hpp"
#include "point.hpp"

namespace boids {
bool Rectangle::contains(const Point& p) const {
  return (p.x() > x - w && p.x() < x + w && p.y() < y + h && p.y() > y - h);
}

Quad_tree::Quad_tree(int capacity, const Rectangle& boundary)
    : m_capacity{capacity}, m_boundary{boundary} {
  assert(capacity > 0);
}

Quad_tree::~Quad_tree() { this->delete_tree(); }

void Quad_tree::subdivide() {
  // ne stands for north east
  // the division by two happens because w and h represent half
  // of the width and height respectively. the center of child
  // quad tree is displace one fourth of the width and height
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

void Quad_tree::insert(const Boid& boid) {
  // checks if passing already inserted boid
  assert(std::none_of(
      m_boids_ptr.begin(), m_boids_ptr.end(),
      [&boid](const Boid* boid_ptr) { return &boid == boid_ptr; }));

  if (m_boundary.contains(boid.pos())) {
    if ((static_cast<int>(m_boids_ptr.size()) < m_capacity && !m_divided)) {
      m_boids_ptr.push_back(&boid);
    }

    else {
      if (!m_divided) {
        subdivide();

        // transfering boids in m_boids_ptr to children cells
        for (auto& inserted_boids : m_boids_ptr) {
          assert(inserted_boids);
          northeast->insert(*inserted_boids);
          northwest->insert(*inserted_boids);
          southeast->insert(*inserted_boids);
          southwest->insert(*inserted_boids);
        }

        m_boids_ptr.clear();
      }

      northeast->insert(boid);
      northwest->insert(boid);
      southeast->insert(boid);
      southwest->insert(boid);
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
                      std::vector<const Boid*>& in_range) const {
  if (!square_collide(range, boid)) {
    return;
  }

  for (auto other_boid_ptr : m_boids_ptr) {
    assert(other_boid_ptr);

    if ((other_boid_ptr->pos() - boid.pos()).distance() < range) {
      if (&boid != other_boid_ptr) {
        in_range.push_back(other_boid_ptr);
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
  // todo: color/thickness should be constat
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

  m_boids_ptr.clear();
}
}  // namespace boids