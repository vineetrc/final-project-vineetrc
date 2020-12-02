#include <visualizer/pool_app.h>

namespace pool {

namespace visualizer {

PoolApp::PoolApp()
    : game_board_(glm::vec2(300, 200), 800 , 350),
    game_engine_(){

  ci::app::setWindowSize(1400, 800);
  update_speed_ = 1; //number of simulation 'steps' gone thru on one update call
}

void PoolApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  std::string player_turn = "player 1";
  if(game_engine_.GetPlayerOneTurn()==false){
    player_turn = "player 2";
  }

  std::string game_message = "Play in progress";
  if(game_board_.GetTurnStatus()==true){
    game_message = "hit the cue ball";
  }
  if(game_engine_.HasCueBallSunk()==true) {
    game_message = "click to place the cue ball";
  }

  if(game_engine_.GetWinCondition()== 1){
    game_message = "player one Wins";
  }

  if(game_engine_.GetWinCondition()== 2){
    game_message = "player two Wins";
  }

  // drawing signs for the player ball racks
  ci::gl::drawStringCentered("player 1",glm::vec2(getWindowWidth()*.075,90),ci::Color("black"));
  ci::gl::drawStringCentered( "player 2",glm::vec2(getWindowWidth()*.925,90),ci::Color("black"));

  // displays message of Winning condition, Player turn, or to place the cue ball due to a foul
  ci::gl::drawStringCentered(game_message,glm::vec2(getWindowWidth()*.5,50),ci::Color("black"));

  // displays which player turn it is
  ci::gl::drawStringCentered(player_turn,glm::vec2(getWindowWidth()*.5,80),ci::Color("black"));

  // drawing ball racks for players
  ci::gl::color(ci::Color("grey"));
  ci::Rectf player_one_rack(getWindowWidth()*.05, getWindowHeight()*.3, getWindowWidth()*.08,
                            getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_one_rack, 20);

  ci::Rectf player_two_rack(getWindowWidth()*.92, getWindowHeight()*.3, getWindowWidth()*.95,
                            getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_two_rack, 20);

  //displaying balls remaining for player one
  ci::gl::color(ci::Color("black"));
  for(int i = 0; i < 8 - game_engine_.GetPlayerOneScore(); i++){
    ci::gl::drawSolidCircle(glm::vec2(getWindowWidth()*.065,
                                      getWindowHeight()*.3 + 20 + i*40), 20);
    ci::gl::color(game_engine_.GetPlayerOneColor());
  }

  // displaying balls remaining for player two
  ci::gl::color(ci::Color("black"));
  for(int i = 0; i < 8 - game_engine_.GetPlayerTwoScore(); i++){
    ci::gl::drawSolidCircle(glm::vec2(getWindowWidth()*.935,
                                      getWindowHeight()*.3 + 20 + i*40), 20);
    ci::gl::color(game_engine_.GetPlayerTwoColor());
  }

  //drawing shot meter
  ci::gl::color(ci::Color("grey"));
  glm::vec2 top_left = ci::vec2(getWindowWidth()*.5-120,getWindowHeight()*.85);
  glm::vec2 bottom_right = ci::vec2(getWindowWidth()*.5+120,getWindowHeight()*.85+50);
  ci::Rectf full_power_bar(top_left, bottom_right);
  ci::gl::drawSolidRoundedRect(full_power_bar, 20);

  ci::gl::color(ci::Color("red"));
  bottom_right.x = getWindowWidth()*.5 - 120 + 240*(force_/max_force_);
  ci::Rectf power_bar(top_left,bottom_right);
  ci::gl::drawSolidRoundedRect(power_bar, 20);
  ci::gl::drawStringCentered(
      std::to_string((force_/max_force_)*100).substr(0,4)+"%",
      glm::vec2(getWindowWidth()*.5,getWindowHeight()*.85+25),ci::Color("black"));

  // draw game_board_
  game_board_.Draw();
}
void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  end_coords_ = event.getPos();
  CalculateForce();
}

void PoolApp::update() {
  if(game_engine_.GetWinCondition()==0) {
    for (size_t i = 0; i < update_speed_; i++) {
      game_board_.Update();
    }
  }
  game_engine_.Update(game_board_);
}

void PoolApp::mouseUp(ci::app::MouseEvent event) {
  if(game_engine_.HasCueBallSunk() == true){
    // allow current player to add the cue ball back to the board
    if(game_board_.AddCueBall(event.getPos())) {
      game_engine_.AddCueBall(); // resets state to no cue ball sunk
    }
  }
  else if(game_engine_.GetWinCondition()==0){
    end_coords_ = event.getPos();
    CalculateForce();
    game_board_.HandleClick(game_board_.GetMouseCoords(), force_);
    force_ = 0;
  }
}

void PoolApp::mouseDown(ci::app::MouseEvent event) {
  //adds particle to simulation
  if(game_engine_.GetWinCondition()==0) {
    // if no winner
    init_coords_ = event.getPos();
  }
}

void PoolApp::mouseMove(ci::app::MouseEvent event) {
  //adds particle to simulation
  if(game_engine_.GetWinCondition()==0) {
    // if no winner
    game_board_.UpdateMousePosition(event.getPos());
  }
}

void PoolApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_DELETE: {
      game_board_.Clear();
      break;
    }

    case ci::app::KeyEvent::KEY_UP: {
      update_speed_++;
      break;
    }

    case ci::app::KeyEvent::KEY_DOWN: {
      if (update_speed_ <= 1) {
        break;
      }
      update_speed_--;
      break;
    }
  }
}

void PoolApp::CalculateForce() {
  force_ = .1 * glm::length(init_coords_ - end_coords_);
  if (force_ > max_force_) {
    force_ = max_force_; // max power for a possible hit
  }
}
}  // namespace visualizer
}  // namespace pool
