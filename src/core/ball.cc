#include <core/ball.h>
namespace pool {

Ball::Ball(const glm::vec2 &init_position, size_t radius, ci::Color color, Type type) {
  position_ = init_position;
  velocity_ = glm::vec2(0, 0);
  radius_ = radius;
  color_ = color;
  type_ = type;
}

void Ball::UpdateVelocitiesAfterCollision(const glm::vec2 &x_diff_this, const glm::vec2 &x_diff_other,
                                          double dot_product_this, double dot_product_other, Ball &other) {
  //changes velocity of 'this' particle
  velocity_ = velocity_ - ((float) (dot_product_this) /
      (glm::length(x_diff_this) * glm::length(x_diff_this))) * x_diff_this;

  //changes velocity of the other particle
  other.velocity_ = other.velocity_ - ((float) (dot_product_other) /
      (glm::length(x_diff_other) * glm::length(x_diff_other))) * x_diff_other;
}

void Ball::HandleParticleCollision(Ball &other) {

  glm::vec2 x_diff_this = position_ - other.position_;
  glm::vec2 v_diff_this = velocity_ - other.velocity_;

  glm::vec2 x_diff_other = other.position_ - position_;
  glm::vec2 v_diff_other = other.velocity_ - velocity_;

  double dot_product_this = glm::dot(x_diff_this, v_diff_this);
  double dot_product_other = glm::dot(x_diff_other, v_diff_other);

  if (dot_product_this < 0 && glm::length(x_diff_this) < (radius_ + other.radius_)) {
    //determines if two particles are moving towards each other and are intersecting
    UpdateVelocitiesAfterCollision(x_diff_this, x_diff_other, dot_product_this, dot_product_other, other);
  }
}

void Ball::HandleBoundaryCollision(const glm::vec2 &top_left, const glm::vec2 &bottom_right) {
  if (this->radius_ > std::abs(this->position_.x - top_left.x) && velocity_.x < 0) {
    //handles left boundary collision
    velocity_.x *= -1;
  }

  if (this->radius_ > std::abs(this->position_.x - bottom_right.x) && velocity_.x > 0) {
    //handles right boundary collision
    velocity_.x *= -1;
  }

  if (this->radius_ > std::abs(this->position_.y - top_left.y) && velocity_.y < 0) {
    //handles top boundary collision
    velocity_.y *= -1;
  }

  if (this->radius_ > std::abs(this->position_.y - bottom_right.y) && velocity_.y > 0) {
    //handles bottom boundary collision
    velocity_.y *= -1;
  }
}

void Ball::UpdatePosition(double stop_point, double force) {
  position_ += velocity_;
  AddFriction(stop_point, force);
}

void Ball::AddFriction(double stop_point, double force) {
  if (glm::length(velocity_) < stop_point) {
    //set velocity to 0 once vector goes below threshold
    velocity_ = glm::vec2(0, 0);
  }
  else {
    if (velocity_.y < 0) {
      velocity_.y += force * std::abs(velocity_.y);
    } else if (velocity_.y > 0) {
      velocity_.y -= force * std::abs(velocity_.y);
    }

    if (velocity_.x < 0) {
      velocity_.x += force * std::abs(velocity_.x);
    }
    else if (velocity_.x > 0) {
      velocity_.x -= force * std::abs(velocity_.x);
    }
  }
}


glm::vec2 Ball::GetPosition() const {
  return position_;
}

glm::vec2 Ball::GetVelocity() const {
  return velocity_;
}

double Ball::GetSpeed() const {
  return glm::length(velocity_);
}

Type Ball::GetType() const {
  return type_;
}

ci::Color Ball::GetColor() const {
  return color_;
}

size_t Ball::GetRadius() const {
  return radius_;
}
void Ball::SetVelocity(const glm::vec2 &v_coords) {
  velocity_ = v_coords;
}

bool Ball::HasStopped() {
  //checks when all the ball have stopped moving
  return glm::vec2(0, 0) == velocity_;
}

} //namespace pool
