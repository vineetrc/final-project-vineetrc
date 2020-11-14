#include <visualizer/ideal_gas_app.h>

namespace idealgas {

namespace visualizer {

IdealGasApp::IdealGasApp()
    : gas_sim_(glm::vec2(200, 200),800 , 300 , 10,
               5, ci::Color("white")) {

  ci::app::setWindowSize(2000, 800);
  update_speed_ = 1; //number of simulation 'steps' gone thru on one update call

}

void IdealGasApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  gas_sim_.Draw();
  ci::gl::drawStringCentered(
      "This is an Ideal Gas Sim",
      glm::vec2(kWindowSize / 2, kMargin / 2), ci::Color("black"));



}
void IdealGasApp::mouseDrag(ci::app::MouseEvent event) {
  gas_sim_.HandleClick(event.getPos());
}

void IdealGasApp::update() {
  for (size_t i = 0; i < update_speed_; i++) {
    gas_sim_.Update();
  }

}

void IdealGasApp::mouseDown(ci::app::MouseEvent event) {
  //adds particle to simulation
  gas_sim_.HandleClick(event.getPos());
}

void IdealGasApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_r: {
     // gas_sim_.AddParticle(gas_sim_.GetRandPosition(), gas_sim_.GetRandVelocity(),sim_radii_[0],sim_particle_mass_[0],ci::Color(sim_particle_colors_[0]),Type::Small);
      break;
    }

    case ci::app::KeyEvent::KEY_g: {
      //gas_sim_.AddParticle(gas_sim_.GetRandPosition(), gas_sim_.GetRandVelocity(),sim_radii_[1],sim_particle_mass_[1],ci::Color(sim_particle_colors_[1]),Type::Medium);
      break;
    }

    case ci::app::KeyEvent::KEY_b: {
      //gas_sim_.AddParticle(gas_sim_.GetRandPosition(), gas_sim_.GetRandVelocity(),sim_radii_[2],sim_particle_mass_[2],ci::Color(sim_particle_colors_[2]),Type::Large);
      break;
    }

    case ci::app::KeyEvent::KEY_DELETE: {
      gas_sim_.Clear();
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
}  // namespace idealgas
