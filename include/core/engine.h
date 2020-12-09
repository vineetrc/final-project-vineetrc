#pragma once
#include "visualizer/board.h"

namespace pool {
class Engine {
 public:

  /**
   * Initializes a Game Engine that represents a Pool Game start state
   */
  Engine();

  /**
   * Updates Player Stats and Game Progression
   */
  void Update(visualizer::Board& game_board);

  /**
   * Returns true if player 1 turn, false if player 2 turn
   */
  bool GetPlayerOneTurn() const;

  /**
   * Returns 1 or 2 depending on which player is a red-ball player. 0, if no coloring has been assigned
   */
  size_t GetRedBallPlayer() const;

  /**
   * Sets player Ball coloring, for testing purposes
   */
  void SetRedBallPlayer(int player_num);

  /**
   * Returns player 1 score
   */
  size_t GetPlayerOneScore() const;

  /**
   * Returns player 2 score
   */
  size_t GetPlayerTwoScore() const;

  /**
   * Returns the color of the Player 1's ball. Returns grey if ball color is not assigned yet
   */
  ci::Color GetPlayerOneColor() const;

  /**
   * Returns the color of the Player 2's ball. Returns grey if ball color is not assigned yet
   */
  ci::Color GetPlayerTwoColor() const;

  /**
   * Returns 0 for no winner, 1 for player 1 win, 2 for player 2 win
   */
  size_t GetWinCondition() const;

  /**
   * Returns true if cue ball was pocketed
   */
  bool HasCueBallSunk() const;

  /**
   * Returns true if player didn't hit their assigned color on their turn
   */
  bool HasIncorrectColorHit() const;

  /**
   *  Lets Engine know that a cue ball has been added to the board
   */
  void AddCueBall();

  /**
   * Setter to allow player turn iteration, used for testing purposes
   */
  void SetHasChanged(bool val);

  /**
   * Sets player's scores, for testing purposes
   */
  void SetPlayerScore(size_t player_one, size_t player_two);

  /**
   * Returns a Game Message depending on the state of the game score and situation
   */
  std::string GetGameMessage(const visualizer::Board& game_board) const;

  size_t GetWinningScore() const;

 private:

  bool is_player_one_turn_; // true if it is player one's turn, false for player two

  bool has_changed_; // update variable so engine runs at start of each player turn

  size_t player_one_score_; // starts at 0 and goes till 8 represents every ball pocketed

  size_t player_two_score_; // starts at 0 and goes till 8 represents every ball pocketed

  size_t red_ball_player_; // represents what player (1 or 2) the red ball color is assigned to, 0 if unassigned

  size_t winner_; // returns 1 or 2 depending on which player won, 0 if there is no winner

  bool has_cue_ball_sunk_; // true if the cue ball has been sunk on a turn

  bool hit_same_color_ball_; // true if player cue hit wrong ball color first

  bool is_first_turn_; // returns true if its the first turn of the game

  const size_t winning_score_ = 8;
  /**
   * counts all the different types of pocketed balls in a run
   */
  std::vector<size_t> CountBallTypes(visualizer::Board& game_board) const;

  /**
   * determines which ball color corresponds to each player based on first sunk ball
   */
  void CalculatePlayerBalls(visualizer::Board& game_board);

  /**
   * Updates the scores of each player based on the pocketed balls
   */
  void UpdateScores(const std::vector<size_t>& counts);

  /**
   * Checks for a winning condition after each turn
   */
  void CheckForWinner(const std::vector<size_t>& counts);

  /**
   * Determines if there is an extra turn due to the player pocketing a ball of their color already
   */
  void CheckForExtraTurn(const std::vector<size_t>& counts);

  /**
   * Determines if the player hit the wrong color ball in the last turn
   */
  void CheckIncorrectColorHit(visualizer::Board& game_board, bool balls_assigned_this_turn);
};
}

