#pragma once
#include "cinder/gl/gl.h"
#include "core/ball.h"
#include <cmath>

namespace pool {

namespace visualizer {

 // constants for the 3 different types of particles
 static const std::vector<double> sim_radii_ = {10};
 static const std::vector<ci::Color> sim_particle_colors_ = {"red", "blue", "black"};

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
  Board(const glm::vec2& top_left_corner, double x_boundary_dim, double y_boundary_dim, size_t init_num_particles,
        size_t max_velocity, ci::Color background_color);

  /**
   * Displays the current state of the simulator in the Cinder application.
   */
  void Draw() const;

  /**
   * Adds particle at clicked coordinates if is within simulation boundary
   */
  void HandleClick(const glm::vec2& clicked_screen_coords);

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

  void HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords);

  bool CheckIfPocketed(Ball& ball);

  void UpdateMousePosition(const glm::vec2& mouse_coords);

  /**
   * Returns true when the balls have come to a stop and is awaiting another hit
   */
  bool GetTurnStatus();

 private:

  glm::vec2 top_left_corner_; // top left corner of the simulation-box

  glm::vec2 bottom_right_corner_; // bottom right corner of simulation-box

  double sim_x_bound_length_; // side length of simulator boundary

  double sim_y_bound_length_;

  size_t init_num_particles_; // not needed remove future

  std::vector<Ball> game_balls_; // all particles in the simulator

  std::vector<Ball> pocketed_balls_; //balls that fell into the pocket every turn

  size_t max_particle_velocity_; //not needed

  glm::vec2 mouse_;

  std::vector<glm::vec2> pockets_;

  bool next_turn_;

  const double stop_point_ = 0.1; // friction parameters

  const double friction_force_ = .01; //friction parameters
};
}  // namespace visualizer
}  // namespace pool
