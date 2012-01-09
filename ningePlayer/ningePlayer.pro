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

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../output/release/lib -lcore
else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../output/debug/lib -lcored

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

FORMS += \
    Player.ui

RESOURCES += \
    ningePlayerRC.qrc

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
UI_DIR = $$OUTPUT_DIR/build/ui
OBJECTS_DIR = $$OUTPUT_DIR/build/obj
MOC_DIR = $$OUTPUT_DIR/build/moc

unix:{
  target.path = $$OUTPUT_DIR/bin/ningePlugins
  INSTALLS += target
}
