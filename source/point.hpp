#ifndef POINT_HPP
#define POINT_HPP

namespace boids {
class Point {
  double m_x{};
  double m_y{};

 public:
  Point(double x = 0., double y = 0.);
  
  // returns m_x, m_y
  double x() const;
  double y() const;

  // distance of Point from origin
  double distance() const;

  // implementation of rotation trasformation.
  // Param: angle of rotation (in radians, counterclockwise)
  void rotate(double);
};

// defines Point (that is mathematical 2-vector) operations
Point operator+(const Point&, const Point&);
Point operator-(const Point&, const Point&);
Point operator*(double, const Point&);
}  // namespace boids

#endif