#-------------------------------------------------
#
# Project created by QtCreator 2011-10-01T21:16:26
#
#-------------------------------------------------

QT    += core gui xml

TARGET = core
TEMPLATE = lib

include(../version.pri)

CONFIG += create_prl

DEFINES += CORE_LIBRARY

SOURCES += Core.cpp \
    PluginManager.cpp \
    NMLReader.cpp \
    GlobalSetting.cpp

HEADERS += Core.h\
        core_global.h \
    PluginManager.h \
    CorePluginInterface.h \
    NMLReader.h \
    GlobalSetting.h

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
RCC_DIR = $$OUTPUT_DIR/build/rcc/$$TARGET

unix:{
  target.path = $$OUTPUT_DIR/bin
  INSTALLS += target
}
