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
  std::string power = "n/a";
  if(game_board_.GetTurnStatus()==true){
    game_message = "hit the cue ball";
    power = std::to_string(force_);
  }

  //game stats mainly for manual testing
  ci::gl::drawStringCentered(std::to_string(game_engine_.GetRedBallPlayer()) + "red",glm::vec2(getWindowWidth()*.4,90),ci::Color("black"));
  ci::gl::drawStringCentered(std::to_string(game_engine_.GetBlueBallPlayer()) + "blue",glm::vec2(getWindowWidth()*.6,90),ci::Color("black"));
  ci::gl::drawStringCentered(std::to_string(game_engine_.GetPlayerOneScore()),glm::vec2(getWindowWidth()*.4,100),ci::Color("black"));
  ci::gl::drawStringCentered(std::to_string(game_engine_.GetPlayerTwoScore()),glm::vec2(getWindowWidth()*.6,100),ci::Color("black"));


  ci::gl::drawStringCentered(game_message,glm::vec2(getWindowWidth()*.5,50),ci::Color("black"));
  ci::gl::drawStringCentered(power,glm::vec2(getWindowWidth()*.5,70),ci::Color("black"));
  ci::gl::drawStringCentered(player_turn,glm::vec2(getWindowWidth()*.5,80),ci::Color("black"));

  // drawing ball racks for players
  ci::gl::color(ci::Color("grey"));
  ci::Rectf player_one_rack(getWindowWidth()*.05, getWindowHeight()*.3, getWindowWidth()*.08, getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_one_rack, 20);

  ci::Rectf player_two_rack(getWindowWidth()*.92, getWindowHeight()*.3, getWindowWidth()*.95, getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_two_rack, 20);

  ci::gl::color(ci::Color("red"));
  for(int i = 0; i < 7 - game_engine_.GetPlayerOneScore(); i++){
    ci::gl::drawSolidCircle(glm::vec2(getWindowWidth()*.065, getWindowHeight()*.3 + 20 + i*40), 20);
  }
  for(int i = 0; i < 7 - game_engine_.GetPlayerTwoScore(); i++){
    ci::gl::color(ci::Color("blue"));
    ci::gl::drawSolidCircle(glm::vec2(getWindowWidth()*.935, getWindowHeight()*.3 + 20 + i*40), 20);
  }


  //drawing shot meter
  ci::gl::color(ci::Color("grey"));
  glm::vec2 top_left = ci::vec2(getWindowWidth()*.5-120,getWindowHeight()*.85);
  glm::vec2 bottom_right = ci::vec2(getWindowWidth()*.5+120,getWindowHeight()*.85+50);
  ci::Rectf full_power_bar(top_left, bottom_right);
  ci::gl::drawSolidRoundedRect(full_power_bar, 20);

  ci::gl::color(ci::Color("red"));
  bottom_right.x = getWindowWidth()*.5 - 120 + 240*(force_/20.0);
  ci::Rectf power_bar(top_left,bottom_right);
  ci::gl::drawSolidRoundedRect(power_bar, 20);
  ci::gl::drawStringCentered(std::to_string(force_/20.0).substr(0,4)+"%",glm::vec2(getWindowWidth()*.5,getWindowHeight()*.85+25),ci::Color("black"));


  game_board_.Draw();
}
void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  //game_board_.HandleClick(event.getPos());


  end_coords_ = event.getPos();

  force_ = .1*glm::length(init_coords_-end_coords_);
  if(force_>20.0){
    force_ = 20.0;
  }
  //game_board_.UpdateMousePosition(event.getPos());
}

void PoolApp::update() {
  for (size_t i = 0; i < update_speed_; i++) {
    game_board_.Update();
  }

  game_engine_.Update(game_board_);
}

void PoolApp::mouseUp(ci::app::MouseEvent event) {
  end_coords_ = event.getPos();

  force_ = .1*glm::length(init_coords_-end_coords_);
  if(force_>20.0){
    force_ = 20.0; // max power for a possible hit
  }

  game_board_.HandleClick(game_board_.GetMouseCoords(), force_);
  force_ = 0;
}

void PoolApp::mouseDown(ci::app::MouseEvent event) {
  //adds particle to simulation
  init_coords_ = event.getPos();
}

void PoolApp::mouseMove(ci::app::MouseEvent event) {
  //adds particle to simulation
  game_board_.UpdateMousePosition(event.getPos());
}

void PoolApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_r: {
      break;
    }

    case ci::app::KeyEvent::KEY_g: {
      break;
    }

    case ci::app::KeyEvent::KEY_b: {
      break;
    }

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
}  // namespace visualizer
}  // namespace pool
