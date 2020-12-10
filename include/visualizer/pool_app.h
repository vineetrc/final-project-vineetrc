#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "board.h"
#include "core/engine.h"

namespace pool {
namespace visualizer {

/**
 * Allows User to play a Standard Game of 8ball Pool
 */
class PoolApp : public ci::app::App {
 public:
  PoolApp();

  void draw() override;
  void update() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void mouseUp(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;
  void mouseMove(ci::app::MouseEvent event) override;
  const double kWindow_size_x = 1400;
  const double kWindow_size_y = 800;
  const double kGame_size_x = 800;
  const double kGame_size_y = 350;
  const glm::vec2 kGame_loc = glm::vec2(300, 200);

 private:
  Board game_board_;
  glm::vec2 init_coords_; // starting coords of the drag action
  glm::vec2 end_coords_; // ending coords of the drag action
  double force_; // real-time force based on user mouse drag
  size_t update_speed_;
  Engine game_engine_;
  const double kMax_force = 20.0; // max force given to a mouse drag
  bool is_start_screen_; // determines when start screen is shown

  /**
   * Calculates force applied to ball based on mouse movement
   */
  void CalculateForce();

  /**
   *  Draws the power bar and player racks
   */
  void DrawGameExternalities();

  /**
   * Draws Start Screen of Game
   */
  void DrawStartScreen();
};
}  // namespace visualizer
}  // namespace pool
