#pragma once
#include "cinder/gl/gl.h"
#include "core/ball.h"
#include <cmath>

namespace pool {
namespace visualizer {
 // constants for the 3 different types of particles
 static const double kBall_radii = 10;
 static const std::vector<ci::Color> kGame_particle_colors = {"red", "blue", "black"};

/**
 * A Board that represents the environment in which the gas particles interact
 */
class Board {
 public:

  /**
   * Creates a Pool Game Board with the specified top-left coords, x-length, and y-length
   */
  Board(const glm::vec2& top_left_corner, double x_boundary_dim, double y_boundary_dim);

  /**
   * Displays the current state of the game board in the Cinder application.
   */
  void Draw() const;

  /**
   * Called when player clicks to shoot cue ball
   */
  void HandleClick(const glm::vec2& clicked_screen_coords, float force);

  /**
   * Clears all balls in the game-board
   */
  void Clear();

  /**
   * Updates all ball movement in the simulator
   */
  void Update();


  /**
   * Returns true if the ball falls in a pocket
   */
  bool CheckIfPocketed(Ball& ball);

  /**
   * Updates User mouse position
   */
  void UpdateMousePosition(const glm::vec2& mouse_coords);

  /**
   * Returns true when all the balls have come to a stop and is awaiting another hit
   */
  bool GetTurnStatus() const;

  /**
   * Setter for indicating next turn, used for testing
   */
  void SetTurnStatus(bool val);

  /**
   * Sets pocketed balls, used for testing
   */
  void SetPocketedBalls(std::vector<Ball>& pocketed_balls);

  /**
   * Returns a vector of all balls currently on the game-board
   */
  std::vector<Ball>& GetGameBalls();

  /**
   * Returns a vector of all balls that have been pocketed
   */
  std::vector<Ball> GetPocketedBalls() const;

  /**
   * Added Ball for testing purposes
   */
  void AddBall(Ball ball);

  /**
   * Returns unit vector in direction where user wants cue ball to be hit
   */
  glm::vec2 CalculateHitDirection(const glm::vec2& mouse_coord) const;

  glm::vec2 GetMouseCoords() const;

  /**
   * Clears all balls in the vector of pocketed balls
   */
  void ClearPocketedBalls();

  /**
   *  returns true if cue ball was successfully added, false if user clicked out of bounds
   */
  bool AddCueBall(glm::vec2 mouse_coords);

 private:
  glm::vec2 top_left_corner_; // top left corner of the simulation-box

  glm::vec2 bottom_right_corner_; // bottom right corner of simulation-box

  double sim_x_bound_length_; // x-side length of simulator boundary

  double sim_y_bound_length_; // y-side length of simulator boundary

  std::vector<Ball> game_balls_; // all particles in the simulator

  std::vector<Ball> pocketed_balls_; //balls that fell into the pocket every turn

  glm::vec2 mouse_; // coordinates of the current state of the user's mouse

  std::vector<glm::vec2> pockets_; // stores coordinates of pockets on board

  bool next_turn_; // represents if player can hit a ball to start a new 'play'

  const double kStop_point = 0.1; // friction parameters

  const double kFriction_force = .0125; //friction parameters

  const double kPocket_radius = 20.0;

  /**
   * Helper method that gives cueball a certain velocity to create the 'hit'
   */
  void HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords, float force);
};
}  // namespace visualizer
}  // namespace pool
