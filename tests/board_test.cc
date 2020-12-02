#include <visualizer/board.h>
#include <core/ball.h>
#include <catch2/catch.hpp>
#include <cmath>
using glm::vec2;

TEST_CASE("Board Tests") {
  //gameboard
  pool::visualizer::Board game_board_(glm::vec2(200, 200), 800, 300);

  SECTION("Test Board Initialization") {
    REQUIRE(game_board_.GetGameBalls().size() == 16); //check if there are 16 balls
  }

  SECTION("Calculate Cue Hit Direction"){
    //direction in which cue is angled at based on clicked coords
    vec2 hit_dir = game_board_.CalculateHitDirection(vec2(20,20));
    REQUIRE(hit_dir.x == Approx(.755).margin(1e-3));
    REQUIRE(hit_dir.y == Approx(.655).margin(1e-3));
  }

  game_board_.Clear(); //clear game board for testing purposes

  SECTION("Test Pocketing") {
    //add ball within top left pocket range
    game_board_.AddBall(pool::Ball(vec2(203,203), 5, ci::Color("white"),
                                   pool::Type::Red));
    game_board_.Update();
    REQUIRE(game_board_.GetGameBalls().size() == 0); // check if ball is removed due to being in a pocket
    REQUIRE(game_board_.GetPocketedBalls().size() == 1); //ball is added to pocket
  }

  SECTION("Test Cue Ball Placement"){
    REQUIRE(game_board_.AddCueBall(glm::vec2(0,0)) == false); // adding a cue ball outside of game board bounds
    REQUIRE(game_board_.AddCueBall(glm::vec2(300,300)) == true); // adding a cue ball inside game board bounds
  }
}



