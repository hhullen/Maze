#include "cave_gen.h"

namespace s21 {

CaveGen::CaveGen() : init_chance_(30), bordered_(false) {
  set_size(10, 10);
  set_limits(4, 6);
}

CaveGen::~CaveGen() {}

void CaveGen::set_size(int w, int h) {
  ValidateValue(&w, 10, 50);
  ValidateValue(&h, 10, 50);
  cave_.resize(h);
  for (vector<bool> &row : cave_) {
    row.resize(w);
  }
}

void CaveGen::set_limits(int dead, int live) {
  ValidateValue(&dead, 0, 7);
  ValidateValue(&live, 0, 7);
  dead_limit_ = dead;
  live_limit_ = live;
}

void CaveGen::set_init_chance(double percent) {
  if (percent > 100) {
    percent = 100;
  } else if (percent < 0) {
    percent = 0;
  }
  init_chance_ = percent;
}

void CaveGen::set_bordered(bool bordered) { bordered_ = bordered; }

vector<vector<bool>> *CaveGen::get_cave() { return &cave_; }

void CaveGen::FillRandomly(bool bordered) {
  for (vector<bool> &row : cave_) {
    for (size_t i = 0; i < row.size(); ++i) {
      row[i] = fmod(rand() * init_chance_ / 100, 101) <= init_chance_;
    }
  }
  if (bordered) {
    FillBorder();
  }
}

double CaveGen::get_chance() { return init_chance_; }

bool CaveGen::IsBordered() { return bordered_; }

bool CaveGen::Load(string &path) {
  bool returnable = false;
  ifstream file(path);
  size_t iter = 0;
  string line;

  if (file.is_open()) {
    getline(file, line, '\n');
    returnable = ReadSize(line);
    while (returnable && getline(file, line, '\n')) {
      returnable = ReadRow(line, iter);
      ++iter;
    }
    file.close();
  }

  return returnable;
}

bool CaveGen::Save(string &path) {
  bool returnable = false;
  ofstream file(path);

  if (file.is_open()) {
    file << cave_.size() << " " << cave_[0].size() << "\n";
    for (vector<bool> row : cave_) {
      for (bool col : row) {
        file << col << " ";
      }
      file << "\n";
    }
    returnable = true;
    file.close();
  }
  return returnable;
}

bool CaveGen::RenderNext() {
  vector<vector<bool>> temp(cave_);
  bool updated = false;

  for (size_t i = 0; i < cave_.size(); ++i) {
    for (size_t j = 0; j < cave_[i].size(); ++j) {
      int alives = CalculateAround(i, j);
      if (cave_[i][j] && alives < dead_limit_) {
        temp[i][j] = false;
      } else if (!(cave_[i][j]) && alives > live_limit_) {
        temp[i][j] = true;
      }
    }
  }

  if (cave_ != temp) {
    updated = true;
    cave_ = temp;
  }

  return updated;
}

void CaveGen::FillBorder() {
  for (size_t i = 0; i < cave_.size(); ++i) {
    if (i == 0 || i == cave_.size() - 1) {
      for (size_t j = 0; j < cave_[i].size(); ++j) {
        cave_[i][j] = true;
      }
    } else {
      cave_[i][0] = true;
      cave_[i][cave_.size() - 1] = true;
    }
  }
}

void CaveGen::ValidateValue(int *value, int min, int max) {
  if (*value < min) {
    *value = min;
  } else if (*value > max) {
    *value = max;
  }
}

bool CaveGen::ReadSize(string &line) {
  bool returnable = false;
  int w = 0, h = 0;

  if (sscanf(line.data(), "%d %d", &w, &h) == 2) {
    set_size(w, h);
    returnable = true;
  }

  return returnable;
}

bool CaveGen::ReadRow(string &line, size_t iter) {
  bool returnable = false;

  if (line.size() >= cave_[iter].size() * 2 - 1) {
    for (size_t i = 0; i < cave_[iter].size(); ++i) {
      cave_[iter][i] = stoi(&line.data()[i * 2]);
    }
    returnable = true;
  }

  return returnable;
}

int CaveGen::CalculateAround(size_t i, size_t j) {
  int returnable = 0;

  returnable += IsAlive(i - 1, j - 1);
  returnable += IsAlive(i, j - 1);
  returnable += IsAlive(i + 1, j - 1);

  returnable += IsAlive(i - 1, j);
  returnable += IsAlive(i + 1, j);

  returnable += IsAlive(i - 1, j + 1);
  returnable += IsAlive(i, j + 1);
  returnable += IsAlive(i + 1, j + 1);

  return returnable;
}

bool CaveGen::IsAlive(int i, int j) {
  return i >= 0 && j >= 0 && i < (int)cave_.size() &&
         j < (int)cave_[0].size() && cave_[i][j];
}

}  // namespace s21
