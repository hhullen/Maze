#ifndef SRC_MAZE_MODEL_MODULE_MAZE_MODEL_H_
#define SRC_MAZE_MODEL_MODULE_MAZE_MODEL_H_

#include "../include/maze_location.h"
#include "cave/cave_gen.h"
#include "maze/maze_gen.h"
#include "maze/route_finder.h"

namespace s21 {

class MazeModel {
 public:
  MazeModel();
  ~MazeModel();

  vector<vector<bool>> *get_cave();
  bool LoadCaveFile(string &path);
  void RandomizeCave(bool bordered);
  void set_cave_parameters(int size, double chance, bool border, int birth,
                           int death);
  bool RenderNextCave();

  vector<vector<bool>> *get_right_walls();
  vector<vector<bool>> *get_bottom_walls();
  bool LoadMazeFile(string &path);
  void RandomizeMaze();
  void set_maze_size(int size);

  bool FindRouteFromTo(Point &start, Point &end);
  vector<Location> *get_route();

 private:
  CaveGen cave_;
  MazeGen maze_;
  RouteFinder route_finder_;
};

}  // namespace s21

#endif  // SRC_MAZE_MODEL_MODULE_MAZE_MODEL_H_
