#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

namespace boids {
class Point {
  double m_x{};
  double m_y{};

 public:
  Point(double x = 0, double y = 0);
  Point(const Point&);
  // aggiungere distruttore

  // returns m_x, m_y
  double x() const;
  double y() const;

  // distance of Point from origin
  double distance() const;

  // implementation of rotation trasformation.
  // parameter assumed to be in radians
  void rotate(double);

  Point& operator=(const Point&);
};

// defines Point (that is mathematical 2-vector) operations
Point operator+(const Point&, const Point&);
Point operator-(const Point&, const Point&);
Point operator*(double, const Point&);
//double operator*(const Point&, const Point&);
}  // namespace boids

#endif