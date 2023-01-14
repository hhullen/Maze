#ifndef SRC_MAZE_MODEL_MODULE_MAZE_MAZE_GEN_H_
#define SRC_MAZE_MODEL_MODULE_MAZE_MAZE_GEN_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using std::fmod;
using ::std::ifstream;
using std::invalid_argument;
using std::rand;
using std::stoi;
using std::string;
using std::vector;

namespace s21 {

class MazeGen {
 public:
  MazeGen();
  ~MazeGen();

  vector<vector<bool>> *get_right_walls();
  vector<vector<bool>> *get_bottom_walls();

  void set_size(int side_size);
  void GenerateRandomly();
  void Clear();
  bool Load(string path);

 private:
  enum CellState { SetWall = true, NoWall = false };

  vector<vector<bool>> right_walls_matrix_;
  vector<vector<bool>> bottom_walls_matrix_;
  vector<int> cells_sets_;
  int side_size_;
  int unique_set_start_;
  double wall_adding_chance_;

  bool ReadSize(string &line);
  bool ReadRowToMatrixFromLine(vector<vector<bool>> &matrix, string &line,
                               size_t iter);
  bool IsLineMatrixSeparator(string &line, size_t iter);
  void AddRightWallsToRow(int row);
  bool IsRandomlyDecided();
  bool IsFromOneSetWIthNext(int index);
  bool IsFromOneSetWIthPrev(int index);
  void MergeSetWithNextSet(int index, int set);
  void AddBottomWallsToRow(int row);
  void DeleteSetsValuesWithBottomWall(int row);
  void FinishMaze();
  void ResizeWallsMatrix(vector<vector<bool>> &matrix);
  void ClearWallsMatrix(vector<vector<bool>> &matrix);
  void AssignUniqueSet();
};

}  // namespace s21

#endif  // SRC_MAZE_MODEL_MODULE_MAZE_MAZE_GEN_H_
