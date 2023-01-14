#ifndef GRAPHICSGRID_H_
#define GRAPHICSGRID_H_

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QStack>

namespace s21 {

const int kBaseSize = 500;
enum class CellSide { Left, Top, Right, Bottom };

class GraphicsGrid : public QGraphicsView {
  Q_OBJECT

 public:
  explicit GraphicsGrid(QGraphicsView *parent = nullptr);
  ~GraphicsGrid();

  void set_grid_size(int size);
  void set_background_color(QColor color);
  void DrawCell(int x, int y, QPen &pen, QBrush &brush);
  void DrawWall(int x, int y, QPen &pen, CellSide pos);
  void DrawRoutePoint(int x, int y, QPen &pen, QBrush &brush);
  void DrawRouteChunk(int x, int y, QPen &pen, CellSide from, CellSide to);
  void DeleteRoutePoints();
  void DeleteRoute();
  void Clear();

 signals:
  void CellClicked(int x, int y);

 private:
  QGraphicsScene scene_;
  QSizePolicy size_policy_;
  int grid_width_;
  int grid_height_;
  double cell_width_;
  double cell_height_;
  QStack<QGraphicsItem *> route_points_;
  QStack<QGraphicsItem *> route_chunks_;

  void mousePressEvent(QMouseEvent *event) override;
  void CalculateCellSize();
};

}  // namespace s21

#endif  // GRAPHICSGRID_H_
