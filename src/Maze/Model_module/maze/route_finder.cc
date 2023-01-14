#include "route_finder.h"

namespace s21 {

RouteFinder::RouteFinder() : right_walls_(nullptr), bottom_walls_(nullptr) {
  start_ = Point(0, 0);
  end_ = Point(0, 0);
  state_.is_finding = false;
  state_.is_route_found = false;
  ClearRoute();
}

RouteFinder::~RouteFinder() {
  ClearRoute();
  right_walls_ = nullptr;
  bottom_walls_ = nullptr;
}

void RouteFinder::set_maze(vector<vector<bool>> *right_walls,
                           vector<vector<bool>> *bottom_walls) {
  right_walls_ = right_walls;
  bottom_walls_ = bottom_walls;
}

void RouteFinder::set_start(int x, int y) { start_ = Point(x, y); }

void RouteFinder::set_end(int x, int y) { end_ = Point(x, y); }

void RouteFinder::set_start(const Point &start) { start_ = start; }

void RouteFinder::set_end(const Point &end) { end_ = end; }

void RouteFinder::FindRoute() {
  SetStartFindingState();
  if (IsRouteDataCorrect()) {
    ClearRoute();
    AddStartPositionToPath();
    ScanAvailableDirectionsIfNeed();

    while (!IsGotDestination() && IsRouteExists()) {
      BackIfNoAvailableDirections();
      MoveToAnyAvailableDirection();
      ScanAvailableDirectionsIfNeed();
    }
  }
  SetEndFindingState();
}

void RouteFinder::ClearRoute() {
  for (Location &loc : route_) {
    loc.directions().clear();
  }
  route_.clear();
}

bool RouteFinder::IsFound() { return state_.is_route_found; }

bool RouteFinder::IsRunning() { return state_.is_finding; }

vector<Location> *RouteFinder::get_route() { return &route_; }

bool RouteFinder::IsRouteDataCorrect() {
  return right_walls_ && bottom_walls_ && IsRoutePointCorrect(start_) &&
         IsRoutePointCorrect(end_);
}

bool RouteFinder::IsRouteExists() {
  Point &current_position = route_.back().position();
  return !(!IsAnyDirectionAvailable() && current_position == start_);
}

bool RouteFinder::IsRoutePointCorrect(Point &p) {
  int maze_size = right_walls_->size();
  return 0 <= p.x() && p.x() < maze_size && 0 <= p.y() && p.y() < maze_size;
}

void RouteFinder::AddStartPositionToPath() {
  route_.push_back(Location(start_, Directions(Directions::There)));
}

bool RouteFinder::IsGotDestination() {
  return route_.back().position() == end_;
}

void RouteFinder::ScanAvailableDirectionsIfNeed() {
  Location &current_location = route_.back();
  if (!current_location.scanned()) {
    vector<Directions> &directions = route_.back().directions();
    Point &current_position = route_.back().position();

    directions.clear();

    if (IsLeftDirectionAvailable(current_position)) {
      directions.push_back(Directions(Directions::Left));
    }
    if (IsRightDirectionAvailable(current_position)) {
      directions.push_back(Directions(Directions::Right));
    }
    if (IsUpDirectionAvailable(current_position)) {
      directions.push_back(Directions(Directions::Up));
    }
    if (IsDownDirectionAvailable(current_position)) {
      directions.push_back(Directions(Directions::Down));
    }
    current_location.set_scanned(true);
  }
}

bool RouteFinder::IsLeftDirectionAvailable(Point &pos) {
  bool is_previous = route_.back().previous() == Directions::Left;
  return pos.x() > 0 && !(*right_walls_)[pos.y()][pos.x() - 1] && !is_previous;
}

bool RouteFinder::IsRightDirectionAvailable(Point &pos) {
  bool is_previous = route_.back().previous() == Directions::Right;
  return pos.x() < right_walls_->size() && !(*right_walls_)[pos.y()][pos.x()] &&
         !is_previous;
}

bool RouteFinder::IsUpDirectionAvailable(Point &pos) {
  bool is_previous = route_.back().previous() == Directions::Up;
  return pos.y() > 0 && !(*bottom_walls_)[pos.y() - 1][pos.x()] && !is_previous;
}

bool RouteFinder::IsDownDirectionAvailable(Point &pos) {
  bool is_previous = route_.back().previous() == Directions::Down;
  return pos.y() < bottom_walls_->size() &&
         !(*bottom_walls_)[pos.y()][pos.x()] && !is_previous;
}

void RouteFinder::BackIfNoAvailableDirections() {
  bool is_start = route_.back().position() == start_;
  if (!IsAnyDirectionAvailable() && !is_start) {
    route_.pop_back();
  }
}

void RouteFinder::MoveToAnyAvailableDirection() {
  if (IsAnyDirectionAvailable()) {
    Directions &direction = route_.back().directions().back();
    Point next_position = route_.back().position();
    Directions direction_to_prev;

    if (direction == Directions::Left) {
      direction_to_prev = Directions::Right;
      next_position.x() -= 1;
    } else if (direction == Directions::Right) {
      direction_to_prev = Directions::Left;
      next_position.x() += 1;
    } else if (direction == Directions::Down) {
      direction_to_prev = Directions::Up;
      next_position.y() += 1;
    } else if (direction == Directions::Up) {
      direction_to_prev = Directions::Down;
      next_position.y() -= 1;
    }
    route_.back().directions().pop_back();
    route_.push_back(Location(next_position, direction_to_prev));
  }
}

bool RouteFinder::IsAnyDirectionAvailable() {
  return route_.back().directions().size() > 0;
}

void RouteFinder::SetStartFindingState() {
  state_.is_finding = true;
  state_.is_route_found = false;
}

void RouteFinder::SetEndFindingState() {
  state_.is_finding = false;
  bool is_found = route_.back().position() == end_;
  if (is_found && route_.size() > 1) {
    state_.is_route_found = true;
  }
}

}  // namespace s21
