#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>

#include "boid.hpp"
#include "constants.hpp"
#include "doctest.h"
#include "point.hpp"
#include "quadtree.hpp"
#include "statistics.hpp"

TEST_CASE("Testing the Point class") {
  SUBCASE("checking if x() and y() return m_x, m_y") {
    boids::Point p1;  // also checks deafult is (0.,0.)
    boids::Point p2{3., 2.};
    CHECK(p1.x() == doctest::Approx(0.));
    CHECK(p1.x() == doctest::Approx(0.));
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
    // 0 vector will always rotate to itself
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
    auto p5 = -0.32 * p4;
    CHECK(p5.x() == doctest::Approx(-1. * (-0.32)));
    CHECK(p5.y() == doctest::Approx(-2. * (-0.32)));
    auto p6 = 13. / 91. * (p3 - p4) + (p5 - p2 - p3) + p4 + p4 - 2 * p4;
    CHECK(p6.x() == doctest::Approx(-4.10857));
    CHECK(p6.y() == doctest::Approx(-2.78857));
    p6 = p5 = p4 = p3;  // checking if multiple assignments work
    CHECK(p6.x() == doctest::Approx(3.));
    CHECK(p6.y() == doctest::Approx(2.));
  }
}

TEST_CASE("Testing the quadtree class") {
  SUBCASE("checking if contains method works") {
    boids::Rectangle rect{0., 0., 10., 10.};
    boids::Point p1{5., -4.};
    boids::Point p2{
        11.,
        0,
    };
    boids::Point p3{0., 11.};
    boids::Point p4{10., 10.};
    CHECK(rect.contains(p1));
    CHECK(!rect.contains(p2));
    CHECK(!rect.contains(-1. * p2));
    CHECK(!rect.contains(p3));
    CHECK(!rect.contains(-1. * p3));
    CHECK(rect.contains(p4));
  }
}
/*
TEST_CASE("Testing the boid class") {
  SUBCASE("checking if separation method works") {
    boids::Point p1{0., 0.};
    boids::Point p2{1., 1.};

    // Create temporary Point objects
    boids::Point vel1{1., 0.};
    boids::Point vel2{0., 1.};
    boids::Point vel3{-1., -1.};
    boids::Boid boid1{p1, vel1};
    boids::Boid boid2{p2, vel2};
    boids::Boid boid3{p2, vel3};

    std::vector<boids::Boid*> boids_in_range = {&boid2, &boid3};
    boid1.separation(boids_in_range);

    boids::Point expected_velocity =
        boid1.vel() - boids::Point{separation_x, separation_y};
    CHECK(boid1.vel().x() == doctest::Approx(expected_velocity));
    CHECK(boid1.vel().y() == doctest::Approx(expected_velocity));
  }
}*/

TEST_CASE("Testing statistics.cpp") {
  SUBCASE("Testing approx_distance") {
    auto pos1 = boids::Point{10., 10.};
    auto pos2 = boids::Point{5., 15.};
    auto pos3 = boids::Point{-35., 17.};
    auto def_vel = boids::Point{};
    std::vector<boids::Boid> boids_vec = {boids::Boid{pos1, def_vel},
                                          boids::Boid{pos2, def_vel},
                                          boids::Boid{pos3, def_vel}};

    double distance_avg{};
    for(const auto& boid : boids_vec){
      for(const auto& other_boid : boids_vec){
        distance_avg += (other_boid.pos() - boid.pos()).distance();
      }
    }
    distance_avg *= 1./(boids_vec.size()*boids_vec.size());

    int sample_number = 100;

    double app_dist{0.};

    for (int j = 0; j < sample_number; ++j) {
    // flattening to one dimension, on a random 2d vector
      auto angle = boids::uniform(0., 2. * constants::pi);
      std::cout << angle;
      std::vector<double> flattened_positions;

    for (const auto &boid : boids_vec) {
      flattened_positions.push_back(cos(angle) * boid.pos().distance());
    }

    std::sort(flattened_positions.begin(), flattened_positions.end());

    // finding one dimensional sum of distances from first point. (sect 1.1 in
    // paper)
    double distance_from_first{
        std::accumulate(flattened_positions.begin(), flattened_positions.end(),
                        0., [&flattened_positions](double a, double b) {
                          return (a + (b - flattened_positions[0]));
                        })};
    app_dist += distance_from_first;

    // finding average distance of flattened points. (implementing formula in
    // section 1.1 of paper)
    for (int i = 0; i < static_cast<int>(flattened_positions.size()) - 1; ++i) {
      distance_from_first +=
          (2. * (i + 1) - static_cast<double>(flattened_positions.size())) *
          (flattened_positions[i + 1] - flattened_positions[i]);
      app_dist += distance_from_first;
    }
  }
  // taking mean to get expected value (eq 4. in paper) and multipling by
  // constant p(d) (see appendix in paper)
  app_dist *= (constants::pi) / (sample_number*boids_vec.size()*boids_vec.size());
  //dividing by number of points(boids)
  //todo: handle case in which size is 0
    CHECK(distance_avg == doctest::Approx(app_dist));
  }
}