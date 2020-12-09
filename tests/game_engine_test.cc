#include <visualizer/board.h>
#include <core/ball.h>
#include <core/engine.h>
#include <catch2/catch.hpp>
#include <cmath>
using glm::vec2;

TEST_CASE("Game Engine Tests") {
  //gameboard
  pool::visualizer::Board game_board_(glm::vec2(200, 200), 800, 300);
  SECTION("Test Engine Initialization") {
    pool::Engine game_engine;
    REQUIRE(game_engine.GetWinCondition()==0); // check to see if there is no winner
    REQUIRE(game_engine.GetPlayerOneScore()==0);
    REQUIRE(game_engine.GetPlayerTwoScore()==0);
  }

  SECTION("Test Player Turn Alternation") {
    pool::Engine game_engine;
    game_engine.Update(game_board_);
    REQUIRE(game_engine.GetPlayerOneTurn() == true); // player one turn
    game_board_.SetTurnStatus(true); // next playable turn
    game_engine.SetHasChanged(false); // set has_changed to false because of new turn
    game_engine.Update(game_board_);
    REQUIRE(game_engine.GetPlayerOneTurn() == false); // player two turn
  }

  SECTION("Test The Setting of Player Ball Coloring") {
    SECTION("Single Ball Pocketed") {
      pool::Engine game_engine;
      game_engine.Update(game_board_);
      REQUIRE(game_engine.GetPlayerOneTurn() == true); // currently player 1 turn
      std::vector<pool::Ball> list;
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("red"), pool::Type::Red));
      game_board_.SetPocketedBalls(list); // simulate a pocket of a red ball

      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_); // update board state
      REQUIRE(game_engine.GetRedBallPlayer() == 1); // player two ball color is red
    }

    SECTION("Equal number of Colors Pocketed") {
      pool::Engine game_engine;
      REQUIRE(game_engine.GetPlayerOneTurn() == false); // currently player 2 turn
      std::vector<pool::Ball> list;
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("red"), pool::Type::Red));
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("blue"), pool::Type::Blue));

      game_board_.SetPocketedBalls(list); // simulate a pocket of a red ball

      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_); // update board state
      REQUIRE(game_engine.GetRedBallPlayer() == 2); // default in case of 'tie' player two ball color is red
    }

    SECTION("Uneven number of Colors Pocketed") {
      pool::Engine game_engine;
      REQUIRE(game_engine.GetPlayerOneTurn() == false); // currently player 2 turn
      std::vector<pool::Ball> list;
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("red"), pool::Type::Red));
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("blue"), pool::Type::Blue));
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("blue"), pool::Type::Blue));

      game_board_.SetPocketedBalls(list); // simulate a pocket of a red ball

      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_); // update board state
      // take the ball color of the current player to be what has been sunk more: Blue Ball in this case
      REQUIRE(game_engine.GetBlueBallPlayer() == 2);
    }
  }

  SECTION("Test Win Conditions"){
    // vector representation of the number of each type of pocketed balls: {#red, #blue, #cue, #8ball}

    SECTION("Test No Winner"){
      pool::Engine game_engine;
      game_engine.Update(game_board_); // currently player 1 turn
      REQUIRE(game_engine.GetWinCondition() == 0); // no current winner
    }

    SECTION("Test Illegal EightBall Pocketing Win") {
      pool::Engine game_engine;
      game_engine.Update(game_board_); // currently player 1 turn

      std::vector<size_t> ball_counts = {0, 0, 0, 1}; // simulation of an eightball pocket
      game_engine.CheckForWinner(ball_counts);
      REQUIRE(game_engine.GetWinCondition() == 2); // winner is player 2 b/c player 1 pocketed 8ball before other balls
    }

    SECTION("Test Legal EightBall Pocketing Win") {
      // test when player sinks all their balls and the eightball
      pool::Engine game_engine;
      game_engine.Update(game_board_); // currently player 1 turn
      game_engine.SetPlayerScore(7,2);
      std::vector<size_t> ball_counts = {0, 0, 0, 1}; // simulation of an eightball pocket
      game_engine.CheckForWinner(ball_counts);
      REQUIRE(game_engine.GetWinCondition() == 1); // winner is player 1 b/c player 1 pocketed 8ball after colored balls
    }
  }

  SECTION("Test Turn Switching When a Ball is Pocketed"){
      pool::Engine game_engine;
      game_engine.Update(game_board_); // currently player 1 turn

      SECTION("Test Turn Switching On a Pocket of the player's own color") {
        std::vector<size_t> ball_counts = {1, 0, 0, 0}; // pocketing of a ball means player gets extra turn
        game_engine.CheckForExtraTurn(ball_counts);
        REQUIRE(game_engine.GetPlayerOneTurn() == true); // remains player one turn
      }

      SECTION("Test Turn Switching On a Pocket of the opponent's color") {
        game_board_.SetTurnStatus(true);
        game_engine.SetHasChanged(false);
        game_engine.Update(game_board_); // update gameboard
        std::vector<size_t> ball_counts = {0, 2, 0, 0}; // pocketing of a ball means player gets extra turn
        game_engine.CheckForExtraTurn(ball_counts);
        REQUIRE(game_engine.GetPlayerOneTurn() == false); // switches to player two turn
      }
  }
}
