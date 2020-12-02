#pragma once
#ifndef NAIVE_BAYES_INCLUDE_CORE_ENGINE_H_
#define NAIVE_BAYES_INCLUDE_CORE_ENGINE_H_
#endif //NAIVE_BAYES_INCLUDE_CORE_ENGINE_H_
#include "visualizer/board.h"

namespace pool {
class Engine {
 public:

  Engine();

  void Update(visualizer::Board& game_board);

  bool GetPlayerOneTurn() const;

  int GetRedBallPlayer() const;

  int GetBlueBallPlayer() const;

  int GetPlayerOneScore() const;

  int GetPlayerTwoScore() const;

  ci::Color GetPlayerOneColor() const;

  ci::Color GetPlayerTwoColor() const;

  int GetWinCondition() const;

  bool HasCueBallSunk() const; // returns true or false depending on if cue ball has been sunk

  void AddCueBall(); // changes variables to let engine know that cue ball has been placed

 private:

  bool is_player_one_turn_; // true if it is player one's turn, false for player two

  // update variable to make sure game engine calculates the new situation at the begining of each players turn
  bool has_changed_;

  int player_one_score_; // starts at 0 and goes till 8 represents every ball pocketed

  int player_two_score_; // starts at 0 and goes till 8 represents every ball pocketed

  int red_ball_player_; // represents what player (1 or 2) the red ball color is assigned to, 0 if unassigned

  int blue_ball_player_; // represents what player (1 or 2) the blue ball color is assigned to, 0 if unassigned

  int winner_; // returns 1 or 2 depending on which player won, 0 if there is no winner

  bool cue_ball_sunk_; // true if the cue ball has been sunk on a turn

  /**
   * determines which ball color corresponds to each player based on first sunk ball
   */
  void CalculatePlayerBalls(visualizer::Board &game_board);

  /**
   * counts all the different types of pocketed balls in a run
   */
  std::vector<size_t> CountBallTypes(visualizer::Board &game_board);

  /**
   * Updates the scores of each player based on the pocketed balls
   */
  void UpdateScores(std::vector<size_t>& counts);

  /**
   * Checks for a winning condition after each turn
   */
  void CheckForWinner(std::vector<size_t>& counts);

  /**
   * Determines if there is an extra turn due to the player pocketing a ball of their color already
   */
  void CheckForExtraTurn(std::vector<size_t>& counts);
};
}

