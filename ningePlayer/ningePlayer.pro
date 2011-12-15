#-------------------------------------------------
#
# Project created by QtCreator 2011-10-30T15:35:55
#
#-------------------------------------------------

QT       += phonon

TARGET = ningePlayer
TEMPLATE = lib

include(../version.pri)

CONFIG += create_prl

DEFINES += NINGEPLAYER_LIBRARY

SOURCES += NingePlayer.cpp \
    Player.cpp

HEADERS += NingePlayer.h\
        ningePlayer_global.h \
    Player.h \
    ../core/CorePluginInterface.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE6C7E5CA
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ningePlayer.dll
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
    Player.ui

RESOURCES += \
    ningePlayerRC.qrc

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




