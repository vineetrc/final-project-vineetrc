#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "board.h"

namespace idealgas {

namespace visualizer {

/**
 * Allows a user to draw a digit on a sketchpad and uses Naive Bayes to
 * classify it.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;

  const double kWindowSize = 900; //875
  const double kMargin = 10; //100

 private:
  Board gas_sim_;

  size_t update_speed_;

};
}  // namespace visualizer
}  // namespace idealgas
