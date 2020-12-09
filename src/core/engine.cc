#include <core/engine.h>

namespace pool{

  Engine::Engine() {
    player_one_score_ = 0;
    player_two_score_ = 0;
    is_player_one_turn_ = false;
    red_ball_player_ = 0;
    has_changed_ = false;
    winner_ = 0; // corresponds to no winner
    has_cue_ball_sunk_ = false;
    hit_same_color_ball_ = false;
    is_first_turn_ = true;
  }

  void Engine::Update(visualizer::Board& game_board){
    if (game_board.GetTurnStatus() && !has_changed_){
      hit_same_color_ball_ = false;
      bool balls_assigned_this_turn = false;

      if (red_ball_player_ == 0){
      // check to see if ball coloring can be assigned
        CalculatePlayerBalls(game_board);
        if (red_ball_player_ != 0){
          balls_assigned_this_turn = true;
        }
      }

      std::vector<size_t> counts = CountBallTypes(game_board);

      CheckForWinner(counts);

      UpdateScores(counts);

      if (counts[2] == 1 && winner_ == 0){
        // cue ball has been sunk
        has_cue_ball_sunk_ = true;
      }

      CheckIncorrectColorHit(game_board, balls_assigned_this_turn);

      if (!has_cue_ball_sunk_) {
        // if the cue ball is still in the board, reset its first_ball_collision parameter
        game_board.GetGameBalls().at(0).ClearFirstBallCollision();
      }

      CheckForExtraTurn(counts);

      //is_player_one_turn_prev = is_player_one_turn_;
      is_player_one_turn_ = !is_player_one_turn_; // flip player turn
      has_changed_ = true;

      game_board.ClearPocketedBalls(); // clear out all the balls after updating player states
    }

    if (!game_board.GetTurnStatus()){
      // board is still running a turn
      has_changed_ = false;
    }

    is_first_turn_ = false; // after first iteration of game play, it is no longer the first turn
  }

void Engine::CalculatePlayerBalls(visualizer::Board& game_board) {
  // assigns the player what ball they will have for the rest of the game, based on first sunk ball
  size_t red_count = 0;
  size_t blue_count = 0;
  for (size_t i = 0; i < game_board.GetPocketedBalls().size(); i++){
    if (game_board.GetPocketedBalls()[i].GetType() == Type::Red){
      red_count ++;
    }
    if (game_board.GetPocketedBalls()[i].GetType() == Type::Blue){
      blue_count ++;
    }
  }
  if (red_count + blue_count != 0) {
    if (red_count >= blue_count) {
      if (is_player_one_turn_) {
        red_ball_player_ = 1;
      }
      else {
        red_ball_player_ = 2;
      }
    }
    if (red_count < blue_count) {
      if (is_player_one_turn_) {
        red_ball_player_ = 2;
      }
      else {
        red_ball_player_ = 1;
      }
    }
  }
}

size_t Engine::GetPlayerOneScore() const {
  return player_one_score_;
}

size_t Engine::GetPlayerTwoScore() const {
  return player_two_score_;
}

bool Engine::GetPlayerOneTurn() const {
  return is_player_one_turn_;
}

size_t Engine::GetRedBallPlayer() const {
  return red_ball_player_;
}

void Engine::SetRedBallPlayer(int player_num) {
  red_ball_player_ = player_num;
}

ci::Color Engine::GetPlayerOneColor() const {
  if (red_ball_player_ == 0){
    return "grey";
  }
  if (red_ball_player_ == 1){
    return "red";
  }
  return "blue";
}

ci::Color Engine::GetPlayerTwoColor() const {
  if (red_ball_player_ == 0){
    return "grey";
  }
  if (red_ball_player_ == 2){
    return "red";
  }
  return "blue";
}

size_t Engine::GetWinCondition() const {
  return winner_;
}

bool Engine::HasCueBallSunk() const {
  return has_cue_ball_sunk_;
}

bool Engine::HasIncorrectColorHit() const {
  return hit_same_color_ball_;
}

void Engine::AddCueBall() {
  has_cue_ball_sunk_ = false;
}

void Engine::UpdateScores(const std::vector<size_t>& counts) {
  if (red_ball_player_ == 1) {
    // player one has control of red balls
    player_one_score_ += counts[0]; // adding red balls to player one
    player_two_score_ += counts[1]; // adding blue balls to player two
  }
  if (red_ball_player_ == 2) {
    // player two has control of red balls
    player_one_score_ += counts[1]; // adding blue balls to player one
    player_two_score_ += counts[0]; // adding red balls to player two
  }
}

void Engine::CheckForWinner(const std::vector<size_t>& counts) {
  // determines if player one wins due to pocketing 8 ball
  if (is_player_one_turn_){
    if (counts[3] == 1){
      // winner by sinking 8ball correctly
      if (player_one_score_ == winning_score_-1){
        player_one_score_++;
        winner_ = 1;
      }
      else {
        // loser by fouling
        winner_ = 2;
      }
    }
  }

  // determines if player two wins due to pocketing 8 ball
  if (!is_player_one_turn_){
    if (counts[3] == 1){
      if (player_two_score_ == winning_score_-1){
        // winner by sinking 8bal
        player_two_score_++;
        winner_ = 2;
      }
      else {
        // loser by fouling
        winner_ = 1;
      }
    }
  }
}

void Engine::CheckForExtraTurn(const std::vector<size_t>& counts) {
  if (!has_cue_ball_sunk_ && !hit_same_color_ball_) {
    //if no cue ball foul has occured, the player's turn may stay the same may be switched
    if (is_player_one_turn_) {
      // checking to see if a ball of the same color has been pocketed, then dont switch player 1 turn
      if ((red_ball_player_ == 1 && counts[0] > 0) || (red_ball_player_ == 2 && counts[1] > 0)) {
        is_player_one_turn_ = !is_player_one_turn_; // keep the same player's turn
      }
    }
    else if (!is_player_one_turn_) {
      // checking to see if a ball of the same color has been pocketed, then dont switch player 2 turn
      if ((red_ball_player_ == 1 && counts[1] > 0) || (red_ball_player_ == 2 && counts[0] > 0)) {
        is_player_one_turn_ = !is_player_one_turn_; // keep the same player's turn
      }
    }
  }
}

std::vector<size_t> Engine::CountBallTypes(visualizer::Board& game_board) const{
  size_t red_count = 0;
  size_t blue_count = 0;
  size_t cue_count = 0;
  size_t eight_ball_count = 0;

  for (size_t i = 0; i < game_board.GetPocketedBalls().size(); i++){
    if (game_board.GetPocketedBalls()[i].GetType() == Type::Red){
      red_count ++;
    }
    if (game_board.GetPocketedBalls()[i].GetType() == Type::Blue){
      blue_count ++;
    }
    if (game_board.GetPocketedBalls()[i].GetType() == Type::Cue){
      cue_count ++;
    }
    if (game_board.GetPocketedBalls()[i].GetType() == Type::EightBall){
      eight_ball_count ++;
    }
  }
  return {red_count, blue_count, cue_count, eight_ball_count};
}

void Engine::CheckIncorrectColorHit(visualizer::Board& game_board, bool balls_assigned_this_turn) {
  if (!has_cue_ball_sunk_ && red_ball_player_ == 0 && winner_ == 0 && !is_first_turn_){
    // player can only not hit 8ball at the beginning of the game
    hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() == Type::EightBall;
  }

  if (!has_cue_ball_sunk_ && red_ball_player_ != 0 && winner_ == 0 && !balls_assigned_this_turn){
    // checks for ball type when player is on verge of winning and already pocketed all balls of their color
    if (is_player_one_turn_ && player_one_score_ == 7){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::EightBall;
    }
    else if (!is_player_one_turn_ && player_two_score_ == 7){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::EightBall;
    }
    // checks if ball hits the wrong color
    else if (red_ball_player_ == 1 && is_player_one_turn_){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::Red;
    }
    else if (red_ball_player_ == 1 && !is_player_one_turn_){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::Blue;
    }
    else if (red_ball_player_ == 2 && is_player_one_turn_){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::Blue;
    }
    else if (red_ball_player_ == 2 && !is_player_one_turn_){
      hit_same_color_ball_ = game_board.GetGameBalls().at(0).GetFirstBallCollision() != Type::Red;
    }
  }
}

void Engine::SetHasChanged(bool val) {
  has_changed_ = val;
}

void Engine::SetPlayerScore(size_t player_one, size_t player_two) {
  player_one_score_ = player_one;
  player_two_score_ = player_two;
}

std::string Engine::GetGameMessage(const visualizer::Board& game_board) const{
  std::string game_message = "Play in progress";

  if (game_board.GetTurnStatus()){
    game_message = "Hit the cue ball";
  }

  if (HasIncorrectColorHit()) {
    game_message = "Foul. Previous Player Didn't Hit Their Applicable Ball Color. \n Next Player hit the cue ball";
  }

  if (HasCueBallSunk()) {
    game_message = "Click to place the cue ball";
  }

  if (GetWinCondition()== 1){
    game_message = "Player One Wins";
  }

  if (GetWinCondition()== 2){
    game_message = "Player Two Wins";
  }

  return game_message;
}

size_t Engine::GetWinningScore() const {
    return winning_score_;
}
}

