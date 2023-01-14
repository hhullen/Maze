#ifndef MAZE_H_
#define MAZE_H_

#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QWidget>
#include <vector>

#include "graphicsgrid.h"
#include "include/maze_location.h"

using s21::Directions;
using s21::Location;
using s21::Point;
using std::vector;

namespace Ui {
class Maze;
}

namespace s21 {

class Maze : public QWidget {
  Q_OBJECT

 public:
  explicit Maze(QFont font, QWidget *parent = nullptr);
  ~Maze();

  void set_bottom_walls_matrix(vector<vector<bool>> *matrix);
  void set_right_walls_matrix(vector<vector<bool>> *matrix);
  int get_size();
  void set_size(int size);
  void Update();
  const QStack<QPoint> &get_route_points();
  void set_route(vector<Location> *route);
  void DrawRoute();

 signals:
  void SendMessageSignal(QString message);
  void RandomizeSignal();
  void UpdateRequestSignal();
  void UpdatedSignal();
  bool LoadFileSignal(QString &path);
  void CalculateRouteSignal();

 private:
  enum class GenMode { Random, File };
  enum class RenderMode { Manually, Automatic };

  Ui::Maze *ui_;
  GraphicsGrid maze_field_;
  QRegularExpression name_pattern_;
  QGridLayout *maze_layout_;
  QStack<QPoint> route_points_;
  bool under_file_opening_;

  struct Colors {
    QPen border_pen_;
    QPen wall_pen_;
    QPen path_pen_;
    QPen point_pen_;
    QBrush point_brush_;
  } colors_;

  vector<vector<bool>> *right_walls_matrix_;
  vector<vector<bool>> *bottom_walls_matrix_;
  vector<Location> *route_;

  void DrawWallsTo(int i, int j);
  void DrawBorder(QPen &pen);
  void DrawSideBorder(int x, int y, int max, QPen &pen);
  void AddCheckPoint(int x, int y);
  void SetPointLabel(QString prefix, QLabel *label, const QPoint &point);
  void ChangeGenerationMode(int index);
  bool OpenCaveFile();
  void CleanRoute();
  void ChangeSize();
  CellSide GetCellSideFromDirection(Directions dir);
  CellSide InverseCellSide(CellSide side);

  void SlotSignalConnections();
};

}  // namespace s21

#endif  // MAZE_H_
