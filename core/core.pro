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
    NMLReader.cpp

HEADERS += Core.h\
        core_global.h \
    PluginManager.h \
    CorePluginInterface.h \
    NMLReader.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE6C79BF2
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = core.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

UI_DIR = ./ui

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/debug/lib
    DLLDESTDIR = ../output/debug/bin
    unix:{
      target.path = ../output/debug/bin
      INSTALLS += target
    }
  } else {
    DESTDIR = ../output/debug/bin
  }
  OBJECTS_DIR = ./debug/obj
  MOC_DIR = ./debug/moc
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/release/lib
    DLLDESTDIR = ../output/release/bin
    unix:{
      target.path = ../output/release/bin
      INSTALLS += target
    }
  } else {
    DESTDIR = ../output/release/bin
  }
  OBJECTS_DIR = ./release/obj
  MOC_DIR = ./release/moc
}


