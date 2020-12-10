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
  game_balls_.push_back(Ball(cueball_pos, kBall_radii,
                             ci::Color("white"), Type::Cue));

  // arranging triangle formation
  // starting coordinate of ball break formation, around 3/4 length of table and in the middle
  glm::vec2 break_position = top_left_corner_ + vec2(.7 * x_boundary_dim,
                                                     .50 * y_boundary_dim );

  size_t num_rows = 5; // number of rows in a break formation

  for (size_t i = 0; i < num_rows; i++){
    for (size_t j = 0; j < i+1; j++){
        if (i == 2 && j == 1){
          // push back 8ball in this specific position
          game_balls_.push_back(Ball(
              break_position + vec2(0, j*2*kBall_radii),
              kBall_radii, ci::Color(kGame_particle_colors[2]), Type::EightBall));
        }
        else if ((i == 2 || i ==4) && j == 0 ){
          // push back blue balls in these specific positions
          game_balls_.push_back(Ball(
              break_position + vec2(0,j*2*kBall_radii),
              kBall_radii, ci::Color(kGame_particle_colors[1]), Type::Blue));
        }
        else if ((j%2 == 0)){
          // alternates coloring between red and blue balls
          game_balls_.push_back(Ball(
              break_position + vec2(0,j*2*kBall_radii),
              kBall_radii, ci::Color(kGame_particle_colors[0]), Type::Red));
        }
        else {
          game_balls_.push_back(Ball(
              break_position + vec2(0,j*2*kBall_radii),
              kBall_radii, ci::Color(kGame_particle_colors[1]), Type::Blue));
        }
      }
    break_position += vec2(2*kBall_radii, -1*kBall_radii); // updating start loc of drawing for next row
  }

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
  glm::vec2 br = bottom_right_corner_ + vec2(50, 50); // 50 is difference in size between outer table and board
  ci::Rectf box(tl, br);
  ci::gl::drawSolidRoundedRect(box, 20); // 20 is the rounded factor of the table

  //draw green background
  ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner_);
  ci::gl::color(ci::Color("green"));
  ci::gl::drawSolidRect(pixel_bounding_box);

  //drawing the pockets on the board
  for (size_t i = 0; i < pockets_.size(); i++) {
    ci::gl::color(ci::Color("black"));
    ci::gl::drawSolidCircle(pockets_.at(i), kPocket_radius);
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedCircle(pockets_.at(i), kPocket_radius);
  }

  //implemented drawing of cue
  ci::gl::color(ci::Color("white"));
  vec2 hit_dr = CalculateHitDirection(mouse_);
  if (next_turn_ == true && game_balls_.size() > 0 && game_balls_.at(0).GetType() == Type::Cue) {
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

void Board::SetPocketedBalls(std::vector<Ball>& pocketed_balls) {
  pocketed_balls_ = pocketed_balls;
}
void Board::Update() {
  //handles collisions and movement updates
  for (size_t i = 0; i < game_balls_.size(); i++) {
    for (size_t j = i; j < game_balls_.size(); j++) {
      game_balls_.at(i).HandleParticleCollision(game_balls_.at(j));
    }
    game_balls_.at(i).UpdatePosition(kStop_point, kFriction_force);
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
    if (!game_balls_.at(i).HasStopped()) {
      // if balls are still moving, next turn hasn't started
      next_turn_ = false;
    }
  }
}

void Board::HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords, float force) {
  if (next_turn_) {
    cue.SetVelocity(force * (mouse_coords - cue.GetPosition()) / glm::length(mouse_coords - cue.GetPosition()));
  }
}

bool Board::CheckIfPocketed(Ball& ball) {
  //if ball is pocketed add it to the pockets_ vector
  for (size_t i = 0; i < pockets_.size(); i++) {
    if (glm::length(ball.GetPosition() - pockets_.at(i)) < (kPocket_radius + kBall_radii)) {
      return true;
    }
  }
  return false;
}

void Board::UpdateMousePosition(const glm::vec2& mouse_coords) {
  mouse_ = mouse_coords;
}

bool Board::GetTurnStatus() const{
  return next_turn_;
}

std::vector<Ball>& Board::GetGameBalls() {
  return game_balls_;
}

std::vector<Ball> Board::GetPocketedBalls() const {
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
    // insert cue ball if the mouse coords are within the board
    game_balls_.insert(game_balls_.begin(), Ball(mouse_coords, kBall_radii,
                                                 ci::Color("white"), Type::Cue));
    return true;
  }
  // return false if given coords are not in game board bounds
  return false;
}

glm::vec2 Board::CalculateHitDirection(const glm::vec2& mouse_coord) const {
  vec2 ball = game_balls_.at(0).GetPosition(); //cue ball coordinates
  vec2 hit_dr = (mouse_coord - ball) / (-1 * glm::length(mouse_coord - ball));
  return hit_dr; // returning a unit vector in the direction of the user's mouse
}

glm::vec2 Board::GetMouseCoords() const {
  return mouse_;
}

void Board::SetTurnStatus(bool val) {
  next_turn_ = val;
}
}  // namespace visualizer
}  // namespace pool
