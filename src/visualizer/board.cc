#include <visualizer/board.h>
#include <vector>
using glm::vec2;

namespace idealgas {

namespace visualizer {

Board::Board(const vec2 &top_left_corner, double x_boundary_dim, double y_boundary_dim, size_t init_num_particles,
             size_t max_velocity, ci::Color background_color) {
  top_left_corner_ = top_left_corner;
  bottom_right_corner_ = top_left_corner_ + vec2(x_boundary_dim, y_boundary_dim);
  sim_x_bound_length_ = x_boundary_dim;
  sim_y_bound_length_ = y_boundary_dim;
  init_num_particles_ = init_num_particles;
  max_particle_velocity_ = max_velocity;
  //ocketed_balls_ = {}; //start with no pocketed balls
  mouse_ = vec2 (0,0);

  //cue ball is first in the list of game balls
  game_balls_.push_back(Ball(vec2(300, 350), sim_radii_[0],
                             ci::Color("white"), Type::Cue));

  for (size_t i = 0; i < init_num_particles_; i++) {
    game_balls_.push_back(Ball(vec2(600, 250+i*20), sim_radii_[0],
                               ci::Color(sim_particle_colors_[0]), Type::Red));
  }

  pockets_.push_back(top_left_corner_);
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_,0));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_,sim_y_bound_length_));
  pockets_.push_back(top_left_corner_ + vec2(0,sim_y_bound_length_));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_/2,0));
  pockets_.push_back(top_left_corner_ + vec2(sim_x_bound_length_/2,sim_y_bound_length_));
}

void Board::Draw() const {

  //draw brown outer table
  ci::gl::color(ci::Color("brown"));
  glm::vec2 tl = top_left_corner_ + vec2(-50,-50);
  glm::vec2 br = bottom_right_corner_ + vec2(50,50);
  ci::Rectf box(tl, br);
  ci::gl::drawSolidRoundedRect(box, 20);
  //draw green background
  ci::Rectf pixel_bounding_box(top_left_corner_, bottom_right_corner_);
  ci::gl::color(ci::Color("green"));
  ci::gl::drawSolidRect(pixel_bounding_box);


  //drawing the pockets on the board

  for( size_t i = 0; i < pockets_.size(); i++){
    ci::gl::color(ci::Color("black"));
    ci::gl::drawSolidCircle(pockets_.at(i),20);
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedCircle(pockets_.at(i),20);
  }

  ci::gl::color(ci::Color("red"));
  ci::gl::drawLine(game_balls_.at(0).GetPosition(), mouse_);
  for (size_t i = 0; i < game_balls_.size(); i++) {
    //drawing each particle
    ci::gl::color(game_balls_.at(i).GetColor());
    ci::gl::drawSolidCircle(game_balls_.at(i).GetPosition(), game_balls_.at(i).GetRadius());
  }
}

void Board::HandleClick(const vec2 &clicked_screen_coords) {
  //conditionals to check if clicked location is within simulator boundary
  mouse_ = clicked_screen_coords;
  HandleCueBallHit(game_balls_.at(0), clicked_screen_coords);
  bool inside_y_bound = clicked_screen_coords.y > top_left_corner_.y
      && clicked_screen_coords.y < bottom_right_corner_.y;

  bool inside_x_bound = clicked_screen_coords.x > top_left_corner_.x
      && clicked_screen_coords.x < bottom_right_corner_.x;

  if (inside_x_bound && inside_y_bound) {
    //adding a particle at the clicked coordinates
   // AddParticle(clicked_screen_coords, GetRandVelocity(), sim_radii_[2],
                //sim_particle_mass_[2], ci::Color("blue"),Type::Large);

  }
}

void Board::Clear() {
  game_balls_.clear();
}



void Board::Update() {
  //handles collisions and movement updates
  for (size_t i = 0; i < game_balls_.size(); i++) {
    for (size_t j = i; j < game_balls_.size(); j++) {
      game_balls_.at(i).HandleParticleCollision(game_balls_.at(j));
    }
    game_balls_.at(i).UpdatePosition();
    game_balls_.at(i).HandleBoundaryCollision(top_left_corner_, bottom_right_corner_);
  }

  for (size_t i = 0; i < game_balls_.size(); i++) {
    if(CheckIfPocketed(game_balls_.at(i))){
      pocketed_balls_.push_back(game_balls_.at(i));
      game_balls_.erase(game_balls_.begin()+i);
      i--;
    }
  }
}

glm::vec2 Board::GetRandPosition() const {
  glm::vec2 rand_spawn = top_left_corner_
      + vec2((rand() / double(RAND_MAX)) * sim_x_bound_length_, (rand() / double(RAND_MAX)) * sim_y_bound_length_);

  return rand_spawn;
}

glm::vec2 Board::GetRandVelocity() const {
  //creating a random velocity vector ranging from +/- max_particle velocity
  glm::vec2 rand_speed(((rand() / double(RAND_MAX)) * 2 * max_particle_velocity_) - max_particle_velocity_,
                       (rand() / double(RAND_MAX) * 2 * max_particle_velocity_) - max_particle_velocity_);

  return rand_speed;
}

void Board::HandleCueBallHit(Ball& cue, const glm::vec2& mouse_coords) {
  cue.SetVelocity(.01f*(mouse_coords-cue.GetPosition()));
}


std::vector<Ball> Board::GetParticles() const {
  return game_balls_;
}

bool Board::CheckIfPocketed(Ball &ball) {
  for(size_t i = 0; i< pockets_.size(); i++){
    if(glm::length(ball.GetPosition()-pockets_.at(i))<20){
      if(ball.GetType()!=Type::Cue){
        std::cout<<"pocketed";
        return true;
      }
    }
  }
  return false;
}






}  // namespace visualizer
}  // namespace idealgas