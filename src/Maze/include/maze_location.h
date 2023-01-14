#ifndef SRC_MAZE_INCLUDE_MAZE_LOCATION_H_
#define SRC_MAZE_INCLUDE_MAZE_LOCATION_H_

#include <vector>

using std::vector;

namespace s21 {

enum class Directions { Left, Right, Down, Up, There };

class Point {
 public:
  Point() {}
  Point(int x, int y) : x_(x), y_(y) {}
  bool operator==(const Point &p) const { return p.x_ == x_ && p.y_ == y_; }
  Point &operator=(const Point &p) {
    x_ = p.x_;
    y_ = p.y_;
    return *this;
  }
  int &x() { return x_; }
  int &y() { return y_; }

 private:
  int x_, y_;
};

class Location {
 public:
  Location(Point position, Directions previous)
      : position_(position), previous_(previous), is_scanned_(false) {
    directions_.clear();
  }
  Point &position() { return position_; }
  Directions &previous() { return previous_; }
  vector<Directions> &directions() { return directions_; }
  bool scanned() { return is_scanned_; }
  void set_scanned(bool scanned) { is_scanned_ = scanned; }

 private:
  Point position_;
  Directions previous_;
  vector<Directions> directions_;
  bool is_scanned_;
};

}  // namespace s21

#endif  // SRC_MAZE_INCLUDE_MAZE_LOCATION_H_
