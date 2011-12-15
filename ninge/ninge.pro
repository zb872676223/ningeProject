#-------------------------------------------------
#
# Project created by QtCreator 2011-10-03T17:08:53
#
#-------------------------------------------------

QT       += core gui

TARGET = ninge
TEMPLATE = app

include(../version.pri)

SOURCES += main.cpp

HEADERS  +=

FORMS    +=

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../output/release/lib -lcore
else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../output/debug/lib -lcored

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

UI_DIR = ./ui

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/debug/lib
    DLLDESTDIR = ../output/debug/bin
  } else {
    DESTDIR = ../output/debug/bin
  }
  OBJECTS_DIR = ./debug/obj
  MOC_DIR = ./debug/moc
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/release/lib
    DLLDESTDIR = ../output/release/bin
  } else {
    DESTDIR = ../output/release/bin
  }
  OBJECTS_DIR = ./release/obj
  MOC_DIR = ./release/moc
}
