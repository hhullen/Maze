#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QGridLayout>
#include <QMainWindow>

#include "../Controller_module/maze_controller.h"
#include "cave.h"
#include "maze.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QFont font, MazeController *controller, QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui_;
  MazeController *controller_;
  QFont main_font_;

  QGridLayout *main_layout_;
  QWidget *main_widget_;
  Cave *cave_widget_;
  Maze *maze_widget_;

  void ChangeMode(QWidget *widget);
  bool LoadCaveFile(QString &path);
  bool LoadMazeFile(QString &path);
  void UpdateCaveWidget();
  void UpdateCaveState();
  void UpdateMazeWidget();
  void UpdateMazeState();
  void RandomizeCave(bool bordered);
  void RandomizeMaze();
  bool RenderNext();
  void ShowCaveWidget();
  void ShowMazeWidget();
  void GetMazeRoute();

  void ShowMessage(QString message);
  void SlotSignalConnections();
};

}  // namespace s21

#endif  // MAINWINDOW_H_
