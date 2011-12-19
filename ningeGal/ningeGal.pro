#-------------------------------------------------
#
# Project created by QtCreator 2011-12-15T15:03:41
#
#-------------------------------------------------

QT       += xml

TARGET = ningeGal
TEMPLATE = lib

include(../version.pri)

CONFIG += create_prl

DEFINES += NINGEGAL_LIBRARY

SOURCES += NingeGal.cpp \
    Gal.cpp \
    GalPixmapItem.cpp \
    GalTextItem.cpp

HEADERS += NingeGal.h\
        ningeGal_global.h \
    Gal.h \
    ../core/CorePluginInterface.h \
    GalPixmapItem.h \
    GalTextItem.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE85E2898
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ningeGal.dll
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

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../output/release/lib -lcore
else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../output/debug/lib -lcored

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

FORMS += \
    Gal.ui

UI_DIR = ./ui

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/debug/lib
    DLLDESTDIR = ../output/debug/bin/ningePlugins
    unix:{
      target.path = ../output/debug/bin/ningePlugins
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
    DLLDESTDIR = ../output/release/bin/ningePlugins
    unix:{
      target.path = ../output/release/bin/ningePlugins
      INSTALLS += target
    }
  } else {
    DESTDIR = ../output/release/bin
  }
  OBJECTS_DIR = ./release/obj
  MOC_DIR = ./release/moc
}
