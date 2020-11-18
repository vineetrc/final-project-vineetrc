#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "board.h"

namespace pool {

namespace visualizer {

/**
 * Allows a user to draw a digit on a sketchpad and uses Naive Bayes to
 * classify it.
 */
class PoolApp : public ci::app::App {
 public:
  PoolApp();

  void draw() override;
  void update() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;
  void mouseMove(ci::app::MouseEvent event) override;
  const double kWindowSize = 900; //875
  const double kMargin = 10; //100

 private:
  Board game_board_;

  size_t update_speed_;

};
}  // namespace visualizer
}  // namespace pool