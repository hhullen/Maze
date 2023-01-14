#include "maze_model.h"

namespace s21 {

MazeModel::MazeModel() {
  cave_.FillRandomly(false);
  route_finder_.set_maze(maze_.get_right_walls(), maze_.get_bottom_walls());
}

MazeModel::~MazeModel() {}

vector<vector<bool>> *MazeModel::get_cave() { return cave_.get_cave(); }

bool MazeModel::LoadCaveFile(string &path) { return cave_.Load(path); }

void MazeModel::RandomizeCave(bool bordered) { cave_.FillRandomly(bordered); }

void MazeModel::set_cave_parameters(int size, double chance, bool border,
                                    int birth, int death) {
  int old_size = cave_.get_cave()->size();
  int old_chance = cave_.get_chance();
  bool old_border = cave_.IsBordered();
  cave_.set_size(size, size);
  cave_.set_init_chance(chance);
  cave_.set_limits(death, birth);
  cave_.set_bordered(border);
  if (size != old_size || chance != old_chance || border != old_border) {
    cave_.FillRandomly(border);
  }
}

bool MazeModel::RenderNextCave() { return cave_.RenderNext(); }

vector<vector<bool>> *MazeModel::get_right_walls() {
  return maze_.get_right_walls();
}

vector<vector<bool>> *MazeModel::get_bottom_walls() {
  return maze_.get_bottom_walls();
}

bool MazeModel::LoadMazeFile(string &path) { return maze_.Load(path); }

void MazeModel::RandomizeMaze() { maze_.GenerateRandomly(); }

void MazeModel::set_maze_size(int size) { maze_.set_size(size); }

bool MazeModel::FindRouteFromTo(Point &start, Point &end) {
  route_finder_.set_start(start);
  route_finder_.set_end(end);
  route_finder_.FindRoute();
  return route_finder_.IsFound();
}

vector<Location> *MazeModel::get_route() { return route_finder_.get_route(); }

}  // namespace s21
