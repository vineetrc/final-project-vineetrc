#include <visualizer/pool_app.h>

namespace pool {

namespace visualizer {

PoolApp::PoolApp()
    : game_board_(glm::vec2(200, 200), 800 , 300) {

  ci::app::setWindowSize(2000, 800);
  update_speed_ = 1; //number of simulation 'steps' gone thru on one update call

}

void PoolApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);
  std::string game_message = "Play in progress";
  std::string power = "n/a";
  if(game_board_.GetTurnStatus()==true){
    game_message = "hit the cue ball";
    power = std::to_string(.3*glm::length(init_coords-end_coords));
  }
  ci::gl::drawStringCentered(game_message,glm::vec2(300,700),ci::Color("black"));
  ci::gl::drawStringCentered(power,glm::vec2(300,725),ci::Color("black"));

  game_board_.Draw();
}
void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  //game_board_.HandleClick(event.getPos());
  end_coords = event.getPos();
}

void PoolApp::update() {
  for (size_t i = 0; i < update_speed_; i++) {
    game_board_.Update();
  }
}

void PoolApp::mouseUp(ci::app::MouseEvent event) {
  end_coords = event.getPos();

  game_board_.HandleClick(event.getPos(), .3*glm::length(init_coords-end_coords));
}

void PoolApp::mouseDown(ci::app::MouseEvent event) {
  //adds particle to simulation
  init_coords = event.getPos();
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
