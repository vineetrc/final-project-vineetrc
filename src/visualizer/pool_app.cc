#include <visualizer/pool_app.h>

namespace pool {
namespace visualizer {

PoolApp::PoolApp()
    : game_board_(glm::vec2(300, 200), 800 , 350),
    game_engine_(){

  ci::app::setWindowSize(window_size_x_, window_size_y_);
  update_speed_ = 1; //number of simulation 'steps' gone thru on one update call
  is_start_screen_ = true;
}

void PoolApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  if (!is_start_screen_) {
    // displays game screen
    std::string player_turn = "Player 1";
    if (!game_engine_.GetPlayerOneTurn()) {
      player_turn = "Player 2";
    }

    // displays message of Winning condition, Player turn, or to place the cue ball due to a foul
    ci::gl::drawStringCentered(game_engine_.GetGameMessage(game_board_),
                               glm::vec2(getWindowWidth() * .5, 50),
                               ci::Color("black"));

    // displays which player turn it is
    if (game_engine_.GetWinCondition() == 0) {
      ci::gl::drawStringCentered(player_turn + " turn",
                                 glm::vec2(getWindowWidth() * .5, 80), ci::Color("black"));
    }

    // drawing signs for the player ball racks
    ci::gl::drawStringCentered("Player 1",
                               glm::vec2(getWindowWidth() * .075, 120), ci::Color("black"));
    ci::gl::drawStringCentered("Player 2",
                               glm::vec2(getWindowWidth() * .925, 120), ci::Color("black"));

    DrawGameExternalities();  // draws player ball racks and power bar

    // draw game_board_
    game_board_.Draw();
  }
  else{
    DrawStartScreen();
  }
}
void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  if (!is_start_screen_) {
    // record mouseDrag when on game screen
    end_coords_ = event.getPos();
    CalculateForce();
  }
}

void PoolApp::update() {
  if (game_engine_.GetWinCondition()==0) {
    // while no winner update game engine and board
    for (size_t i = 0; i < update_speed_; i++) {
      game_board_.Update();
    }
  }
  game_engine_.Update(game_board_);
}

void PoolApp::mouseUp(ci::app::MouseEvent event) {
  if (!is_start_screen_) {
    // When on game screen
    if (game_engine_.HasCueBallSunk()) {
      // allow current player to add the cue ball back to the board
      if (game_board_.AddCueBall(event.getPos())) {
        game_engine_.AddCueBall(); // resets state to no cue ball sunk
      }
    } else if (game_engine_.GetWinCondition() == 0) {
      end_coords_ = event.getPos();
      CalculateForce();
      game_board_.HandleClick(game_board_.GetMouseCoords(), force_);
      force_ = 0;
    }
  }
}

void PoolApp::mouseDown(ci::app::MouseEvent event) {
  if (!is_start_screen_) {
    // when on game screen
    //adds particle to simulation
    if (game_engine_.GetWinCondition() == 0) {
      // if no winner
      init_coords_ = event.getPos();
    }
  }
}

void PoolApp::mouseMove(ci::app::MouseEvent event) {
  //adds particle to simulation
  if (!is_start_screen_) {
    //when on game screen
    if (game_engine_.GetWinCondition() == 0) {
      // if no winner, update user mouse position over board
      game_board_.UpdateMousePosition(event.getPos());
    }
  }
}

void PoolApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_SPACE: {
      is_start_screen_ = false; // go to game screen
      break;
    }

    case ci::app::KeyEvent::KEY_DELETE: {
      game_board_.Clear();
      break;
    }

    case ci::app::KeyEvent::KEY_UP: {
      update_speed_++; // increases speed of game-play
      break;
    }

    case ci::app::KeyEvent::KEY_DOWN: {
      // decreases speed of game-play
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

void PoolApp::DrawStartScreen() {
  double const logo_width = 400;
  double const logo_y_pos = getWindowHeight()*.15;
  double const line_spacing = 20;

  auto img = loadImage( loadAsset( "../assets/PoolHome.jpg" ) );
  ci::gl::TextureRef mTexture;
  mTexture = ci::gl::Texture2d::create( img );
  ci::Rectf drawRect(getWindowWidth()/2 - logo_width/2, logo_y_pos,
                     getWindowWidth()/2 + logo_width/2, logo_y_pos + logo_width/2  );
  ci::gl::draw(mTexture, drawRect); // drawing start screen Logo

  ci::gl::drawStringCentered("Press Space To Start!",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.45),
                             ci::Color("black"),ci::Font("Impact", 40));
  ci::gl::drawStringCentered("Rules:", glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.55),
                             ci::Color("black"), ci::Font("Times New Roman", 30));
  ci::gl::drawStringCentered("1. First Ball Pocketed Determines Player's Ball Colors ",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.6 + line_spacing)
                             , ci::Color("black"), ci::Font("Times New Roman", 20));
  ci::gl::drawStringCentered("2. Win by Pocketing all your balls and the 8ball",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.6 + 2*line_spacing),
                             ci::Color("black"), ci::Font("Times New Roman", 20));
  ci::gl::drawStringCentered("3. Pocketing the 8ball before your other balls is an automatic loss",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.6 + 3*line_spacing),
                             ci::Color("black"), ci::Font("Times New Roman", 20));
  ci::gl::drawStringCentered("4. Remember to always hit your own ball color first ",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.6 + 4*line_spacing),
                             ci::Color("black"), ci::Font("Times New Roman", 20));
  ci::gl::drawStringCentered("5. Pocketing your color gives you a second turn ",
                             glm::vec2(getWindowWidth()/2 ,getWindowHeight()*.6+ 5*line_spacing),
                             ci::Color("black"), ci::Font("Times New Roman", 20));
}

void PoolApp::DrawGameExternalities() {
  // drawing ball racks for players

  // draw ball rack for player 1
  ci::gl::color(ci::Color("grey"));
  ci::Rectf player_one_rack(getWindowWidth()*.05, getWindowHeight()*.3, getWindowWidth()*.08,
                            getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_one_rack, 20);

  // draw ball rack for player 2
  ci::Rectf player_two_rack(getWindowWidth()*.92, getWindowHeight()*.3, getWindowWidth()*.95,
                            getWindowHeight()*.7 );
  ci::gl::drawSolidRoundedRect(player_two_rack, 20);

  //displaying balls remaining for player one
  ci::gl::color(ci::Color("black")); // color of the first ball
  for(int i = 0; i < game_engine_.GetWinningScore() - game_engine_.GetPlayerOneScore(); i++){
    if(game_engine_.GetWinCondition() != 1) {
      // displays only if player 1 hasn't won
      ci::gl::drawSolidCircle(glm::vec2(getWindowWidth() * .065,
                                        getWindowHeight() * .3 + 20 + i * 40), 20);
      ci::gl::color(game_engine_.GetPlayerOneColor());
    }
  }

  // displaying balls remaining for player two
  ci::gl::color(ci::Color("black")); // color of the first ball on the rack
  for(int i = 0; i < game_engine_.GetWinningScore() - game_engine_.GetPlayerTwoScore(); i++){
    if(game_engine_.GetWinCondition() != 2) {
      // displays only if player 2 hasn't won
      ci::gl::drawSolidCircle(glm::vec2(getWindowWidth() * .935,
                                        getWindowHeight() * .3 + 20 + i * 40), 20);
      ci::gl::color(game_engine_.GetPlayerTwoColor());
    }
  }

  //drawing shot meter
  ci::gl::color(ci::Color("grey"));
  glm::vec2 top_left = ci::vec2(getWindowWidth()*.5-120,getWindowHeight()*.85);
  glm::vec2 bottom_right = ci::vec2(getWindowWidth()*.5+120,getWindowHeight()*.85+50);
  ci::Rectf full_power_bar(top_left, bottom_right);
  ci::gl::drawSolidRoundedRect(full_power_bar, 20); // drawing total shot bar

  // drawing force bar of variable length
  ci::gl::color(ci::Color("red"));
  bottom_right.x = getWindowWidth()*.5 - 120 + 240*(force_/max_force_);
  ci::Rectf power_bar(top_left,bottom_right);
  ci::gl::drawSolidRoundedRect(power_bar, 20);
  ci::gl::drawStringCentered(
      std::to_string((force_/max_force_)*100).substr(0,4)+"%",
      glm::vec2(getWindowWidth()*.5,getWindowHeight()*.85+25),ci::Color("black"));

}
}  // namespace visualizer
}  // namespace pool
