#pragma once
#include "cinder/gl/gl.h"
#include <cmath>

namespace pool {

/**
 * Represents ID for the 3 particle types in the simulator, and a None type for collision detection
 */
enum class Type { Cue , Red , Blue , EightBall, None };

class Ball {
 public:

  /**
   * Creates a ball at a position coordinate with the given color and type
   */
  Ball(const glm::vec2& init_position, size_t radius, ci::Color color ,Type type);

  /**
   * Handles collisions with simulation boundary
   * uses top-left and bottom-right coordinate of boundary box
   */
  void HandleBoundaryCollision(const glm::vec2& top_left, const glm::vec2& bottom_right);

  /**
   * Handles particle-particle collisions
   */
  void HandleParticleCollision(Ball& other);

  /**
   * Updates position of particle based on current velocity
   */
  void UpdatePosition(double stop_point, double force);

  /**
   * Bool representing if particle has stopped moving
   */
  bool HasStopped();

  glm::vec2 GetVelocity() const;

  glm::vec2 GetPosition() const;

  size_t GetRadius() const;

  ci::Color GetColor() const;

  Type GetType() const;

  /**
   * Resets the first ball collision type to None
   */
  void ClearFirstBallCollision();

  /**
   * Returns the Type of the first ball this ball object collides with
   */
  Type GetFirstBallCollision() const;

  /**
   * Setter for collision variable for testing purposes
   */
  void SetFirstBallCollision(Type type);

  /**
   * Set velocity for testing purposes
   */
  void SetVelocity(const glm::vec2& v_coords);

 private:
  glm::vec2 position_;

  glm::vec2 velocity_;

  size_t radius_;

  ci::Color color_;

  Type type_;

  Type first_ball_collision_; // Stores type of the first ball collision in a new turn

  /**
   * Helper method that changes velocities when a collision occurs
   */
  void UpdateVelocitiesAfterCollision(const glm::vec2& x_diff_this, const glm::vec2& x_diff_other,
                                      double dot_product_this, double dot_product_other, Ball& other);

  /**
   * Implements friction in movement, with a force representing the decreasing velocity and a stop-point velocity
   */
  void AddFriction(double stop_point, double force);
};
}//namespace pool