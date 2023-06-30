#include "point.hpp"
#include <cmath>

// Point constructor
Point::Point(double x, double y) : m_x{x}, m_y{y} {};

Point::Point(const Point& a) : m_x{a.m_x}, m_y{a.m_y} {};

// functions to access x and y coordinates
double Point::x() const { return m_x; }

double Point::y() const { return m_y; }

// distance of Point from origin
double Point::distance() const { return std::sqrt(m_x * m_x + m_y * m_y); }

void Point::rotate(double angle) {
  double temp{};
  temp = m_x * std::cos(angle) - m_y * std::sin(angle);
  m_y = m_x * std::sin(angle) + m_y * std::cos(angle);
  m_x = temp;
}

void Point::operator=(const Point& a) {
  m_x = a.x();
  m_y = a.y();
}

// defines Point (that is mathematical 2-vector) operations
Point operator+(const Point& a, const Point& b) {
  Point sum{
      a.x() + b.x(),
      a.y() +
          b.y()};  // maybe use brace initialization when calling constructor?
  return sum;
}

Point operator-(const Point& a, const Point& b) {
  Point diff{
      a.x() - b.x(),
      a.y() -
          b.y()};  // maybe use brace initialization when calling constructor?
  return diff;
}

Point operator*(double c, const Point& a) {
  Point res{
      c * a.x(),
      c * a.y()};  // maybe use brace initialization when calling constructor?
  return res;
}