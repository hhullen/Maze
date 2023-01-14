#ifndef SRC_MAZE_MODEL_MODULE_CAVE_CAVE_GEN_H_
#define SRC_MAZE_MODEL_MODULE_CAVE_CAVE_GEN_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::fmod;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::rand;
using std::sscanf;
using std::stoi;
using std::string;
using std::vector;

namespace s21 {

class CaveGen {
 public:
  CaveGen();
  ~CaveGen();

  void FillRandomly(bool bordered);
  bool Load(string &path);
  bool Save(string &path);
  bool RenderNext();

  void set_size(int w, int h);
  void set_limits(int dead, int live);
  void set_init_chance(double percent);
  void set_bordered(bool bordered);

  vector<vector<bool>> *get_cave();
  double get_chance();
  bool IsBordered();

 private:
  vector<vector<bool>> cave_;
  int dead_limit_;
  int live_limit_;
  double init_chance_;
  bool bordered_;

  void FillBorder();
  void ValidateValue(int *value, int min, int max);
  bool ReadSize(string &line);
  bool ReadRow(string &line, size_t iter);
  int CalculateAround(size_t i, size_t j);
  bool IsAlive(int i, int j);
};

}  // namespace s21

#endif  // SRC_MAZE_MODEL_MODULE_CAVE_CAVE_GEN_H_
