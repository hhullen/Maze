#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QFont font, MazeController *controller, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      controller_(controller),
      main_font_(font),
      main_widget_(nullptr) {
  this->setFont(main_font_);
  ui_->setupUi(this);

  main_layout_ = new QGridLayout();
  main_layout_->setSpacing(0);
  main_layout_->setAlignment(Qt::AlignTop);
  main_layout_->setContentsMargins(0, 0, 0, 0);
  ui_->centralwidget->setLayout(main_layout_);
  cave_widget_ = new Cave(main_font_);
  cave_widget_->set_cave(controller_->get_cave());
  maze_widget_ = new Maze(main_font_);
  controller_->set_maze_size(maze_widget_->get_size());
  maze_widget_->set_right_walls_matrix(controller_->get_right_walls());
  maze_widget_->set_bottom_walls_matrix(controller_->get_bottom_walls());
  maze_widget_->set_route(controller_->get_route());
  maze_widget_->Update();
  ChangeMode(maze_widget_);

  SlotSignalConnections();
}

MainWindow::~MainWindow() {
  delete ui_;
  delete cave_widget_;
  delete main_layout_;
}

void MainWindow::ChangeMode(QWidget *widget) {
  if (main_widget_) {
    main_widget_->setVisible(false);
    main_layout_->removeWidget(main_widget_);
  }
  main_widget_ = widget;
  main_widget_->setVisible(true);
  main_layout_->addWidget(main_widget_);
}

bool MainWindow::LoadCaveFile(QString &path) {
  string file_path = path.toStdString();
  return controller_->LoadCaveFile(file_path);
}

bool MainWindow::LoadMazeFile(QString &path) {
  string file_path = path.toStdString();
  return controller_->LoadMazeFile(file_path);
}

void MainWindow::UpdateCaveWidget() {
  cave_widget_->set_area_size(controller_->get_cave()->size());
}

void MainWindow::UpdateCaveState() {
  int size, birth, death;
  double chance;
  bool border;

  cave_widget_->get_parameters(&size, &chance, &border, &birth, &death);
  controller_->set_cave_parameters(size, chance, border, birth, death);
}

void MainWindow::UpdateMazeWidget() {
  maze_widget_->set_size(controller_->get_bottom_walls()->size());
}

void MainWindow::UpdateMazeState() {
  controller_->set_maze_size(maze_widget_->get_size());
}

void MainWindow::RandomizeCave(bool bordered) {
  controller_->RandomizeCave(bordered);
}

void MainWindow::RandomizeMaze() {
  controller_->RandomizeMaze();
  maze_widget_->Update();
}

bool MainWindow::RenderNext() { return controller_->RenderNextCave(); }

void MainWindow::ShowCaveWidget() { ChangeMode(cave_widget_); }

void MainWindow::ShowMazeWidget() { ChangeMode(maze_widget_); }

void MainWindow::GetMazeRoute() {
  const QStack<QPoint> &points = maze_widget_->get_route_points();
  Point start(points.back().x(), points.back().y());
  Point end(points.front().x(), points.front().y());
  if (controller_->FindRouteFromTo(start, end)) {
    maze_widget_->DrawRoute();
  }
}

void MainWindow::ShowMessage(QString message) {
  ui_->statusbar->showMessage(message, 6000);
}

void MainWindow::SlotSignalConnections() {
  connect(cave_widget_, &Cave::LoadFileSignal, this, &MainWindow::LoadCaveFile);
  connect(cave_widget_, &Cave::UpdateRequestSignal, this,
          &MainWindow::UpdateCaveWidget);
  connect(cave_widget_, &Cave::UpdatedSignal, this,
          &MainWindow::UpdateCaveState);
  connect(cave_widget_, &Cave::RandomizeSignal, this,
          &MainWindow::RandomizeCave);
  connect(cave_widget_, &Cave::SendMessageSignal, this,
          &MainWindow::ShowMessage);
  connect(cave_widget_, &Cave::NextRenderSignal, this, &MainWindow::RenderNext);
  connect(ui_->actionCave, &QAction::triggered, this,
          &MainWindow::ShowCaveWidget);
  connect(ui_->actionMaze, &QAction::triggered, this,
          &MainWindow::ShowMazeWidget);
  connect(maze_widget_, &Maze::SendMessageSignal, this,
          &MainWindow::ShowMessage);
  connect(maze_widget_, &Maze::LoadFileSignal, this, &MainWindow::LoadMazeFile);
  connect(maze_widget_, &Maze::RandomizeSignal, this,
          &MainWindow::RandomizeMaze);
  connect(maze_widget_, &Maze::UpdatedSignal, this,
          &MainWindow::UpdateMazeState);
  connect(maze_widget_, &Maze::UpdateRequestSignal, this,
          &MainWindow::UpdateMazeWidget);
  connect(maze_widget_, &Maze::CalculateRouteSignal, this,
          &MainWindow::GetMazeRoute);
}

}  // namespace s21
