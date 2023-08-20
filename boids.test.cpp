#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "constants.hpp"
#include "doctest.h"
#include "point.hpp"
#include "quadtree.hpp"

TEST_CASE("Testing the Point class") {
  SUBCASE("checking if x() and y() return m_x, m_y") {
    boids::Point p1;  // also checks deafult is (0.,0.)
    boids::Point p2{3., 2.};
    CHECK(p1.x() == doctest::Approx(0.));
    CHECK(p1.y() == doctest::Approx(0.));
    CHECK(p2.x() == doctest::Approx(3.));
    CHECK(p2.y() == doctest::Approx(2.));
  }

  SUBCASE("checking if copy constructor works") {
    boids::Point p1{1., 3.};
    auto p2{p1};
    CHECK(p2.x() == doctest::Approx(1.));
    CHECK(p2.y() == doctest::Approx(3.));

    boids::Point p3{-123., 0.0021};
    auto p4{p3};
    CHECK(p4.x() == doctest::Approx(-123.));
    CHECK(p4.y() == doctest::Approx(0.0021));
  }

  SUBCASE("checking if distance method works") {
    boids::Point p1{3., 4.};
    CHECK(p1.distance() == doctest::Approx(5.));
    boids::Point p2{-2.432, -39.32};
    CHECK(p2.distance() == doctest::Approx(39.3951));
    boids::Point p3{1., 0.};
    boids::Point p4{0., -3.};
    CHECK((p4 - p3).distance() == doctest::Approx(3.16228));
  }

  SUBCASE("checking if rotate method works") {
    boids::Point p1{1., 0.};
    p1.rotate(constants::pi);
    CHECK(p1.x() == doctest::Approx(-1.));
    CHECK(p1.y() == doctest::Approx(0.));
    p1.rotate(-1. / 2. * constants::pi);
    CHECK(p1.x() == doctest::Approx(0.));
    CHECK(p1.y() == doctest::Approx(1.));
    p1.rotate(2.6912977);
    CHECK(p1.x() == doctest::Approx(-0.435231));
    CHECK(p1.y() == doctest::Approx(-0.90031));
    p1.rotate(2 * constants::pi);
    CHECK(p1.x() == doctest::Approx(-0.435231));
    CHECK(p1.y() == doctest::Approx(-0.90031));
    //0 vector will always rotate to itself
    boids::Point p0{0., 0.};
    p0.rotate(-3421.);
    CHECK(p0.x() == doctest::Approx(0.));
    CHECK(p0.y() == doctest::Approx(0.));
  }

  SUBCASE("checking point operations") {
    boids::Point p1{1., 0};
    boids::Point p2{2., 2.};
    auto p3 = p1 + p2;
    CHECK(p3.x() == doctest::Approx(3.));
    CHECK(p3.y() == doctest::Approx(2.));
    auto p4 = p1 - p2;
    CHECK(p4.x() == doctest::Approx(-1.));
    CHECK(p4.y() == doctest::Approx(-2.));
    auto p5 = -0.32*p4;
    CHECK(p5.x() == doctest::Approx(-1.*(-0.32)));
    CHECK(p5.y() == doctest::Approx(-2.*(-0.32)));
    auto p6 = 13./91.*(p3 - p4)+(p5 -p2 -p3) + p4 + p4 - 2*p4;
    CHECK(p6.x() == doctest::Approx(-4.10857));
    CHECK(p6.y() == doctest::Approx(-2.78857));
    p6 = p5 = p4 = p3; //checking if multiple assignments work
    CHECK(p6.x() == doctest::Approx(3.));
    CHECK(p6.y() == doctest::Approx(2.));
  }
}

TEST_CASE("Testing the quadtree class"){
    SUBCASE("checking if rectangle contains method works"){
      boids::Rectangle rect{0.,0.,10.,10.};
      boids::Point p1{5., -4.};
      boids::Point p2{11., 0,};
      boids::Point p3 {0., 11.};
      boids::Point p4{10., 10.};
      CHECK(rect.contains(p1));
      CHECK(!rect.contains(p2));
      CHECK(!rect.contains(-1.*p2));
      CHECK(!rect.contains(p3));
      CHECK(!rect.contains(-1.*p3));
      CHECK(rect.contains(p4));
    }
}