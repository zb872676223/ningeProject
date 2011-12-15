#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T10:34:19
#
#-------------------------------------------------

TARGET = ningeMain
TEMPLATE = lib

include(../version.pri)

CONFIG += create_prl

DEFINES += NINGEMAIN_LIBRARY

SOURCES += NingeMain.cpp \
    Main.cpp

HEADERS += NingeMain.h\
        ningeMain_global.h \
    Main.h \
    ../core/CorePluginInterface.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEA3E5689
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ningeMain.dll
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

FORMS += \
    Main.ui

RESOURCES += \
    ningeMain.qrc
