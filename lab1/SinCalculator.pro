QT       += core gui widgets

TEMPLATE  = app
TARGET    = SinCalculator

CONFIG   += c++17

# Заголовки ищутся от каталога src: "model/sinmodel.h", "view/mainwindow.h" и т.д.
INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/model/sinmodel.cpp \
    src/view/mainwindow.cpp \
    src/view/inputdialog.cpp \
    src/controller/sincontroller.cpp

HEADERS += \
    src/model/sinmodel.h \
    src/view/mainwindow.h \
    src/view/inputdialog.h \
    src/controller/sincontroller.h
