#include <visualizer/ideal_gas_app.h>

namespace pool {

namespace visualizer {

PoolApp::PoolApp()
    : game_board_(glm::vec2(200, 200), 800 , 300 , 10,
                  5, ci::Color("white")) {

  ci::app::setWindowSize(2000, 800);
  update_speed_ = 1; //number of simulation 'steps' gone thru on one update call

}

void PoolApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  game_board_.Draw();
}
void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  game_board_.HandleClick(event.getPos());
}

void PoolApp::update() {
  for (size_t i = 0; i < update_speed_; i++) {
    game_board_.Update();
  }
}

void PoolApp::mouseDown(ci::app::MouseEvent event) {
  //adds particle to simulation
  game_board_.HandleClick(event.getPos());
}

void PoolApp::mouseMove(ci::app::MouseEvent event) {
  //adds particle to simulation
  game_board_.UpdateMousePosition(event.getPos());
}



void PoolApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_r: {
     // game_board_.AddParticle(game_board_.GetRandPosition(), game_board_.GetRandVelocity(),sim_radii_[0],sim_particle_mass_[0],ci::Color(sim_particle_colors_[0]),Type::Small);
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
