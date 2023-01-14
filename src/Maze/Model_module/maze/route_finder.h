#ifndef SRC_MAZE_MODEL_MODULE_ROUTE_FINDER_H_
#define SRC_MAZE_MODEL_MODULE_ROUTE_FINDER_H_

#include <vector>

#include "../../include/maze_location.h"

using s21::Directions;
using s21::Location;
using s21::Point;
using std::vector;

namespace s21 {

class RouteFinder {
 public:
  RouteFinder();
  ~RouteFinder();

  void set_maze(vector<vector<bool>> *right_walls,
                vector<vector<bool>> *bottom_walls);
  void set_start(int x, int y);
  void set_end(int x, int y);
  void set_start(const Point &start);
  void set_end(const Point &end);
  void FindRoute();
  void ClearRoute();
  bool IsFound();
  bool IsRunning();
  vector<Location> *get_route();

 private:
  struct FinderState {
    bool is_route_found;
    bool is_finding;
  };

  vector<Location> route_;
  Point start_;
  Point end_;
  FinderState state_;

  vector<vector<bool>> *right_walls_;
  vector<vector<bool>> *bottom_walls_;

  bool IsRouteDataCorrect();
  bool IsRouteExists();
  bool IsRoutePointCorrect(Point &p);
  void AddStartPositionToPath();
  bool IsGotDestination();
  void ScanAvailableDirectionsIfNeed();
  bool IsLeftDirectionAvailable(Point &pos);
  bool IsRightDirectionAvailable(Point &pos);
  bool IsUpDirectionAvailable(Point &pos);
  bool IsDownDirectionAvailable(Point &pos);
  void BackIfNoAvailableDirections();
  void MoveToAnyAvailableDirection();
  bool IsAnyDirectionAvailable();
  void SetStartFindingState();
  void SetEndFindingState();
};

}  // namespace s21

#endif  // SRC_MAZE_MODEL_MODULE_ROUTE_FINDER_H_
