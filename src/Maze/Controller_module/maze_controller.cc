#include "maze_controller.h"

namespace s21 {

MazeController::MazeController(MazeModel *model) : model_(model) {}

MazeController::~MazeController() {}

vector<vector<bool>> *MazeController::get_cave() { return model_->get_cave(); }

bool MazeController::LoadCaveFile(string &path) {
  return model_->LoadCaveFile(path);
}

void MazeController::RandomizeCave(bool bordered) {
  model_->RandomizeCave(bordered);
}

void MazeController::set_cave_parameters(int size, double chance, bool border,
                                         int birth, int death) {
  model_->set_cave_parameters(size, chance, border, birth, death);
}

bool MazeController::RenderNextCave() { return model_->RenderNextCave(); }

vector<vector<bool>> *MazeController::get_right_walls() {
  return model_->get_right_walls();
}

vector<vector<bool>> *MazeController::get_bottom_walls() {
  return model_->get_bottom_walls();
}

bool MazeController::LoadMazeFile(string &path) {
  return model_->LoadMazeFile(path);
}

void MazeController::RandomizeMaze() { model_->RandomizeMaze(); }

void MazeController::set_maze_size(int size) { model_->set_maze_size(size); }

bool MazeController::FindRouteFromTo(Point &start, Point &end) {
  return model_->FindRouteFromTo(start, end);
}

vector<Location> *MazeController::get_route() { return model_->get_route(); }

}  // namespace s21
