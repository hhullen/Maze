#ifndef CAVE_H_
#define CAVE_H_

#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QRegularExpression>
#include <QTimer>
#include <QWidget>
#include <vector>

#include "graphicsgrid.h"

using std::vector;

namespace Ui {
class Cave;
}

namespace s21 {

class Cave : public QWidget {
  Q_OBJECT

 public:
  explicit Cave(QFont font, QWidget *parent = nullptr);
  ~Cave();

  void set_cave(vector<vector<bool>> *cave);
  void set_area_size(int size);
  void get_parameters(int *size, double *chance, bool *border, int *birth,
                      int *death);

 signals:
  void SendMessageSignal(QString message);
  void UpdatedSignal();
  void UpdateRequestSignal();
  bool NextRenderSignal();
  void ResetSignal();
  void RandomizeSignal(bool bordered);
  bool LoadFileSignal(QString &path);

 private:
  enum class GenMode { Random, File };
  enum class RenderMode { Manually, Automatic };

  Ui::Cave *ui_;
  GraphicsGrid graphics_field_;
  QGridLayout *cave_layout_;
  vector<vector<bool>> *cave_;
  QString file_path_;
  QTimer timer_;
  QRegularExpression name_pattern_;
  bool under_file_opening_;

  void SetElementsFont(QFont &font);
  void FillCaveArea();
  void ChangeGenerationMode(int index);
  void ChangeRenderingMode(int index);
  bool OpenCaveFile();
  void ChangeSize();
  void Render();
  void RenderIteration();
  void RunRendering();
  void TryRender();
  void Reset();

  void SlotSignalConnections();
};

}  // namespace s21

#endif  // CAVE_H_
