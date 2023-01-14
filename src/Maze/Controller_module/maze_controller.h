#ifndef SRC_MAZE_CONTROLLER_MODULE_MAZE_CONTROLLER_H_
#define SRC_MAZE_CONTROLLER_MODULE_MAZE_CONTROLLER_H_

#include "../Model_module/maze_model.h"

namespace s21 {

class MazeController {
 public:
  MazeController(MazeModel *model);
  ~MazeController();

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
  MazeModel *model_;
};

}  // namespace s21

#endif  // SRC_MAZE_CONTROLLER_MODULE_MAZE_CONTROLLER_H_
