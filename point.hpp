#ifndef POINT_HPP
#define POINT_HPP

class Point {
 double m_x{};
 double m_y{};

 public:
 Point(double = 0, double = 0);
 Point(const Point&);

//returns m_x, m_y
 double x() const; //is it necessary for const to go here?
 double y() const; //is it necessary for const to go here?

 double distance() const;
 void rotate(double);
 void operator=(const Point&);
 //aggiungere distruttore
};

Point operator+(const Point &, const Point &);  //should they stay inside or outside?
Point operator-(const Point &, const Point&);
Point operator*(double, const Point&);


#endif