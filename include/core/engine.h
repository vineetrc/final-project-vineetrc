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

 private:

  bool player_one_turn_;

  bool has_changed_;

  bool player_two_turn_;

  int player_one_score_; //starts at 0 and goes till 8 represents every ball pocketed

  int player_two_score_;

  int red_ball_player_; // represents what player (1 or 2) the ball color is assigned to, 0 if unassigned

  int blue_ball_player_;

  //int has_winner; // returns 1 or 2 depending on which player won, 0 if there is no winner

  void CalculatePlayerBalls(visualizer::Board &game_board);

  std::vector<size_t> CountBallTypes(visualizer::Board &game_board);

};

}

