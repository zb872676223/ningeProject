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

SOURCES += \
    Main.cpp \
    NingeMain.cpp

HEADERS +=\
        ningeMain_global.h \
    Main.h \
    ../core/CorePluginInterface.h \
    NingeMain.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEA3E5689
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ningeMain.dll
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

CONFIG(release, debug|release): LIBS += -L$$PWD/../output/release/lib -lcore
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../output/debug/lib -lcored

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

FORMS += \
    Main.ui

RESOURCES += \
    ningeMain.qrc

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
