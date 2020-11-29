#include <core/engine.h>

namespace pool{

  Engine::Engine() {
    player_one_score_ = 0;
    player_two_score_ = 0;

    player_one_turn_ = false;
    player_two_turn_ = false;

    red_ball_player_ = 0;
    blue_ball_player_ = 0;
    has_changed_ = false;
  }

  void Engine::Update(visualizer::Board &game_board){

    if(game_board.GetTurnStatus()==true && has_changed_ == false){
      if(red_ball_player_ == 0){
      // check to see if ball coloring can be assigned
      CalculatePlayerBalls(game_board);
    }
    std::vector<size_t> counts = CountBallTypes(game_board);

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


    if(player_one_turn_){
      // checking to see if a ball of the same color has been pocketed, then dont switch player 1 turn
      if((red_ball_player_ == 1 && counts[0] > 0) || (red_ball_player_ == 2 && counts[1] > 0)){
        player_one_turn_ = !player_one_turn_;
      }
    }

    if(!player_one_turn_){
      // checking to see if a ball of the same color has been pocketed, then dont switch player 2 turn
      if((red_ball_player_ == 1 && counts[1] > 0) || (red_ball_player_ == 2 && counts[0] > 0)){
          player_one_turn_ = !player_one_turn_;
      }
    }

    player_one_turn_ = !player_one_turn_;
    has_changed_ = true;

      // clear out all the balls and analyze them
      // analyze if ball match
    game_board.ClearPocketedBalls();
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
      if(player_one_turn_ == true) {
        red_ball_player_ = 1;
        blue_ball_player_ = 2;
      }
      else {
        red_ball_player_ = 2;
        blue_ball_player_ = 1;
      }
    }
    if (red_count < blue_count) {
      if(player_one_turn_ == true) {
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
    return player_one_turn_;
}

int Engine::GetRedBallPlayer() const {
    return red_ball_player_;
}

int Engine::GetBlueBallPlayer() const {
    return blue_ball_player_;
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
    if(game_board.GetPocketedBalls()[i].GetType() == Type::Cue){
      eight_ball_count ++;
    }
  }
  return {red_count,blue_count,cue_count,eight_ball_count};
  }
}