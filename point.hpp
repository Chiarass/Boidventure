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
inline Point operator+(const Point& a, const Point& b) {
  Point sum{a.x() + b.x(), a.y() + b.y()};
  return sum;
}

inline Point operator-(const Point& a, const Point& b) {
  Point diff{a.x() - b.x(), a.y() - b.y()};
  return diff;
}

inline Point operator*(double c, const Point& a) {
  Point res{c * a.x(), c * a.y()};
  return res;
}
}  // namespace boids

#endif