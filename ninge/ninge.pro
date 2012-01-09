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

CONFIG(release, debug|release): LIBS += -L$$PWD/../output/release/lib -lcore
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../output/debug/lib -lcored

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

CONFIG(debug, debug|release) {
  OUTPUT_DIR = $$PWD/../output/debug
  message(debug: $$OUTPUT_DIR)
  TARGET = $$join(TARGET,,,d)
} else {
  OUTPUT_DIR = $$PWD/../output/release
  message(release: $$OUTPUT_DIR)
}

contains(TEMPLATE, "lib") {
  DESTDIR = $$OUTPUT_DIR/lib
  DLLDESTDIR = $$OUTPUT_DIR/bin
} else {
  DESTDIR = $$OUTPUT_DIR/bin
}
UI_DIR = $$OUTPUT_DIR/build/ui/$$TARGET
OBJECTS_DIR = $$OUTPUT_DIR/build/obj/$$TARGET
MOC_DIR = $$OUTPUT_DIR/build/moc/$$TARGET

#unix:{
#  target.path = $$OUTPUT_DIR/bin
#  INSTALLS += target
#}
