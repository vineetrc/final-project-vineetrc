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

  SECTION("Test The Initial Setting of Player Ball Coloring") {

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
      REQUIRE(game_engine.GetRedBallPlayer() == 1); // RedBallPlayer is player 1, means Player 2 is blue player
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
      // when player accidentally sinks 8ball before sinking al their own balls, leading to the opponent winning
      pool::Engine game_engine;
      game_engine.Update(game_board_); // player 1's turn
      std::vector<pool::Ball> list;
      // represents Player 1 pocketing of eightball
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("black"), pool::Type::EightBall));

      game_board_.SetPocketedBalls(list);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_); // now updates game state in next loop

      REQUIRE(game_engine.GetWinCondition() == 2); // winner is player 2 b/c player 1 pocketed 8ball before other balls
    }

    SECTION("Test Legal EightBall Pocketing Win") {
      // test when player sinks all their balls and the eightball
      pool::Engine game_engine;
      game_engine.Update(game_board_); // currently player 1 turn
      game_engine.SetPlayerScore(7,2); // player one pocketed all his 7 colored balls

      std::vector<pool::Ball> list;
      list.push_back(pool::Ball(vec2(5, 2),5,ci::Color("black"),
                                pool::Type::EightBall)); // pockets 8ball as his last ball

      game_board_.SetPocketedBalls(list);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_);
      REQUIRE(game_engine.GetWinCondition() == 1); // winner is player 1 b/c player 1 pocketed 8ball after colored balls
    }
  }

  SECTION("Test Turn Switching When a Ball is Pocketed"){

      SECTION("Test Turn Switching On a Pocket of the Player's own color") {
        pool::Engine game_engine;
        game_engine.Update(game_board_); // currently player 1 turn
        std::vector<pool::Ball> list;
        // represents Player 1 pocketing of eightball
        list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("black"), pool::Type::Blue));

        game_board_.SetPocketedBalls(list);
        game_board_.SetTurnStatus(true);
        game_engine.SetHasChanged(false);
        game_engine.Update(game_board_);
        REQUIRE(game_engine.GetRedBallPlayer() == 2);
        REQUIRE(game_engine.GetPlayerOneTurn() == true); // remains player one turn
      }

      SECTION("Test Turn Switching On a Pocket of the opponent's color") {
        // next turn player one pockets red ball when they are a blue ball player
        pool::Engine game_engine;
        game_engine.Update(game_board_); // player 1's turn
        game_engine.SetRedBallPlayer(2); // player 1 is a blue ball player
        std::vector<pool::Ball> list;
        list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("blue"), pool::Type::Red));

        game_board_.SetPocketedBalls(list);
        game_board_.SetTurnStatus(true);
        game_engine.SetHasChanged(false);
        game_engine.Update(game_board_); // update gameboard
        REQUIRE(game_engine.GetPlayerOneTurn() == false); // switches to player two turn due to incorrect ball pocketing
      }
  }

  SECTION("Test Cue Ball Pocketing"){
    pool::Engine game_engine;
    game_engine.Update(game_board_); // currently player 1 turn

    std::vector<pool::Ball> list;
    list.push_back(pool::Ball(vec2(5, 2), 5,
                              ci::Color("black"), pool::Type::Cue)); // simulate a pocket of the cue ball
    game_board_.SetPocketedBalls(list);
    game_board_.SetTurnStatus(true);
    game_engine.SetHasChanged(false);
    game_engine.Update(game_board_); // update gameboard

    REQUIRE(game_engine.HasCueBallSunk() == true); // condition is true
  }

  SECTION("Test Incorrect Color Ball Collision Foul Situations"){
    // testing cases in which players hit or don't hit their applicable ball color

    SECTION("Test Incorrect Ball Collision Foul When Only 8ball left") {
      // tests situation when player 1 has sunk all their balls
      pool::Engine game_engine;
      game_engine.SetPlayerScore(7,1); // player 1 has sunk all their balls
      game_engine.SetRedBallPlayer(1); // make player one's color red
      game_engine.Update(game_board_); // player 1 turn

      // On player 1's turn first collision is with an 8ball
      game_board_.GetGameBalls().at(0).SetFirstBallCollision(pool::Type::EightBall);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_);
      REQUIRE(game_engine.HasIncorrectColorHit() == false); // 8ball hit is now acceptable, as its the last ball left to win

      // player 2 turn
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_);

      // player 1 turn again

      // cue ball first hits blue ball which is the opponents ball color
      game_board_.GetGameBalls().at(0).SetFirstBallCollision(pool::Type::Blue);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_);

      // Foul because player 1 hits blue color ball as a Red color player
      REQUIRE(game_engine.HasIncorrectColorHit() == true);
    }

    SECTION("Test General Case Incorrect Ball Collision Foul") {
      pool::Engine game_engine;
      game_engine.SetRedBallPlayer(1); // make player one's color red
      game_engine.Update(game_board_); // player 1 turn

      game_board_.GetGameBalls().at(0).SetFirstBallCollision(pool::Type::Blue); // first collision is blue ball
      std::vector<pool::Ball> list;
      list.push_back(pool::Ball(vec2(5, 2), 5,
                                ci::Color("black"), pool::Type::Red)); // player sinks their own ball

      game_board_.SetPocketedBalls(list);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_); // update gameboard
      REQUIRE(game_engine.HasIncorrectColorHit() == true);
      REQUIRE(game_engine.GetPlayerOneTurn() == false); // switches players, due to foul, regardless of circumstance

      // player 2 hits blue ball first, which is their own color
      game_board_.GetGameBalls().at(0).SetFirstBallCollision(pool::Type::Blue);
      game_board_.SetTurnStatus(true);
      game_engine.SetHasChanged(false);
      game_engine.Update(game_board_);
      REQUIRE(game_engine.HasIncorrectColorHit() == false); // no foul due to proper ball color hit
    }
  }

  SECTION("Test Engine Scoring"){
    pool::Engine game_engine;
    game_engine.Update(game_board_); // currently player 1 turn
    std::vector<pool::Ball> list;
    list.push_back(pool::Ball(vec2(5, 2), 5,
                              ci::Color("black"), pool::Type::Blue)); // simulate a pocket of the cue ball
    list.push_back(pool::Ball(vec2(5, 2), 5,
                              ci::Color("blue"), pool::Type::Red));
    list.push_back(pool::Ball(vec2(5, 2), 5,
                              ci::Color("blue"), pool::Type::Blue));
    game_board_.SetPocketedBalls(list);

    game_board_.SetTurnStatus(true);
    game_engine.SetHasChanged(false);
    game_engine.Update(game_board_); // update gameboard

    REQUIRE(game_engine.GetPlayerOneScore() == 2); // Player One is blue, 2 blue balls sunk, score = 2
    REQUIRE(game_engine.GetPlayerTwoScore() == 1); // Player Two is red, 1 red ball sunk, score = 1
  }
}
