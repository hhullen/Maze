#include "maze.h"

#include "ui_maze.h"

namespace s21 {

Maze::Maze(QFont font, QWidget *parent)
    : QWidget(parent), ui_(new Ui::Maze), under_file_opening_(false) {
  this->setFont(font);
  ui_->setupUi(this);
  ui_->btn_generate_new->setFont(font);
  ui_->cb_gen_mode->setFont(font);
  ui_->label_gen_mode->setFont(font);
  ui_->label_size->setFont(font);
  ui_->label_file_name->setFont(font);
  ui_->i_size->setFont(font);
  ui_->btn_clean_route->setFont(font);
  ui_->label_p1->setFont(font);
  ui_->label_p2->setFont(font);

  name_pattern_.setPattern("[^\\/]*$");

  maze_layout_ = new QGridLayout();
  maze_layout_->setSpacing(0);
  maze_layout_->setAlignment(Qt::AlignTop);
  maze_layout_->setContentsMargins(0, 0, 0, 0);
  ui_->maze_field->setLayout(maze_layout_);
  maze_layout_->addWidget(&maze_field_);

  colors_.border_pen_.setColor(QColor(23, 22, 239));
  colors_.border_pen_.setWidth(4);
  colors_.wall_pen_.setColor(QColor(23, 22, 239));
  colors_.wall_pen_.setWidth(2);
  colors_.path_pen_.setColor(QColor(248, 176, 144));
  colors_.path_pen_.setWidth(2);
  colors_.path_pen_.setStyle(Qt::PenStyle::DotLine);
  colors_.point_pen_.setColor(QColor(248, 176, 144));
  colors_.point_pen_.setWidth(2);
  colors_.point_brush_.setColor(QColor(248, 176, 144));
  colors_.point_brush_.setStyle(Qt::BrushStyle::SolidPattern);

  SlotSignalConnections();
  DrawBorder(colors_.border_pen_);
}

Maze::~Maze() { delete ui_; }

void Maze::set_bottom_walls_matrix(vector<vector<bool>> *matrix) {
  bottom_walls_matrix_ = matrix;
}

void Maze::set_right_walls_matrix(vector<vector<bool>> *matrix) {
  right_walls_matrix_ = matrix;
}

int Maze::get_size() { return ui_->i_size->value(); }

void Maze::set_size(int size) { ui_->i_size->setValue(size); }

void Maze::Update() {
  CleanRoute();
  maze_field_.Clear();
  maze_field_.set_grid_size(ui_->i_size->value());

  for (int i = 0; i < ui_->i_size->value(); ++i) {
    for (int j = 0; j < ui_->i_size->value(); ++j) {
      DrawWallsTo(i, j);
    }
  }
  DrawBorder(colors_.border_pen_);
}

const QStack<QPoint> &Maze::get_route_points() { return route_points_; }

void Maze::set_route(vector<Location> *route) { route_ = route; }

void Maze::DrawRoute() {
  maze_field_.DeleteRoute();
  Location &loc = (*route_).back();
  Point &pos = loc.position();
  CellSide from = GetCellSideFromDirection(loc.previous());
  CellSide to = from;
  maze_field_.DrawRouteChunk(pos.x(), pos.y(), colors_.path_pen_, from, to);
  for (size_t i = route_->size() - 2; i >= 1; --i) {
    loc = (*route_)[i];
    pos = loc.position();
    from = InverseCellSide(to);
    to = GetCellSideFromDirection(loc.previous());
    maze_field_.DrawRouteChunk(pos.x(), pos.y(), colors_.path_pen_, from, to);
  }
  loc = (*route_).front();
  pos = loc.position();
  to = InverseCellSide(to);
  maze_field_.DrawRouteChunk(pos.x(), pos.y(), colors_.path_pen_, to, to);
}

void Maze::DrawWallsTo(int i, int j) {
  if (right_walls_matrix_ && (*right_walls_matrix_)[j][i]) {
    maze_field_.DrawWall(i, j, colors_.wall_pen_, CellSide::Right);
  }
  if (bottom_walls_matrix_ && (*bottom_walls_matrix_)[j][i]) {
    maze_field_.DrawWall(i, j, colors_.wall_pen_, CellSide::Bottom);
  }
}

void Maze::DrawBorder(QPen &pen) {
  int size = ui_->i_size->value();
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      DrawSideBorder(i, j, size, pen);
    }
  }
}

void Maze::DrawSideBorder(int x, int y, int max, QPen &pen) {
  if (x == 0) {
    maze_field_.DrawWall(x, y, pen, CellSide::Left);
  } else if (x == max - 1) {
    maze_field_.DrawWall(x, y, pen, CellSide::Right);
  }
  if (y == 0) {
    maze_field_.DrawWall(x, y, pen, CellSide::Top);
  } else if (y == max - 1) {
    maze_field_.DrawWall(x, y, pen, CellSide::Bottom);
  }
}

void Maze::AddCheckPoint(int x, int y) {
  if (route_points_.size() < 2) {
    maze_field_.DrawRoutePoint(x, y, colors_.point_pen_, colors_.point_brush_);
    route_points_.push_front(QPoint(x, y));
    if (route_points_.size() == 1) {
      SetPointLabel("point 1:  ", ui_->label_p1, route_points_.at(0));
    } else {
      SetPointLabel("point 2:  ", ui_->label_p2, route_points_.at(0));
      emit CalculateRouteSignal();
    }
  } else {
  }
}

void Maze::SetPointLabel(QString prefix, QLabel *label, const QPoint &point) {
  label->setText(prefix + "(" + QString::number(point.x() + 1) + ";" +
                 QString::number(point.y() + 1) + ")");
}

void Maze::ChangeGenerationMode(int index) {
  if (index == (int)GenMode::Random) {
    emit RandomizeSignal();
    ui_->label_file_name->clear();
    ui_->i_size->setDisabled(false);
    ui_->btn_generate_new->setDisabled(false);
    Update();
  } else if (index == (int)GenMode::File && OpenCaveFile()) {
    emit UpdateRequestSignal();
    ui_->i_size->setDisabled(true);
    ui_->btn_generate_new->setDisabled(true);
    emit SendMessageSignal("File loaded succesfully");
    Update();
  } else {
    emit SendMessageSignal("File have not been loaded");
    ui_->cb_gen_mode->setCurrentIndex((int)GenMode::Random);
    ChangeGenerationMode((int)GenMode::Random);
    ui_->label_file_name->clear();
  }
}

bool Maze::OpenCaveFile() {
  bool returnable = false;

  under_file_opening_ = true;
  QString file_path = QFileDialog::getOpenFileName(this, "Open maze file",
                                                   "/Users", "maze (*.maze);;");
  if (!file_path.isEmpty()) {
    QRegularExpressionMatch match_ = name_pattern_.match(file_path);
    ui_->label_file_name->setText(match_.captured());
    returnable = emit LoadFileSignal(file_path);
  }

  return returnable;
}

void Maze::CleanRoute() {
  maze_field_.DeleteRoute();
  maze_field_.DeleteRoutePoints();
  route_points_.clear();
  ui_->label_p1->setText("point 1:");
  ui_->label_p2->setText("point 2:");
}

void Maze::ChangeSize() {
  CleanRoute();
  emit UpdatedSignal();
  if (!under_file_opening_) {
    emit RandomizeSignal();
  } else {
    under_file_opening_ = false;
  }
}

CellSide Maze::GetCellSideFromDirection(Directions dir) {
  CellSide returnable;

  if (dir == Directions::Down) {
    returnable = CellSide::Bottom;
  } else if (dir == Directions::Up) {
    returnable = CellSide::Top;
  } else if (dir == Directions::Left) {
    returnable = CellSide::Left;
  } else {
    returnable = CellSide::Right;
  }

  return returnable;
}

CellSide Maze::InverseCellSide(CellSide side) {
  CellSide returnable;

  if (side == CellSide::Bottom) {
    returnable = CellSide::Top;
  } else if (side == CellSide::Top) {
    returnable = CellSide::Bottom;
  } else if (side == CellSide::Left) {
    returnable = CellSide::Right;
  } else {
    returnable = CellSide::Left;
  }

  return returnable;
}

void Maze::SlotSignalConnections() {
  connect(&maze_field_, &GraphicsGrid::CellClicked, this, &Maze::AddCheckPoint);
  connect(ui_->cb_gen_mode, &QComboBox::activated, this,
          &Maze::ChangeGenerationMode);
  connect(ui_->btn_clean_route, &QPushButton::clicked, this, &Maze::CleanRoute);
  connect(ui_->btn_generate_new, &QPushButton::clicked, this,
          &Maze::RandomizeSignal);
  connect(ui_->i_size, &QSpinBox::valueChanged, this, &Maze::ChangeSize);
}

}  // namespace s21
