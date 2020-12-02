#include <visualizer/board.h>
#include <vector>
using glm::vec2;

namespace pool {

namespace visualizer {

Board::Board(const vec2& top_left_corner, double x_boundary_dim, double y_boundary_dim) {
  top_left_corner_ = top_left_corner;
  bottom_right_corner_ = top_left_corner_ + vec2(x_boundary_dim, y_boundary_dim);
  sim_x_bound_length_ = x_boundary_dim;
  sim_y_bound_length_ = y_boundary_dim;
  next_turn_ = true; //always start of with a turn
  pocketed_balls_ = {}; //start with no pocketed balls
  mouse_ = vec2(0, 0);

  //cue ball is first in the list of game balls
  glm::vec2 cueball_pos = top_left_corner_ + vec2(.25 * x_boundary_dim, .5 * y_boundary_dim);
  game_balls_.push_back(Ball(cueball_pos, ball_radii_[0],
                             ci::Color("white"), Type::Cue));

  // arranging triangle formation
  glm::vec2 break_position = top_left_corner_ + vec2(.85 * x_boundary_dim,
                                                     .50 * y_boundary_dim - 2.5 * ball_radii_[0]);

  //1st row
  game_balls_.push_back(Ball(break_position + vec2(0, 0), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(0, 1 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  game_balls_.push_back(Ball(break_position + vec2(0, 2 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(0, 3 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  game_balls_.push_back(Ball(break_position + vec2(0, 4 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  //2nd row
  game_balls_.push_back(Ball(break_position + vec2(-20, 10), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(-20, 10 + 1 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  game_balls_.push_back(Ball(break_position + vec2(-20, 10 + 2 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(-20, 10 + 3 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  //3rd row
  game_balls_.push_back(Ball(break_position + vec2(-40, 20 + 0 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(-40, 20 + 1 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[2]), Type::EightBall));
  game_balls_.push_back(Ball(break_position + vec2(-40, 20 + 2 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  //4th row
  game_balls_.push_back(Ball(break_position + vec2(-60, 30 + 0 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[0]), Type::Red));
  game_balls_.push_back(Ball(break_position + vec2(-60, 30 + 1 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));
  //5th row
  game_balls_.push_back(Ball(break_position + vec2(-80, 40 + 0 * 20), ball_radii_[0],
                             ci::Color(game_particle_colors_[1]), Type::Blue));

  //position of pockets that are at the table
  pockets_.push_back(top_left_corner_);
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_, 0));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_, sim_y_bound_length_));
  pockets_.push_back(top_left_corner_ + vec2(0, sim_y_bound_length_));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_ / 2, 0));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_ / 2, sim_y_bound_length_));
}

void Board::Draw() const {
  //draw brown outer table
  ci::gl::color(ci::Color("brown"));
  glm::vec2 tl = top_left_corner_ + vec2(-50, -50);
  glm::vec2 br = bottom_right_corner_ + vec2(50, 50);
  ci::Rectf box(tl, br);
  ci::gl::drawSolidRoundedRect(box, 20);

  //draw green background
  ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner_);
  ci::gl::color(ci::Color("green"));
  ci::gl::drawSolidRect(pixel_bounding_box);

  //drawing the pockets on the board
  for (size_t i = 0; i < pockets_.size(); i++) {
    ci::gl::color(ci::Color("black"));
    ci::gl::drawSolidCircle(pockets_.at(i), 20);
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedCircle(pockets_.at(i), 20);
  }

  //implemented drawing of cue
  ci::gl::color(ci::Color("white"));
  vec2 hit_dr = CalculateHitDirection(mouse_);
  if (next_turn_ == true && game_balls_.size() > 0 && game_balls_.at(0).GetType() == Type::Cue) {
    ci::gl::lineWidth(5.0);
    ci::gl::drawLine(game_balls_.at(0).GetPosition(), game_balls_.at(0).GetPosition() + hit_dr * 100.0f);
  }

  for (size_t i = 0; i < game_balls_.size(); i++) {
    //drawing each particle
    ci::gl::color(game_balls_.at(i).GetColor());
    ci::gl::drawSolidCircle(game_balls_.at(i).GetPosition(), game_balls_.at(i).GetRadius());
  }
}

void Board::HandleClick(const vec2& clicked_screen_coords, float force) {
  //conditionals to check if clicked location is within simulator boundary
  HandleCueBallHit(game_balls_.at(0), clicked_screen_coords, force);
}

void Board::Clear() {
  game_balls_.clear();
}

void Board::ClearPocketedBalls() {
  pocketed_balls_.clear();
}

void Board::Update() {
  //handles collisions and movement updates
  for (size_t i = 0; i < game_balls_.size(); i++) {
    for (size_t j = i; j < game_balls_.size(); j++) {
      game_balls_.at(i).HandleParticleCollision(game_balls_.at(j));
    }
    game_balls_.at(i).UpdatePosition(stop_point_, friction_force_);
    game_balls_.at(i).HandleBoundaryCollision(top_left_corner_, bottom_right_corner_);
  }

  //if ball is pocketed, remove balls from gameboard
  for (size_t i = 0; i < game_balls_.size(); i++) {
    if (CheckIfPocketed(game_balls_.at(i))) {
      pocketed_balls_.push_back(game_balls_.at(i));
      game_balls_.erase(game_balls_.begin() + i);
      i--;
    }
  }
  next_turn_ = true;
  for (size_t i = 0; i < game_balls_.size(); i++) {
    if (game_balls_.at(i).HasStopped() == false) {
      next_turn_ = false;
    }
  }
}

void Board::HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords, float force) {
  if (next_turn_ == true) {
    cue.SetVelocity(force * (mouse_coords - cue.GetPosition()) / glm::length(mouse_coords - cue.GetPosition()));
  }
}

std::vector<Ball> Board::GetParticles() const {
  return game_balls_;
}

bool Board::CheckIfPocketed(Ball& ball) {
  //if ball is pocketed add it to the pockets_ vector
  for (size_t i = 0; i < pockets_.size(); i++) {
    if (glm::length(ball.GetPosition() - pockets_.at(i)) < 30) {
      return true;
    }
  }
  return false;
}

void Board::UpdateMousePosition(const glm::vec2& mouse_coords) {
  mouse_ = mouse_coords;
}

bool Board::GetTurnStatus() {
  return next_turn_;
}

std::vector<Ball> Board::GetGameBalls() {
  return game_balls_;
}

std::vector<Ball> Board::GetPocketedBalls() {
  return pocketed_balls_;
}

void Board::AddBall(Ball ball) {
  game_balls_.push_back(ball);
}

bool Board::AddCueBall(glm::vec2 mouse_coords) {
  bool inside_y_bound = mouse_coords.y > top_left_corner_.y
      && mouse_coords.y < bottom_right_corner_.y;

  bool inside_x_bound = mouse_coords.x > top_left_corner_.x
      && mouse_coords.x < bottom_right_corner_.x;

  if (inside_x_bound && inside_y_bound) {
    game_balls_.insert(game_balls_.begin(), Ball(mouse_coords, ball_radii_[0],
                                                 ci::Color("white"), Type::Cue));
    return true;
  }
  return false;
}

glm::vec2 Board::CalculateHitDirection(const glm::vec2& mouse_coord) const {
  vec2 ball = game_balls_.at(0).GetPosition(); //cue ball coordinates
  vec2 hit_dr = (mouse_coord - ball) / (-1 * glm::length(mouse_coord - ball));
  return hit_dr;
}

glm::vec2 Board::GetMouseCoords() const {
  return mouse_;
}
}  // namespace visualizer
}  // namespace pool
