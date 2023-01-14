#include "maze_gen.h"

namespace s21 {

MazeGen::MazeGen()
    : side_size_(10), unique_set_start_(1), wall_adding_chance_(50) {
  set_size(10);
  GenerateRandomly();
}

MazeGen::~MazeGen() {
  right_walls_matrix_.clear();
  bottom_walls_matrix_.clear();
  cells_sets_.clear();
}

vector<vector<bool>> *MazeGen::get_right_walls() {
  return &right_walls_matrix_;
}

vector<vector<bool>> *MazeGen::get_bottom_walls() {
  return &bottom_walls_matrix_;
}

void MazeGen::set_size(int side_size) {
  if (side_size < 0) {
    throw invalid_argument("size less than zero");
  }
  side_size_ = side_size;
  ResizeWallsMatrix(right_walls_matrix_);
  ResizeWallsMatrix(bottom_walls_matrix_);
}

void MazeGen::GenerateRandomly() {
  Clear();
  AssignUniqueSet();
  int penult_index = side_size_ - 1;
  for (int i = 0; i < penult_index; ++i) {
    AddRightWallsToRow(i);
    AddBottomWallsToRow(i);
    DeleteSetsValuesWithBottomWall(i);
    AssignUniqueSet();
  }
  AddRightWallsToRow(penult_index);
  FinishMaze();
}

void MazeGen::Clear() {
  ClearWallsMatrix(right_walls_matrix_);
  ClearWallsMatrix(bottom_walls_matrix_);
  cells_sets_.clear();
}

bool MazeGen::Load(string path) {
  bool is_loaded = false;
  ifstream file(path);
  size_t iter = 0;
  string line;

  if (file.is_open()) {
    getline(file, line, '\n');
    is_loaded = ReadSize(line);
    vector<vector<bool>> *p_walls_matrix = &right_walls_matrix_;
    while (is_loaded && getline(file, line, '\n')) {
      if (IsLineMatrixSeparator(line, iter)) {
        p_walls_matrix = &bottom_walls_matrix_;
        getline(file, line, '\n');
        iter = 0;
      }
      is_loaded = ReadRowToMatrixFromLine(*p_walls_matrix, line, iter);
      ++iter;
    }
    file.close();
  }

  return is_loaded;
}

bool MazeGen::ReadSize(string &line) {
  bool is_loaded = false;
  int w = 0, h = 0;

  if (sscanf(line.data(), "%d %d", &w, &h) == 2 && w == h) {
    set_size(w);
    is_loaded = true;
  }

  return is_loaded;
}

bool MazeGen::ReadRowToMatrixFromLine(vector<vector<bool>> &matrix,
                                      string &line, size_t iter) {
  bool is_loaded = false;
  size_t size = (size_t)side_size_;

  if (line.size() >= size * 2 - 1 && iter < size) {
    for (size_t i = 0; i < size; ++i) {
      matrix[iter][i] = stoi(&line.data()[i * 2]);
    }
    is_loaded = true;
  }

  return is_loaded;
}

bool MazeGen::IsLineMatrixSeparator(string &line, size_t iter) {
  return line.size() == 0 && iter == (size_t)side_size_;
}

void MazeGen::AddRightWallsToRow(int row) {
  for (int i = 0; i < side_size_; ++i) {
    if (IsRandomlyDecided() || IsFromOneSetWIthNext(i)) {
      right_walls_matrix_[row][i] = CellState::SetWall;
    } else {
      MergeSetWithNextSet(i, cells_sets_[i]);
    }
  }
  int last_element = side_size_ - 1;
  right_walls_matrix_[row][last_element] = CellState::SetWall;
}

bool MazeGen::IsRandomlyDecided() {
  int percents = 100;
  double random_percent = rand() * wall_adding_chance_ / percents;
  double reduced_percent = fmod(random_percent, percents + 1);
  return reduced_percent <= wall_adding_chance_;
}

bool MazeGen::IsFromOneSetWIthNext(int index) {
  int next_index = index + 1;
  int comparable = 0;
  if (next_index < side_size_) {
    comparable = cells_sets_[next_index];
  }
  return cells_sets_[index] == comparable;
}

bool MazeGen::IsFromOneSetWIthPrev(int index) {
  int prev_index = index - 1;
  int comparable = 0;
  if (prev_index > 0) {
    comparable = cells_sets_[prev_index];
  }
  return cells_sets_[index] == comparable;
}

void MazeGen::MergeSetWithNextSet(int index, int set) {
  int changable_set = cells_sets_[index + 1];
  for (int i = 0; i < side_size_; ++i) {
    if (cells_sets_[i] == changable_set) {
      cells_sets_[i] = set;
    }
  }
}

void MazeGen::AddBottomWallsToRow(int row) {
  bool has_one_passage = false;
  for (int i = 0; i < side_size_; ++i) {
    if (!IsFromOneSetWIthPrev(i)) {
      has_one_passage = false;
    }
    if (((!IsFromOneSetWIthPrev(i) && IsFromOneSetWIthNext(i)) ||
         (IsFromOneSetWIthPrev(i) && IsFromOneSetWIthNext(i)) ||
         (!IsFromOneSetWIthNext(i) && has_one_passage)) &&
        IsRandomlyDecided()) {
      bottom_walls_matrix_[row][i] = CellState::SetWall;
    } else {
      has_one_passage = true;
    }
  }
}

void MazeGen::DeleteSetsValuesWithBottomWall(int row) {
  for (int i = 0; i < side_size_; ++i) {
    if (bottom_walls_matrix_[row][i]) {
      cells_sets_[i] = 0;
    }
  }
}

void MazeGen::FinishMaze() {
  int last_row = side_size_ - 1;
  for (int i = 0; i < side_size_; ++i) {
    if (!IsFromOneSetWIthNext(i)) {
      right_walls_matrix_[last_row][i] = CellState::NoWall;
      MergeSetWithNextSet(i, cells_sets_[i]);
    }
    bottom_walls_matrix_[last_row][i] = CellState::SetWall;
  }
  right_walls_matrix_[last_row][last_row] = CellState::SetWall;
  cells_sets_.clear();
  unique_set_start_ = 1;
}

void MazeGen::ResizeWallsMatrix(vector<vector<bool>> &matrix) {
  matrix.resize(side_size_);
  matrix.shrink_to_fit();
  for (vector<bool> &row : matrix) {
    row.resize(side_size_);
  }
}

void MazeGen::ClearWallsMatrix(vector<vector<bool>> &matrix) {
  for (int i = 0; i < side_size_; ++i) {
    for (int j = 0; j < side_size_; ++j) {
      matrix[i][j] = CellState::NoWall;
    }
  }
}

void MazeGen::AssignUniqueSet() {
  int empty_set = 0;

  cells_sets_.resize(side_size_);
  for (int i = 0; i < side_size_; ++i) {
    if (cells_sets_[i] == empty_set) {
      cells_sets_[i] = i + unique_set_start_;
    }
  }
  unique_set_start_ += side_size_ + 1;
}

}  // namespace s21
