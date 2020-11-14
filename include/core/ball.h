#ifndef NAIVE_BAYES_PARTICLE_H
#define NAIVE_BAYES_PARTICLE_H
#endif //NAIVE_BAYES_PARTICLE_H
#include "cinder/gl/gl.h"
//#include <Box2D/Box2D.h>
//#include <cairo/Cairo.h>

#include <cmath>

namespace idealgas {

/**
 * Represents ID for the 3 particle types in the simulator
 */
enum class Type { Cue , Red , Blue , EightBall };

class Ball {
 public:

  /**
   * Creates a particle with a position coordinate, velocity vector, radius, and color
   */
  Ball(const glm::vec2 &init_position, size_t radius, ci::Color color ,Type type);

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
  void UpdatePosition();

  double GetSpeed() const;

  glm::vec2 GetVelocity() const;

  glm::vec2 GetPosition() const;

  size_t GetRadius() const;

  ci::Color GetColor() const;

  Type GetType() const;

  void SetVelocity(const glm::vec2& v_coords);
 private:

  glm::vec2 position_;

  glm::vec2 velocity_;

  size_t radius_;

  ci::Color color_;

  Type type_;

//  b2Body* hi;

  /**
   * Helper method that changes velocities when a collision occurs
   */
  void UpdateVelocitiesAfterCollision(const glm::vec2& x_diff_this, const glm::vec2& x_diff_other,
                                      double dot_product_this, double dot_product_other, Ball& other);
};
}//namespace idealgas