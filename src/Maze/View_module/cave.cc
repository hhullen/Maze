#include "cave.h"

#include "ui_cave.h"

namespace s21 {

Cave::Cave(QFont font, QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::Cave),
      cave_(nullptr),
      file_path_(""),
      under_file_opening_(false) {
  this->setFont(font);
  ui_->setupUi(this);
  SetElementsFont(font);

  name_pattern_.setPattern("[^\\/]*$");

  cave_layout_ = new QGridLayout();
  cave_layout_->setSpacing(0);
  cave_layout_->setAlignment(Qt::AlignTop);
  cave_layout_->setContentsMargins(0, 0, 0, 0);
  ui_->cave_field->setLayout(cave_layout_);
  ui_->cave_field->layout()->addWidget(&graphics_field_);
  ui_->i_render_delay->setDisabled(true);

  SlotSignalConnections();
}

Cave::~Cave() {
  graphics_field_.Clear();
  delete cave_layout_;
  delete ui_;
}

void Cave::set_cave(vector<vector<bool>> *cave) {
  cave_ = cave;
  set_area_size(cave->size());
  FillCaveArea();
}

void Cave::set_area_size(int size) { ui_->i_size->setValue(size); }

void Cave::get_parameters(int *size, double *chance, bool *border, int *birth,
                          int *death) {
  if (size) {
    *size = ui_->i_size->value();
  }
  if (chance) {
    *chance = ui_->d_init_chance->value();
  }
  if (border) {
    *border = ui_->cb_border_mode->currentIndex();
  }
  if (birth) {
    *birth = ui_->i_birth_limit->value();
  }
  if (death) {
    *death = ui_->i_death_limit->value();
  }
}

void Cave::SetElementsFont(QFont &font) {
  ui_->label_birth_limit->setFont(font);
  ui_->label_init_chance->setFont(font);
  ui_->label_death_limit->setFont(font);
  ui_->label_gen_mode->setFont(font);
  ui_->label_render_delay->setFont(font);
  ui_->label_render_mode->setFont(font);
  ui_->label_file_name->setFont(font);
  ui_->btn_reset->setFont(font);
  ui_->btn_render->setFont(font);
  ui_->i_render_delay->setFont(font);
  ui_->i_birth_limit->setFont(font);
  ui_->i_death_limit->setFont(font);
  ui_->cb_gen_mode->setFont(font);
  ui_->cb_border_mode->setFont(font);
  ui_->cb_render_mode->setFont(font);
  ui_->d_init_chance->setFont(font);
  ui_->label_size->setFont(font);
  ui_->i_size->setFont(font);
}

void Cave::FillCaveArea() {
  QPen pen_wall(QColor(165, 96, 33), 1, Qt::SolidLine, Qt::RoundCap,
                Qt::BevelJoin);
  QBrush brush_wall(QColor(191, 108, 13));
  QPen pen_space(QColor(0, 0, 0, 0), 0);
  QBrush brush_space(QColor(170, 170, 170));

  graphics_field_.Clear();
  graphics_field_.set_grid_size(ui_->i_size->value());
  for (int i = 0; i < ui_->i_size->value(); ++i) {
    for (int j = 0; j < ui_->i_size->value(); ++j) {
      if (cave_ && (*cave_)[i][j] == true) {
        graphics_field_.DrawCell(i, j, pen_wall, brush_wall);
      } else {
        graphics_field_.DrawCell(i, j, pen_space, brush_space);
      }
    }
  }
  ui_->btn_render->setDisabled(false);
}

void Cave::ChangeGenerationMode(int index) {
  if (index == (int)GenMode::Random) {
    emit RandomizeSignal(ui_->cb_border_mode->currentIndex());
    ui_->label_file_name->clear();
    ui_->i_size->setDisabled(false);
    ui_->d_init_chance->setDisabled(false);
    ui_->cb_border_mode->setDisabled(false);
    FillCaveArea();
  } else if (index == (int)GenMode::File && OpenCaveFile()) {
    emit UpdateRequestSignal();
    ui_->i_size->setDisabled(true);
    ui_->d_init_chance->setDisabled(true);
    ui_->cb_border_mode->setDisabled(true);
    emit SendMessageSignal("File loaded succesfully");
    FillCaveArea();
  } else {
    emit SendMessageSignal("File have not been loaded");
    ui_->cb_gen_mode->setCurrentIndex(0);
    ui_->label_file_name->clear();
  }
}

void Cave::ChangeRenderingMode(int index) {
  if (index == (int)RenderMode::Manually) {
    ui_->i_render_delay->setDisabled(true);
    ui_->btn_render->setText("Render next");
  } else if (index == (int)RenderMode::Automatic) {
    ui_->i_render_delay->setDisabled(false);
    ui_->btn_render->setText("Run");
  }
}

bool Cave::OpenCaveFile() {
  bool returnable = false;

  under_file_opening_ = true;
  QString file_path = QFileDialog::getOpenFileName(this, "Open cave file",
                                                   "/Users", "cave (*.cave);;");
  if (!file_path.isEmpty()) {
    file_path_ = file_path;
    QRegularExpressionMatch match_ = name_pattern_.match(file_path_);
    ui_->label_file_name->setText(match_.captured());
    returnable = emit LoadFileSignal(file_path);
  }

  return returnable;
}

void Cave::ChangeSize() {
  emit UpdatedSignal();
  if (!under_file_opening_) {
    emit RandomizeSignal(ui_->cb_border_mode->currentIndex());
  } else {
    under_file_opening_ = false;
  }
  FillCaveArea();
}

void Cave::Render() {
  ui_->d_init_chance->setDisabled(true);
  ui_->i_size->setDisabled(true);
  ui_->cb_border_mode->setDisabled(true);
  if (ui_->btn_render->text() == "Terminate") {
    timer_.stop();
    ChangeRenderingMode(ui_->cb_render_mode->currentIndex());
    ui_->i_birth_limit->setDisabled(false);
    ui_->i_death_limit->setDisabled(false);
    ui_->btn_reset->setDisabled(false);
  } else if (ui_->cb_render_mode->currentIndex() == (int)RenderMode::Manually) {
    RenderIteration();
  } else if (ui_->cb_render_mode->currentIndex() ==
             (int)RenderMode::Automatic) {
    ui_->btn_render->setText("Terminate");
    RunRendering();
  }
}

void Cave::RenderIteration() {
  emit UpdatedSignal();
  bool is_updated = emit NextRenderSignal();
  FillCaveArea();
  if (!is_updated) {
    ui_->btn_render->setDisabled(true);
    timer_.stop();
    ChangeRenderingMode(ui_->cb_render_mode->currentIndex());
    ui_->i_birth_limit->setDisabled(false);
    ui_->i_death_limit->setDisabled(false);
    ui_->btn_reset->setDisabled(false);
  }
}

void Cave::RunRendering() {
  ui_->i_birth_limit->setDisabled(true);
  ui_->i_death_limit->setDisabled(true);
  ui_->i_render_delay->setDisabled(true);
  ui_->btn_reset->setDisabled(true);

  timer_.start(ui_->i_render_delay->value());
  emit UpdatedSignal();
}

void Cave::TryRender() { ui_->btn_render->setDisabled(false); }

void Cave::Reset() {
  ui_->d_init_chance->setDisabled(false);
  ui_->i_size->setDisabled(false);
  ui_->cb_border_mode->setDisabled(false);
  timer_.stop();
  if (ui_->cb_gen_mode->currentIndex() == (int)GenMode::Random) {
    emit RandomizeSignal(ui_->cb_border_mode->currentIndex());
    FillCaveArea();
  } else if (ui_->cb_gen_mode->currentIndex() == (int)GenMode::File) {
    bool is_loaded = emit LoadFileSignal(file_path_);
    if (is_loaded) {
      FillCaveArea();
    } else {
      ChangeGenerationMode((int)GenMode::Random);
    }
  }
}

void Cave::SlotSignalConnections() {
  connect(ui_->cb_gen_mode, &QComboBox::activated, this,
          &Cave::ChangeGenerationMode);
  connect(ui_->i_size, &QSpinBox::valueChanged, this, &Cave::ChangeSize);
  connect(ui_->d_init_chance, &QDoubleSpinBox::valueChanged, this,
          &Cave::ChangeSize);
  connect(ui_->cb_border_mode, &QComboBox::currentIndexChanged, this,
          &Cave::ChangeSize);
  connect(ui_->cb_render_mode, &QComboBox::currentIndexChanged, this,
          &Cave::ChangeRenderingMode);
  connect(ui_->btn_render, &QPushButton::clicked, this, &Cave::Render);
  connect(ui_->i_birth_limit, &QSpinBox::valueChanged, this, &Cave::TryRender);
  connect(ui_->i_death_limit, &QSpinBox::valueChanged, this, &Cave::TryRender);
  connect(ui_->btn_reset, &QPushButton::clicked, this, &Cave::Reset);
  connect(&timer_, &QTimer::timeout, this, &Cave::RenderIteration);
}

}  // namespace s21
