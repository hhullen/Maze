#include "View_module/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "View_module/global_settings.h"

using s21::MainWindow;
using s21::GlobalSettings;
using s21::MazeModel;
using s21::MazeController;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Maze_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MazeModel model;
    MazeController controller(&model);
    MainWindow view(GlobalSettings().font, &controller);

    view.setWindowTitle("Maze");
    view.show();

    return a.exec();
}
