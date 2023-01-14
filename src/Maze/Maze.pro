QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = View_module/Resources/icon.icns

SOURCES += \
    View_module/cave.cc \
    View_module/graphicsgrid.cc \
    View_module/maze.cc \
    main.cc \
    View_module/mainwindow.cc \
    Controller_module/maze_controller.cc \
    Model_module/maze_model.cc \
    Model_module/cave/cave_gen.cc \
    Model_module/maze/maze_gen.cc \
    Model_module/maze/route_finder.cc

HEADERS += \
    View_module/cave.h \
    View_module/graphicsgrid.h \
    View_module/mainwindow.h \
    View_module/global_settings.h \
    Controller_module/maze_controller.h \
    Model_module/maze_model.h \
    Model_module/cave/cave_gen.h \
    Model_module/maze/maze_gen.h \
    View_module/maze.h \
    Model_module/maze/route_finder.h \
    include/maze_location.h

FORMS += \
    View_module/cave.ui \
    View_module/mainwindow.ui \
    View_module/maze.ui

TRANSLATIONS += \
    Maze_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    View_module/resources.qrc
