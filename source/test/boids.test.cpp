#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <cmath>
#include <vector>

#include "./../boid.hpp"
#include "./../gui.hpp"
#include "./../point.hpp"
#include "./../quadtree.hpp"
#include "./../sfml.hpp"
#include "doctest.h"

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

// ////////////////////////////////////////////////////////////////////////////
// testing bird
TEST_CASE("Testing Bird::turn_around") {
  boids::Point origin{0., 0.};
  SUBCASE("if over left boundary it gets repelled") {
    boids::Point left_boundary = {
        constants::margin_size + constants::controls_width, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * left_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() != doctest::Approx(0.));
  }

  SUBCASE("if not over left boundary it does not get repelled") {
    boids::Point left_boundary = {
        constants::margin_size + constants::controls_width, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * left_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
  }

  SUBCASE("if over right boundary it gets repelled") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * right_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() != doctest::Approx(0.));
  }

  SUBCASE("if not over right boundary it does not get repelled") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * right_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() == doctest::Approx(0.));
  }

  SUBCASE("if over upper boundary it gets repelled") {
    boids::Point upper_boundary = {0., constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * upper_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() != doctest::Approx(0.));
  }

  SUBCASE("if not over upper boundary it does not get repelled") {
    boids::Point upper_boundary = {0., constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * upper_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("if under lower boundary it gets repelled") {
    boids::Point upper_boundary = {
        0., constants::window_height - constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * upper_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() != doctest::Approx(0.));
  }

  SUBCASE("if not under lower boundary it does not get repelled") {
    boids::Point upper_boundary = {
        0., constants::window_height - constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * upper_boundary, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  // having a point with no turn around force is necessary for boid and predator
  // class testing.
  SUBCASE("testing if there is no turn around force in window center") {
    boids::Point window_center{
        (constants::window_width - constants::controls_width) / 2.,
        constants::window_height / 2.};

    boids::Boid boid{window_center, origin};
    std::vector<const boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));
    CHECK(boid.vel().x() == doctest::Approx(0.));
  }
}

TEST_CASE("Testing Bird::repel method") {
  boids::Point origin{0., 0.};

  // if such test do not work it may be that constant::repel_coefficent is set
  // to 0.
  SUBCASE(
      "repel moves bird toward lower left quadrant if point is at upper "
      "right") {
    boids::Bird bird{origin, origin};
    boids::Point upper_right{1., 1.};
    bird.repel(upper_right, constants::repel_range,
               constants::repel_coefficent);
    CHECK(bird.vel().y() < 0.);
    CHECK(bird.vel().x() < 0.);
  }

  SUBCASE(
      "repel moves bird toward upper left quadrant if point is at lower "
      "right") {
    boids::Bird bird{origin, origin};
    boids::Point lower_right{1., -1.};
    bird.repel(lower_right, constants::repel_range,
               constants::repel_coefficent);
    CHECK(bird.vel().y() > 0.);
    CHECK(bird.vel().x() < 0.);
  }

  SUBCASE(
      "repel moves bird toward upper right quadrant if point is at lower "
      "left") {
    boids::Bird bird{origin, origin};
    boids::Point lower_left{-1, -1.};
    bird.repel(lower_left, constants::repel_range, constants::repel_coefficent);
    CHECK(bird.vel().y() > 0.);
    CHECK(bird.vel().x() > 0.);
  }

  SUBCASE(
      "repel moves bird toward lower right quadrant if point is at upper "
      "left") {
    boids::Bird bird{origin, origin};
    boids::Point upper_left{-1, 1.};
    bird.repel(upper_left, constants::repel_range, constants::repel_coefficent);
    CHECK(bird.vel().y() < 0.);
    CHECK(bird.vel().x() > 0.);
  }

  SUBCASE("Repel behaves well when passed position of boid as argument") {
    boids::Bird bird{origin, origin};
    bird.repel(bird.pos(), constants::repel_range, constants::repel_coefficent);
    CHECK(!std::isnan(bird.vel().x()));
    CHECK(!std::isnan(bird.vel().y()));
  }

  SUBCASE("repel force moves boid away from in range predator") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.repel(predator.pos(), 2., 1.);

    CHECK(boid.vel().x() <= 0.);
    CHECK(boid.vel().y() >= 0.);

    // check force is radially symmetric
    CHECK(boid.vel().y() == doctest::Approx(-1 * boid.vel().x()));
  }

  SUBCASE("the repel is null if predator out of range") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.repel(predator.pos(), 1., 1.);

    CHECK(boid.vel().x() == 0.);
    CHECK(boid.vel().y() == 0.);
  }

  SUBCASE("the repel force is null if coefficent is 0") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.repel(predator.pos(), 2., 0.);

    CHECK(boid.vel().x() == 0.);
    CHECK(boid.vel().y() == 0.);
  }
}

TEST_CASE("Testing Predator::update") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  // if it returns error it may be that constants::velocity_reduction_coefficent
  // is not between 0 and 1
  SUBCASE("if max velocity is exceeded, then predator is slowed down") {
    boids::Point exceeding_speed = {
        constants::max_velocity / constants::velocity_reduction_coefficent,
        constants::max_velocity / constants::velocity_reduction_coefficent};
    boids::Predator predator{window_center, exceeding_speed};
    std::vector<boids::Boid> in_range;

    predator.update(1., 0., in_range);
    CHECK(predator.vel().x() == doctest::Approx(constants::max_velocity));
    CHECK(predator.vel().y() == doctest::Approx(constants::max_velocity));
  }

  SUBCASE("predator will move towards closest boid") {
    boids::Predator predator{window_center, origin};
    boids::Boid left_boid{window_center + boids::Point{-1., 0.}, origin};
    // right boid is closest boid
    boids::Boid right_boid{window_center + boids::Point{0.5, 0.}, origin};
    boids::Boid upper_boid{window_center + boids::Point{0., 1.}, origin};
    boids::Boid lower_boid{window_center + boids::Point{0., -1.}, origin};

    std::vector<boids::Boid> in_range{left_boid, right_boid, upper_boid,
                                      lower_boid};
    predator.update(1., 2., in_range);

    CHECK(predator.vel().x() > 0.);
    CHECK(predator.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("predator will not move, if no boid is in range") {
    boids::Predator predator{window_center, origin};
    boids::Boid left_boid{window_center + boids::Point{-1., 0.}, origin};
    // right boid is closest boid
    boids::Boid right_boid{window_center + boids::Point{0.5, 0.}, origin};
    boids::Boid upper_boid{window_center + boids::Point{0., 1.}, origin};
    boids::Boid lower_boid{window_center + boids::Point{0., -1.}, origin};

    std::vector<boids::Boid> in_range{left_boid, right_boid, upper_boid,
                                      lower_boid};
    predator.update(1., 0.4, in_range);

    CHECK(predator.vel().x() == doctest::Approx(0.));
    CHECK(predator.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("predator will turn around if outside boundary") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    boids::Predator predator{1.1 * right_boundary, origin};
    std::vector<boids::Boid> in_range;

    predator.update(1., 0., in_range);
    CHECK(predator.vel().x() < 0.);
  }
}

TEST_CASE("testing Boid::separation") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  SUBCASE("boid will move away radially from other boid in separation range") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<const boids::Boid*> in_range{&other_boid};
    boid.update(1., in_range, 2., 1., 0., 0.);

    CHECK(boid.vel().x() < 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));

    in_range.clear();

    // forces of equally distant boids are equal in magnitude
    boids::Boid other_boid2{window_center + boids::Point{1., 1.}, origin};
    boids::Boid other_boid3{window_center + boids::Point{-1., -1.}, origin};
    boids::Boid boid2{window_center, origin};

    in_range.push_back(&other_boid2);
    in_range.push_back(&other_boid3);
    boid2.update(1., in_range, 2., 1., 0., 0.);

    CHECK(boid2.vel().x() == doctest::Approx(0.));
    CHECK(boid2.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply separation, if not in separation range") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<const boids::Boid*> in_range{&other_boid};
    boid.update(1., in_range, 0.5, 1., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply separation, if separation coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<const boids::Boid*> in_range{&other_boid};
    boid.update(1., in_range, 2., 0., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }
}

TEST_CASE("testing Boid::cohesion") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  SUBCASE("boid will approach radially other boid in cohesion range") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<const boids::Boid*> in_range{&other_boid};
    boid.update(1., in_range, 0., 0., 1., 0.);

    CHECK(boid.vel().x() > 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));

    in_range.clear();

    // forces of equally distant boids are equal in magnitude
    boids::Boid other_boid2{window_center + boids::Point{1., 1.}, origin};
    boids::Boid other_boid3{window_center + boids::Point{-1., -1.}, origin};
    boids::Boid boid2{window_center, origin};

    in_range.push_back(&other_boid2);
    in_range.push_back(&other_boid3);
    boid2.update(1., in_range, 0., 0., 1., 0.);

    CHECK(boid2.vel().x() == doctest::Approx(0.));
    CHECK(boid2.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply cohesion, if cohesion coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<const boids::Boid*> in_range{&other_boid};
    boid.update(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE(
      "if center of mass of other boids is boid, cohesion force will be null") {
    boids::Boid boid{window_center, origin};

    boids::Boid left_boid{window_center + boids::Point{-1., 0.}, origin};
    boids::Boid right_boid{window_center + boids::Point{1., 0.}, origin};
    boids::Boid upper_boid{window_center + boids::Point{0., 1.}, origin};
    boids::Boid lower_boid{window_center + boids::Point{0., -1.}, origin};

    std::vector<const boids::Boid*> in_range{&left_boid, &right_boid,
                                             &upper_boid, &lower_boid};
    boid.update(1., in_range, 0., 0., 1., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }
}

TEST_CASE("testing Boid::alignment") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  SUBCASE(
      "testing alignment in the case of stationary boid and only another boid "
      "in alignment range") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center, boids::Point{1., 1.}};

    std::vector<const boids::Boid*> in_range{&other_boid};

    boid.update(1., in_range, 0., 0., 0., 1.);

    CHECK(boid.vel().x() == doctest::Approx(1.));
    CHECK(boid.vel().y() == doctest::Approx(1.));
  }

  SUBCASE("boid will not apply alignment, if alignment coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center, boids::Point{1., 1.}};

    std::vector<const boids::Boid*> in_range{&other_boid};

    boid.update(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("testing alignment in a symmetrical case") {
    boids::Boid boid{window_center, origin};

    boids::Boid left_boid{origin, boids::Point{-1., 0.}};
    // provided the boid is in range, the distance isn't relevant to the
    // alignment force
    boids::Boid right_boid{boids::Point{9., -29.}, boids::Point{1., 0.}};
    boids::Boid upper_boid{origin, boids::Point{0., 1.}};
    boids::Boid lower_boid{origin, boids::Point{0., -1.}};

    std::vector<const boids::Boid*> in_range{&left_boid, &right_boid,
                                             &upper_boid, &lower_boid};
    boid.update(1., in_range, 0., 0., 0., 1.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("testing alignment in a nearly symmetrical case") {
    boids::Point offest{1., 1.};
    boids::Boid boid{window_center, offest};

    // the disposition of speed vectors of the other boids is symmetrical in th
    // reference frame of the boid
    boids::Boid left_boid{origin, boids::Point{-1., 0.} + offest};
    // provided the boid is in range, the distance isn't relevant to the
    // alignment force
    boids::Boid right_boid{boids::Point{9., -29.},
                           boids::Point{1., 0.} + offest};
    boids::Boid upper_boid{origin, boids::Point{0., 1.} + offest};
    boids::Boid lower_boid{origin, boids::Point{0., -1.} + offest};

    std::vector<const boids::Boid*> in_range{&left_boid, &right_boid,
                                             &upper_boid, &lower_boid};
    boid.update(1., in_range, 0., 0., 0., 1.);

    // checks that the alignment force on the boid is null
    CHECK(boid.vel().x() == doctest::Approx(1.));
    CHECK(boid.vel().y() == doctest::Approx(1.));
  }
}

// if it returns error it may be that constants::velocity_reduction_coefficent
// is not between 0 and 1
TEST_CASE("testing Boid::update") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  SUBCASE("if max velocity is exceeded, then boid is slowed down") {
    boids::Point exceeding_speed = {
        constants::max_velocity / constants::velocity_reduction_coefficent,
        constants::max_velocity / constants::velocity_reduction_coefficent};
    boids::Boid boid{window_center, exceeding_speed};
    std::vector<const boids::Boid*> in_range;

    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() == doctest::Approx(constants::max_velocity));
    CHECK(boid.vel().y() == doctest::Approx(constants::max_velocity));
  }

  SUBCASE("boid will turn around if outside boundary") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    boids::Boid boid{1.1 * right_boundary, origin};
    std::vector<const boids::Boid*> in_range;

    boid.update(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() < 0.);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("testing Rectangle::contains") {
  boids::Rectangle rect{0., 0., 10., 10.};
  boids::Point p1{5., -4.};
  boids::Point p2{11., 0.};
  boids::Point p3{0., 11.};
  boids::Point p4{10., 10.};
  CHECK(rect.contains(p1));
  CHECK(!rect.contains(p2));
  CHECK(!rect.contains(-1. * p2));
  CHECK(!rect.contains(p3));
  CHECK(!rect.contains(-1. * p3));

  // contains does not include boundaries
  CHECK(!rect.contains(p4));
}

TEST_CASE("testing Quad_tree::square_collide and Quad_tree::insert") {
  SUBCASE(
      "case of boid whose range collides with cell, and other boid is in "
      "range") {
    boids::Rectangle square{0., 0., 1., 1.};
    boids::Quad_tree tree{1, square};

    boids::Boid boid1{boids::Point{0.9, 0.9}};
    tree.insert(boid1);

    boids::Boid boid2{boids::Point{0.9, 0.9}};

    std::vector<const boids::Boid*> in_range;

    // calls square collision
    tree.query(1., boid2, in_range);
    CHECK(!in_range.empty());

    if (!in_range.empty()) {
      CHECK(in_range[0] == &boid1);
    }
  }

  SUBCASE(
      "second case of boid whose range collides with cell, and other boid is "
      "in range") {
    boids::Rectangle square{0., 0., 1., 1.};
    boids::Quad_tree tree{1, square};

    boids::Boid boid1{boids::Point{0.9, 0.9}};
    tree.insert(boid1);

    boids::Boid boid2{boids::Point{2., 2.}};

    std::vector<const boids::Boid*> in_range;

    // square collides and boid is in range
    tree.query(1.6, boid2, in_range);

    CHECK(!in_range.empty());

    if (!in_range.empty()) {
      CHECK(in_range[0] == &boid1);
    }
  }

  SUBCASE(
      "case of boid whose range collide with cell, but other boid is not in "
      "range") {
    boids::Rectangle square{0., 0., 1., 1.};
    boids::Quad_tree tree{1, square};

    boids::Boid boid1{boids::Point{0.9, 0.9}};
    tree.insert(boid1);

    boids::Boid boid2{boids::Point{2., 2.}};

    std::vector<const boids::Boid*> in_range;

    // square collides but boid is not in range
    tree.query(1., boid2, in_range);
    CHECK(in_range.empty());
  }
}

TEST_CASE("testing Quad_tree::query") {
  SUBCASE("case of boid in cell and boid passed to query being the same boid") {
    boids::Rectangle square{0., 0., 1., 1.};
    boids::Quad_tree tree{1, square};

    boids::Boid boid1{boids::Point{0.9, 0.9}};
    tree.insert(boid1);

    std::vector<const boids::Boid*> in_range;

    // calls square collision
    tree.query(1., boid1, in_range);
    CHECK(in_range.empty());
  }
}

TEST_CASE("testing Quad_tree::subdivide") {
  SUBCASE(
      "if boid is perfectly in between cells, then it is not capture by child "
      "cells") {
    boids::Rectangle square{0., 0., 1., 1.};
    boids::Quad_tree tree{1, square};

    boids::Boid boid1{boids::Point{0.5, 0.5}};

    // is in the middle of subdivided cells
    boids::Boid boid2{};

    tree.insert(boid1);
    tree.insert(boid2);

    std::vector<const boids::Boid*> in_range;
    boids::Boid boid3{};

    tree.query(1., boid3, in_range);

    // both boid1 and boid2 are in the query range of boid3, but only boid1 has
    // been passed to the in_range vector, because, being in_between the divided
    // cells, boid2 has been dropped from the quad tree m_boids_ptr with the
    // subdivision of the cell
    CHECK(in_range[0] == &boid1);
  }
}

// class to test for memory leaks

class memory_tracker {
 public:
  int allocated = 0;
  int freed = 0;

  int current_usage() { return (allocated - freed); }

  void reset() {
    allocated = 0;
    freed = 0;
  }
};

inline memory_tracker tracker;

// operator overlad of new
void* operator new(size_t size) {
  tracker.allocated += 1;
  return malloc(size);
}

// operator overload of delete
void operator delete(void* memory) {
  tracker.freed += 1;
  free(memory);
}

TEST_CASE("testing quad tree for memory leaks") {
  {
    tracker.reset();
    boids::Rectangle unit_square{100., 100., 20, 20};
    boids::Quad_tree tree{1, unit_square};

    boids::Boid boid1{boids::Point{100., 100.}};
    boids::Boid boid2{boids::Point{100.1, 100.1}};

    tree.insert(boid1);
    tree.insert(boid2);
  }
  // new has been called as many times as delete
  CHECK(tracker.freed == tracker.allocated);
}