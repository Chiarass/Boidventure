#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <cmath>
#include <vector>

#include "boid.hpp"
#include "constants.hpp"
#include "doctest.h"
#include "point.hpp"
#include "quadtree.hpp"
#include "sfml.hpp"
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

// ////////////////////////////////////////////////////////////////////////////
// testing bird
TEST_CASE("Testing Bird::turn_around") {
  boids::Point origin{0., 0.};
  SUBCASE("if over left boundary it gets repelled") {
    boids::Point left_boundary = {
        constants::margin_size + constants::controls_width, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * left_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() != doctest::Approx(0.));
  }

  SUBCASE("if not over left boundary it does not get repelled") {
    boids::Point left_boundary = {
        constants::margin_size + constants::controls_width, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * left_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
  }

  SUBCASE("if over right boundary it gets repelled") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * right_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() != doctest::Approx(0.));
  }

  SUBCASE("if not over right boundary it does not get repelled") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * right_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() == doctest::Approx(0.));
  }

  SUBCASE("if over upper boundary it gets repelled") {
    boids::Point upper_boundary = {0., constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * upper_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() != doctest::Approx(0.));
  }

  SUBCASE("if not over upper boundary it does not get repelled") {
    boids::Point upper_boundary = {0., constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * upper_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("if under lower boundary it gets repelled") {
    boids::Point upper_boundary = {
        0., constants::window_height - constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{1.1 * upper_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() != doctest::Approx(0.));
  }

  SUBCASE("if not under lower boundary it does not get repelled") {
    boids::Point upper_boundary = {
        0., constants::window_height - constants::margin_size};
    // initializing boid left of left boundary
    boids::Boid boid{0.9 * upper_boundary, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  // having a point with no turn around force is necessary for boid and predator
  // class testing.
  SUBCASE("testing if there is no turn around force in window center") {
    boids::Point window_center{
        (constants::window_width - constants::controls_width) / 2.,
        constants::window_height / 2.};

    boids::Boid boid{window_center, origin};
    std::vector<boids::Boid*> in_range;
    // passing empty range, only turn around will update
    boid.update_boid(1., in_range, 0., 0., 0., 0.);
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
    bird.repel(upper_right);
    CHECK(bird.vel().y() < 0.);
    CHECK(bird.vel().x() < 0.);
  }

  SUBCASE(
      "repel moves bird toward upper left quadrant if point is at lower "
      "right") {
    boids::Bird bird{origin, origin};
    boids::Point lower_right{1., -1.};
    bird.repel(lower_right);
    CHECK(bird.vel().y() > 0.);
    CHECK(bird.vel().x() < 0.);
  }

  SUBCASE(
      "repel moves bird toward upper right quadrant if point is at lower "
      "left") {
    boids::Bird bird{origin, origin};
    boids::Point lower_left{-1, -1.};
    bird.repel(lower_left);
    CHECK(bird.vel().y() > 0.);
    CHECK(bird.vel().x() > 0.);
  }

  SUBCASE(
      "repel moves bird toward lower right quadrant if point is at upper "
      "left") {
    boids::Bird bird{origin, origin};
    boids::Point upper_left{-1, 1.};
    bird.repel(upper_left);
    CHECK(bird.vel().y() < 0.);
    CHECK(bird.vel().x() > 0.);
  }

  SUBCASE("Repel behaves well when passed position of boid as argument") {
    boids::Bird bird{origin, origin};
    bird.repel(bird.pos());
    CHECK(!std::isnan(bird.vel().x()));
    CHECK(!std::isnan(bird.vel().y()));
  }
}

TEST_CASE("Testing Predator::update_predator") {
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

    predator.update_predator(1., 0., in_range);
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
    predator.update_predator(1., 2., in_range);

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
    predator.update_predator(1., 0.4, in_range);

    CHECK(predator.vel().x() == doctest::Approx(0.));
    CHECK(predator.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("predator will turn around if outside boundary") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    boids::Predator predator{1.1 * right_boundary, origin};
    std::vector<boids::Boid> in_range;

    predator.update_predator(1., 0., in_range);
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

    std::vector<boids::Boid*> in_range{&other_boid};
    boid.update_boid(1., in_range, 2., 1., 0., 0.);

    CHECK(boid.vel().x() < 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));

    in_range.clear();

    // forces of equally distant boids are equal in magnitude
    boids::Boid other_boid2{window_center + boids::Point{1., 1.}, origin};
    boids::Boid other_boid3{window_center + boids::Point{-1., -1.}, origin};
    boids::Boid boid2{window_center, origin};

    in_range.push_back(&other_boid2);
    in_range.push_back(&other_boid3);
    boid2.update_boid(1., in_range, 2., 1., 0., 0.);

    CHECK(boid2.vel().x() == doctest::Approx(0.));
    CHECK(boid2.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply separation, if not in separation range") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<boids::Boid*> in_range{&other_boid};
    boid.update_boid(1., in_range, 0.5, 1., 0., 0.);

    CHECK(boid.vel().x() == doctest::Approx(0.));
    CHECK(boid.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply separation, if separation coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<boids::Boid*> in_range{&other_boid};
    boid.update_boid(1., in_range, 2., 0., 0., 0.);

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

    std::vector<boids::Boid*> in_range{&other_boid};
    boid.update_boid(1., in_range, 0., 0., 1., 0.);

    CHECK(boid.vel().x() > 0.);
    CHECK(boid.vel().y() == doctest::Approx(0.));

    in_range.clear();

    // forces of equally distant boids are equal in magnitude
    boids::Boid other_boid2{window_center + boids::Point{1., 1.}, origin};
    boids::Boid other_boid3{window_center + boids::Point{-1., -1.}, origin};
    boids::Boid boid2{window_center, origin};

    in_range.push_back(&other_boid2);
    in_range.push_back(&other_boid3);
    boid2.update_boid(1., in_range, 0., 0., 1., 0.);

    CHECK(boid2.vel().x() == doctest::Approx(0.));
    CHECK(boid2.vel().y() == doctest::Approx(0.));
  }

  SUBCASE("boid will not apply cohesion, if cohesion coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center + boids::Point{1., 0.}, origin};

    std::vector<boids::Boid*> in_range{&other_boid};
    boid.update_boid(1., in_range, 0., 0., 0., 0.);

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

    std::vector<boids::Boid*> in_range{&left_boid, &right_boid, &upper_boid,
                                       &lower_boid};
    boid.update_boid(1., in_range, 0., 0., 1., 0.);

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

    std::vector<boids::Boid*> in_range{&other_boid};

    boid.update_boid(1., in_range, 0., 0., 0., 1.);

    CHECK(boid.vel().x() == doctest::Approx(1.));
    CHECK(boid.vel().y() == doctest::Approx(1.));
  }

  SUBCASE("boid will not apply alignment, if alignment coefficent is 0") {
    boids::Boid boid{window_center, origin};
    boids::Boid other_boid{window_center, boids::Point{1., 1.}};

    std::vector<boids::Boid*> in_range{&other_boid};

    boid.update_boid(1., in_range, 0., 0., 0., 0.);

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

    std::vector<boids::Boid*> in_range{&left_boid, &right_boid, &upper_boid,
                                       &lower_boid};
    boid.update_boid(1., in_range, 0., 0., 0., 1.);

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

    std::vector<boids::Boid*> in_range{&left_boid, &right_boid, &upper_boid,
                                       &lower_boid};
    boid.update_boid(1., in_range, 0., 0., 0., 1.);

    // checks that the alignment force on the boid is null
    CHECK(boid.vel().x() == doctest::Approx(1.));
    CHECK(boid.vel().y() == doctest::Approx(1.));
  }
}

// if it returns error it may be that constants::velocity_reduction_coefficent
// is not between 0 and 1
TEST_CASE("testing Boid::update_boid") {
  boids::Point window_center{
      (constants::window_width - constants::controls_width) / 2.,
      constants::window_height / 2.};
  boids::Point origin{0., 0.};

  SUBCASE("if max velocity is exceeded, then boid is slowed down") {
    boids::Point exceeding_speed = {
        constants::max_velocity / constants::velocity_reduction_coefficent,
        constants::max_velocity / constants::velocity_reduction_coefficent};
    boids::Boid boid{window_center, exceeding_speed};
    std::vector<boids::Boid*> in_range;

    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() == doctest::Approx(constants::max_velocity));
    CHECK(boid.vel().y() == doctest::Approx(constants::max_velocity));
  }

  SUBCASE("boid will turn around if outside boundary") {
    boids::Point right_boundary = {
        constants::window_width + constants::margin_size, 0.};
    boids::Boid boid{1.1 * right_boundary, origin};
    std::vector<boids::Boid*> in_range;

    boid.update_boid(1., in_range, 0., 0., 0., 0.);
    CHECK(boid.vel().x() < 0.);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////

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

TEST_CASE("testing Boid::escape_predator") {
  boids::Point origin{0., 0.};
  SUBCASE("escape force moves boid away from in range predator") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.escape_predator(predator, 2., 1.);

    CHECK(boid.vel().x() <= 0.);
    CHECK(boid.vel().y() >= 0.);

    // check force is radially symmetric
    CHECK(boid.vel().y() == doctest::Approx(-1 * boid.vel().x()));
  }

  SUBCASE("the escape is null if predator out of range") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.escape_predator(predator, 1., 1.);

    CHECK(boid.vel().x() == 0.);
    CHECK(boid.vel().y() == 0.);
  }

  SUBCASE("the escape force is null if coefficent is 0") {
    boids::Boid boid{origin, origin};
    boids::Predator predator{boids::Point{1., -1.}, origin};

    boid.escape_predator(predator, 2., 0.);

    CHECK(boid.vel().x() == 0.);
    CHECK(boid.vel().y() == 0.);
  }
}

// TEST_CASE("Testing the boid class") {
//   SUBCASE("checking if separation method works") {
//     boids::Point p1{0., 0.};
//     boids::Point p2{1., 1.};

//     // Create temporary Point objects
//     boids::Point vel1{1., 0.};
//     boids::Point vel2{0., 1.};
//     boids::Point vel3{-1., -1.};
//     boids::Boid boid1{p1, vel1};
//     boids::Boid boid2{p2, vel2};
//     boids::Boid boid3{p2, vel3};

//     std::vector<boids::Boid*> boids_in_range = {&boid2, &boid3};
//     boid1.separation(boids_in_range);

//     boids::Point expected_velocity =
//         boid1.vel() - boids::Point{separation_x, separation_y};
//     CHECK(boid1.vel().x() == doctest::Approx(expected_velocity));
//     CHECK(boid1.vel().y() == doctest::Approx(expected_velocity));
//   }
// }