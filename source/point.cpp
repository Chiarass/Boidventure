#include "point.hpp"

#include <cmath> //for sin, cos

namespace boids {
Point::Point(double x, double y) : m_x{x}, m_y{y} {};

double Point::x() const { return m_x; };
double Point::y() const { return m_y; };

double Point::distance() const { return std::sqrt(m_x * m_x + m_y * m_y); };

void Point::rotate(double angle) {
  double temp{};
  //rotation transformation in applied (see: 2D rotation matrix)
  temp = m_x * std::cos(angle) - m_y * std::sin(angle);
  m_y = m_x * std::sin(angle) + m_y * std::cos(angle);
  m_x = temp;
}

Point operator+(const Point& a, const Point& b) {
  Point sum{a.x() + b.x(), a.y() + b.y()};
  return sum;
}

Point operator-(const Point& a, const Point& b) {
  Point diff{a.x() - b.x(), a.y() - b.y()};
  return diff;
}

Point operator*(double c, const Point& a) {
  Point res{c * a.x(), c * a.y()};
  return res;
}
}  // namespace boids