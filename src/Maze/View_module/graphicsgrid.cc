#include "graphicsgrid.h"

namespace s21 {

GraphicsGrid::GraphicsGrid(QGraphicsView *parent)
    : QGraphicsView{parent}, grid_width_(10), grid_height_(10) {
  CalculateCellSize();
  size_policy_.setHorizontalPolicy(QSizePolicy::Expanding);
  size_policy_.setVerticalPolicy(QSizePolicy::Expanding);
  scene_.setSceneRect(0, 0, kBaseSize, kBaseSize);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setScene(&scene_);
  setSizePolicy(size_policy_);
  scene_.setBackgroundBrush(QColor(0, 0, 0));
}

GraphicsGrid::~GraphicsGrid() {}

void GraphicsGrid::set_grid_size(int size) {
  grid_width_ = size;
  grid_height_ = size;
  CalculateCellSize();
}

void GraphicsGrid::set_background_color(QColor color) {
  scene_.setBackgroundBrush(color);
}

void GraphicsGrid::DrawCell(int x, int y, QPen &pen, QBrush &brush) {
  QRectF cell_rect(x * cell_width_, y * cell_height_, cell_width_,
                   cell_height_);
  scene_.addRect(cell_rect, pen, brush);
}

void GraphicsGrid::DrawWall(int x, int y, QPen &pen, CellSide pos) {
  QLineF wall;

  if (pos == CellSide::Bottom) {
    QPoint p1(x * cell_width_, y * cell_height_ + cell_height_);
    QPoint p2(x * cell_width_ + cell_width_, y * cell_height_ + cell_height_);
    wall.setPoints(p1, p2);
  } else if (pos == CellSide::Top) {
    QPoint p1(x * cell_width_, y * cell_height_);
    QPoint p2(x * cell_width_ + cell_width_, y * cell_height_);
    wall.setPoints(p1, p2);
  } else if (pos == CellSide::Right) {
    QPoint p1(x * cell_width_ + cell_width_, y * cell_height_);
    QPoint p2(x * cell_width_ + cell_width_, y * cell_height_ + cell_height_);
    wall.setPoints(p1, p2);
  } else if (pos == CellSide::Left) {
    QPoint p1(x * cell_width_, y * cell_height_);
    QPoint p2(x * cell_width_, y * cell_height_ + cell_height_);
    wall.setPoints(p1, p2);
  }
  scene_.addLine(wall, pen);
}

void GraphicsGrid::DrawRoutePoint(int x, int y, QPen &pen, QBrush &brush) {
  double x_offset = cell_width_ / 4;
  double y_offset = cell_height_ / 4;
  QPointF left_top(x * cell_width_ + x_offset, y * cell_height_ + y_offset);
  QPointF right_bottom(x * cell_width_ + cell_width_ - x_offset,
                       y * cell_height_ + cell_height_ - y_offset);
  scene_.addEllipse(QRectF(left_top, right_bottom), pen, brush);
  route_points_.push(scene_.items().at(0));
}

void GraphicsGrid::DrawRouteChunk(int x, int y, QPen &pen, CellSide from,
                                  CellSide to) {
  QPointF p[2];
  QPointF center(x * cell_width_ + cell_width_ / 2,
                 y * cell_height_ + cell_height_ / 2);
  int i = 0;

  if (from == CellSide::Top || to == CellSide::Top) {
    p[i] = QPointF(x * cell_width_ + cell_width_ / 2, y * cell_height_);
    ++i;
  }
  if (from == CellSide::Bottom || to == CellSide::Bottom) {
    p[i] = QPointF(x * cell_width_ + cell_width_ / 2,
                   y * cell_height_ + cell_height_);
    ++i;
  }
  if (from == CellSide::Left || to == CellSide::Left) {
    p[i] = QPointF(x * cell_width_, y * cell_height_ + cell_height_ / 2);
    ++i;
  }
  if (from == CellSide::Right || to == CellSide::Right) {
    p[i] = QPointF(x * cell_width_ + cell_width_,
                   y * cell_height_ + cell_height_ / 2);
    ++i;
  }
  if (i < 2) {
    p[1] = p[0];
  }
  scene_.addLine(QLineF(p[0], center), pen);
  route_chunks_.push(scene_.items().at(0));
  scene_.addLine(QLineF(center, p[1]), pen);
  route_chunks_.push(scene_.items().at(0));
}

void GraphicsGrid::DeleteRoutePoints() {
  while (!route_points_.empty()) {
    scene_.removeItem(route_points_.front());
    route_points_.pop_front();
  }
}

void GraphicsGrid::DeleteRoute() {
  while (!route_chunks_.empty()) {
    scene_.removeItem(route_chunks_.front());
    route_chunks_.pop_front();
  }
}

void GraphicsGrid::Clear() { scene_.clear(); }

void GraphicsGrid::mousePressEvent(QMouseEvent *event) {
  int x = event->pos().x() / cell_width_;
  int y = event->pos().y() / cell_height_;
  emit CellClicked(x, y);
}

void GraphicsGrid::CalculateCellSize() {
  cell_width_ = kBaseSize / (double)grid_width_;
  cell_height_ = kBaseSize / (double)grid_height_;
}

}  // namespace s21
