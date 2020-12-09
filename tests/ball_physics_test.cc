#include <core/ball.h>
#include <catch2/catch.hpp>
using glm::vec2;

ci::Color def_color(255, 0, 0); // color to used to initalize balls in test cases
pool::Type type = pool::Type::Red; // used to intialize type in tests

TEST_CASE("Boundary Collisions", "[Boundary]") {
  // boundary Box coordinates top Left:(0,0) and bottom Right:(10,10)
  glm::vec2 top_left_bounds = vec2(0, 0);
  glm::vec2 bottom_right_bounds = vec2(10, 10);

  SECTION("Right Boundary Collision Tests") {
    SECTION("Moving towards Boundary") {
      pool::Ball ball(vec2(8, 5), 5, def_color, type);
      ball.SetVelocity(vec2(3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(-3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }

    SECTION("Moving Away from Boundary") {
      // checking for erroneous collision detection

      // particle is intersecting with boundary but moving away
      pool::Ball ball(vec2(8, 5), 5, def_color, type);
      ball.SetVelocity(vec2(-3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(-3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }
  }

  SECTION("Left Boundary Collision Test") {
    SECTION("Moving towards Boundary") {
      pool::Ball ball(vec2(2, 5), 5, def_color, type);
      ball.SetVelocity(vec2(-3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }

    SECTION("Moving Away from Boundary") {
      // checking for erroneous collision detection

      // particle is intersecting with boundary but moving away
      pool::Ball ball(vec2(2, 5), 5, def_color, type);
      ball.SetVelocity(vec2(3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }
  }

  SECTION("Top Boundary Collision Test") {
    SECTION("Moving towards Boundary") {
      pool::Ball ball(vec2(5, 2), 5, def_color, type);
      ball.SetVelocity(vec2(3, -2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }

    SECTION("Moving Away from Boundary") {
      // checking for erroneous collision detection

      // particle is intersecting with boundary but moving away
      pool::Ball ball(vec2(5, 2), 5, def_color, type);
      ball.SetVelocity(vec2(3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(2));
    }
  }SECTION("Bottom Boundary Collision Test") {
    SECTION("Moving towards Boundary") {
      pool::Ball ball(vec2(5, 8), 5, def_color, type);
      ball.SetVelocity(vec2(3, 2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(-2));
    }

    SECTION("Moving Away from Boundary") {
      // checking for erroneous collision detection

      // particle is intersecting with boundary but moving away
      pool::Ball ball(vec2(5, 8), 5, def_color, type);
      ball.SetVelocity(vec2(3, -2));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(3));
      REQUIRE(ball.GetVelocity().y == Approx(-2));
    }
  }SECTION("Boundary Corner Collisions Test") {
    SECTION("Top-Left Corner Collision") {
      // hits corner (0,0)
      pool::Ball ball(vec2(1, 1), 5, def_color, type);
      ball.SetVelocity(vec2(-1, -1));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(1));
      REQUIRE(ball.GetVelocity().y == Approx(1));
    }

    SECTION("Top-Right Corner Collision") {
      // hits corner (10,0)
      pool::Ball ball(vec2(9, 1), 5, def_color, type);
      ball.SetVelocity(vec2(1, -1));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(-1));
      REQUIRE(ball.GetVelocity().y == Approx(1));
    }

    SECTION("Bottom-Left Corner Collision") {
      // hits corner (0,10)
      pool::Ball ball(vec2(1, 9), 5, def_color, type);
      ball.SetVelocity(vec2(-1, 1));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(1));
      REQUIRE(ball.GetVelocity().y == Approx(-1));
    }

    SECTION("Bottom-Right Corner Collision") {
      // hits corner (10,10)
      pool::Ball ball(vec2(9, 9), 5, def_color, type);
      ball.SetVelocity(vec2(1, 1));
      ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
      REQUIRE(ball.GetVelocity().x == Approx(-1));
      REQUIRE(ball.GetVelocity().y == Approx(-1));
    }
  }

  SECTION("Test No Boundary Collision") {
    pool::Ball ball(vec2(5, 5), 2, def_color, type);
    ball.SetVelocity(vec2(3,-2));
    ball.HandleBoundaryCollision(top_left_bounds, bottom_right_bounds);
    REQUIRE(ball.GetVelocity().x == Approx(3));
    REQUIRE(ball.GetVelocity().y == Approx(-2));
  }
 }

TEST_CASE("Ball Collisions", "[Particle]") {
  SECTION("Balls that have not Collided") {
    // checking for erroneous collision detection

    // particles are not intersecting at all. Checks if no changes have been made to the velocity
    pool::Ball ball1(vec2(5, 5), 2, def_color, type);
    ball1.SetVelocity(vec2(1,0));
    pool::Ball ball2(vec2(10, 5), 2, def_color,type);
    ball2.SetVelocity(vec2(-1,0));
    ball1.HandleParticleCollision(ball2);
    REQUIRE(ball1.GetVelocity() == vec2(1, 0));
    REQUIRE(ball2.GetVelocity() == vec2(-1, 0));
  }

  SECTION("Balls Moving Away from Each Other") {
    // checking for erroneous collision detection

    // particles are intersecting but moving away. Checks if no changes have been made to the velocity
    pool::Ball ball1(vec2(5, 5), 2, def_color, type);
    ball1.SetVelocity(vec2(-1,0));
    pool::Ball ball2(vec2(6, 5), 2, def_color, type);
    ball2.SetVelocity(vec2(1,0));
    ball1.HandleParticleCollision(ball2);
    REQUIRE(ball1.GetVelocity() == vec2(-1, 0));
    REQUIRE(ball2.GetVelocity() == vec2(1, 0));
  }

  SECTION("Balls Colliding along X-axis") {
    pool::Ball ball1(vec2(20, 20), 1, def_color,type);
    ball1.SetVelocity(vec2(1,0));
    pool::Ball ball2(vec2(21, 20), 1, def_color, type);
    ball2.SetVelocity(vec2(-1,0));
    ball1.HandleParticleCollision(ball2);
    REQUIRE(ball1.GetVelocity() == vec2(-1, 0));
    REQUIRE(ball2.GetVelocity() == vec2(1, 0));
  }

  SECTION("Balls Colliding along Y-axis") {
    pool::Ball ball1(vec2(20, 20), 2, def_color,type);
    ball1.SetVelocity(vec2(0,1));
    pool::Ball ball2(vec2(20, 23), 2, def_color, type);
    ball2.SetVelocity(vec2(0,-1));
    ball1.HandleParticleCollision(ball2);
    REQUIRE(ball1.GetVelocity() == vec2(0, -1));
    REQUIRE(ball2.GetVelocity() == vec2(0, 1));
  }

  SECTION("Balls Colliding along Diagonal Axis") {
    pool::Ball ball1(vec2(20, 20), 2, def_color,type);
    ball1.SetVelocity(vec2(1,1));
    pool::Ball ball2(vec2(21, 21), 2, def_color, type);
    ball2.SetVelocity(vec2(-1,-1));
    ball1.HandleParticleCollision(ball2);
    REQUIRE(ball1.GetVelocity().x == Approx(-1.0).margin(1e-3));
    REQUIRE(ball1.GetVelocity().y == Approx(-1.0).margin(1e-3));
    REQUIRE(ball2.GetVelocity().x == Approx(1.0).margin(1e-3));
    REQUIRE(ball2.GetVelocity().y == Approx(1.0).margin(1e-3));
  }

  SECTION("Multiple Particle Collisions") {
    SECTION("2 particles Collide in 3 particle system") {
      pool::Ball ball1(vec2(20, 20), 2, def_color, type);
      pool::Ball ball2(vec2(20, 23), 2, def_color, type);
      pool::Ball ball3(vec2(20, 27), 2, def_color, type);

      ball1.SetVelocity(vec2(0,1));
      ball2.SetVelocity(vec2(0,-1));
      ball3.SetVelocity(vec2(0,-1));

      // collide entire 3 particle system with each other
      ball1.HandleParticleCollision(ball2);
      ball1.HandleParticleCollision(ball3);

      REQUIRE(ball1.GetVelocity() == vec2(0, -1));
      REQUIRE(ball2.GetVelocity() == vec2(0, 1));
      REQUIRE(ball3.GetVelocity() == vec2(0, -1)); // no change in particle ball3 since it never collided
    }

    SECTION("3 particles Collide in 3 particle system") {
      pool::Ball ball1(vec2(20, 20), 2, def_color, type);
      pool::Ball ball2(vec2(20, 23), 2, def_color, type);
      pool::Ball ball3(vec2(21, 21), 2, def_color, type);

      ball1.SetVelocity(vec2(0,1));
      ball2.SetVelocity(vec2(0,-1));
      ball3.SetVelocity(vec2(-1,0));
      // collide entire 3 particle system with each other
      ball2.HandleParticleCollision(ball3);
      ball1.HandleParticleCollision(ball2);

      // test to see change in velocities for all particles in the system
      REQUIRE(ball1.GetVelocity().x == Approx(0.0).margin(1e-3));
      REQUIRE(ball1.GetVelocity().y == Approx(.200).margin(1e-3));
      REQUIRE(ball2.GetVelocity().x == Approx(-.60).margin(1e-3));
      REQUIRE(ball2.GetVelocity().y == Approx(1.0).margin(1e-3));
      REQUIRE(ball3.GetVelocity().x == Approx(-.400).margin(1e-3));
      REQUIRE(ball3.GetVelocity().y == Approx(-1.20).margin(1e-3));
    }
  }
}
TEST_CASE("Ball Friction"){
  // friction coefficients used
  double stop_point = 5;
  double friction_force = .30;

  pool::Ball ball1(vec2(20, 20), 2, def_color, type);

  SECTION("Test Ball Slow Down"){
    ball1.SetVelocity(vec2(10,10));
    ball1.UpdatePosition(stop_point, friction_force);

    REQUIRE(ball1.GetVelocity().x == Approx(7.0).margin(1e-3));
    REQUIRE(ball1.GetVelocity().y == Approx(7.0).margin(1e-3));
  }

  SECTION("Test Ball Stop Down"){
    ball1.SetVelocity(vec2(1,1)); // velocity below stop-threshold
    ball1.UpdatePosition(stop_point, friction_force);
    REQUIRE(ball1.GetVelocity().x == Approx(0.0).margin(1e-3)); // velocity should be 0
    REQUIRE(ball1.GetVelocity().y == Approx(0.0).margin(1e-3));
  }

  SECTION("Test Ball Position Update"){
    ball1.SetVelocity(vec2(10,10));
    ball1.UpdatePosition(stop_point, friction_force);
    ball1.UpdatePosition(stop_point, friction_force);

    REQUIRE(ball1.GetPosition().x == Approx(37.0).margin(1e-3));
    REQUIRE(ball1.GetPosition().y == Approx(37.0).margin(1e-3));
  }
}