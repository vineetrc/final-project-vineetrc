#include <core/engine.h>

namespace pool{

  Engine::Engine() {
    player_one_score_ = 0;
    player_two_score_ = 0;
    is_player_one_turn_ = false;

    red_ball_player_ = 0;
    blue_ball_player_ = 0;

    has_changed_ = false;
    winner_ = 0; // corresponds to no winner
    cue_ball_sunk_ = false;
  }

  void Engine::Update(visualizer::Board &game_board){

    if(game_board.GetTurnStatus()==true && has_changed_ == false){
      if(red_ball_player_ == 0){
      // check to see if ball coloring can be assigned
      CalculatePlayerBalls(game_board);
    }
    std::vector<size_t> counts = CountBallTypes(game_board);

    CheckForWinner(counts);

    UpdateScores(counts);

    if(counts[2] == 1){
        // cue ball has been sunk
        cue_ball_sunk_ = true;
    }

    CheckForExtraTurn(counts);

    is_player_one_turn_ = !is_player_one_turn_; // flip player turn
    has_changed_ = true;

    game_board.ClearPocketedBalls(); // clear out all the balls after updating player states
    }

    if(game_board.GetTurnStatus()==false){
      // board is still running a turn
      has_changed_ = false;
    }
  }

void Engine::CalculatePlayerBalls(visualizer::Board &game_board) {
  // assigns the player what ball they will have for the rest of the game, based on first sunk ball
  size_t red_count = 0;
  size_t blue_count = 0;
  for(size_t i = 0; i< game_board.GetPocketedBalls().size(); i++){
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Red){
      red_count ++;
    }
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Blue){
      blue_count ++;
    }
  }
  if(red_count + blue_count !=0) {
    if (red_count >= blue_count) {
      if(is_player_one_turn_ == true) {
        red_ball_player_ = 1;
        blue_ball_player_ = 2;
      }
      else {
        red_ball_player_ = 2;
        blue_ball_player_ = 1;
      }
    }
    if (red_count < blue_count) {
      if(is_player_one_turn_ == true) {
        red_ball_player_ = 2;
        blue_ball_player_ = 1;
      }
      else{
        red_ball_player_ = 1;
        blue_ball_player_ = 2;
      }
    }
  }
}

int Engine::GetPlayerOneScore() const {
    return player_one_score_;
}

int Engine::GetPlayerTwoScore() const {
  return player_two_score_;
}

bool Engine::GetPlayerOneTurn() const {
    return is_player_one_turn_;
}

int Engine::GetRedBallPlayer() const {
    return red_ball_player_;
}

int Engine::GetBlueBallPlayer() const {
    return blue_ball_player_;
}

ci::Color Engine::GetPlayerOneColor() const {
  if(red_ball_player_ == 0){
    return "grey";
  }
  if(red_ball_player_ == 1){
    return "red";
  }
  return "blue";
}

ci::Color Engine::GetPlayerTwoColor() const {
  if(red_ball_player_ == 0){
    return "grey";
  }
  if(red_ball_player_ == 2){
    return "red";
  }
  return "blue";
}

int Engine::GetWinCondition() const {
    return winner_;
}

bool Engine::HasCueBallSunk() const {
    return cue_ball_sunk_;
}

void Engine::AddCueBall() {
  cue_ball_sunk_ = false;
}

void Engine::UpdateScores(std::vector<size_t> &counts) {
  if(red_ball_player_ == 1) {
    // player one has control of red balls
    player_one_score_ += counts[0]; // adding red balls to player one
    player_two_score_ +=counts[1]; // adding blue balls to player two
  }
  if(red_ball_player_ == 2) {
    // player two has control of red balls
    player_one_score_ += counts[1]; // adding blue balls to player one
    player_two_score_ +=counts[0]; // adding red balls to player two
  }
}

void Engine::CheckForWinner(std::vector<size_t> &counts) {
  // determines if player one wins due to pocketing 8 ball
  if(is_player_one_turn_){
    if(counts[3] == 1){
      if(player_one_score_ == 7){
        player_one_score_++;
        winner_ = 1;
      }
      else{
        winner_ = 2;
      }
    }
  }

  // determines if player two wins due to pocketing 8 ball
  if(!is_player_one_turn_){
    if(counts[3] == 1){
      if(player_two_score_ == 7){
        player_two_score_++;
        winner_ = 2;
      }
      else{
        winner_ = 1;
      }
    }
  }
}

void Engine::CheckForExtraTurn(std::vector<size_t> &counts) {
  if(cue_ball_sunk_ == false) {
    //if no cue ball foul has occured, the player's turn may stay the same may be switched
    if (is_player_one_turn_) {
      // checking to see if a ball of the same color has been pocketed, then dont switch player 1 turn
      if ((red_ball_player_ == 1 && counts[0] > 0) || (red_ball_player_ == 2 && counts[1] > 0)) {
        is_player_one_turn_ = !is_player_one_turn_; // keep the same player's turn
      }
    }

    if (!is_player_one_turn_) {
      // checking to see if a ball of the same color has been pocketed, then dont switch player 2 turn
      if ((red_ball_player_ == 1 && counts[1] > 0) || (red_ball_player_ == 2 && counts[0] > 0)) {
        is_player_one_turn_ = !is_player_one_turn_; // keep the same player's turn
      }
    }
  }
}

std::vector<size_t> Engine::CountBallTypes(visualizer::Board &game_board) {
  size_t red_count = 0;
  size_t blue_count = 0;
  size_t cue_count = 0;
  size_t eight_ball_count = 0;
  for(size_t i = 0; i< game_board.GetPocketedBalls().size(); i++){
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Red){
      red_count ++;
    }
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Blue){
      blue_count ++;
    }
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Cue){
      cue_count ++;
    }
    if(game_board.GetPocketedBalls()[i].GetType() == Type::EightBall){
      eight_ball_count ++;
    }
  }
  return {red_count,blue_count,cue_count,eight_ball_count};
  }
}

