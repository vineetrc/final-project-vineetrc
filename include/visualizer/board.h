#pragma once
#include "cinder/gl/gl.h"
#include "core/ball.h"
#include <cmath>

namespace pool {

namespace visualizer {

 // constants for the 3 different types of particles
 static const std::vector<double> ball_radii_ = {10};
 static const std::vector<ci::Color> game_particle_colors_ = {"red", "blue", "black"};

/**
 * A Board that represents the environment in which the gas particles interact
 */
class Board {

 public:

  /**
   * Creates a simulation of gas particles
   * @param top_left_corner coordinate of simulation
   * @param boundary_dim side length dimension of simulation
   * @param init_num_particles number of initial particles of each type in simulation
   */
  Board(const glm::vec2& top_left_corner, double x_boundary_dim, double y_boundary_dim);

  /**
   * Displays the current state of the simulator in the Cinder application.
   */
  void Draw() const;

  /**
   * Adds particle at clicked coordinates if is within simulation boundary
   */
  void HandleClick(const glm::vec2& clicked_screen_coords, float force);

  /**
   * Clears all particles in the simulator
   */
  void Clear();

  /**
   * Updates all gas movement in the simulator
   */
  void Update();


  /**
   * Getter method that returns particles in simulation. Used for Testing
   */
  std::vector<Ball> GetParticles() const;

  void HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords, float force);

  bool CheckIfPocketed(Ball& ball);

  void UpdateMousePosition(const glm::vec2& mouse_coords);

  /**
   * Returns true when the balls have come to a stop and is awaiting another hit
   */
  bool GetTurnStatus();

  /**
   * getter for testing
   */
  std::vector<Ball> GetGameBalls();

  std::vector<Ball> GetPocketedBalls();

  void AddBall(Ball ball);

  glm::vec2 CalculateHitDirection(const glm::vec2& mouse_coord) const;

  glm::vec2 GetMouseCoords() const;

  void ClearPocketedBalls();
 private:

  glm::vec2 top_left_corner_; // top left corner of the simulation-box

  glm::vec2 bottom_right_corner_; // bottom right corner of simulation-box

  double sim_x_bound_length_; // side length of simulator boundary

  double sim_y_bound_length_;

  std::vector<Ball> game_balls_; // all particles in the simulator

  std::vector<Ball> pocketed_balls_; //balls that fell into the pocket every turn

  glm::vec2 mouse_; // coordinates of the current state of the mouse

  std::vector<glm::vec2> pockets_;

  bool next_turn_; // represents if player can hit a ball to start a new 'play'

  const double stop_point_ = 0.1; // friction parameters

  const double friction_force_ = .0125; //friction parameters
};
}  // namespace visualizer
}  // namespace pool
