#-------------------------------------------------
#
# Project created by QtCreator 2012-01-30T13:29:36
#
#-------------------------------------------------

QT       += gui

TARGET = ningeConsole
TEMPLATE = lib

include(../version.pri)

DEFINES += NINGECONSOLE_LIBRARY

SOURCES += NingeConsole.cpp \
    Console.cpp

HEADERS += NingeConsole.h\
        ningeConsole_global.h \
    Console.h \
    ../core/CorePluginInterface.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4B660D0
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ningeConsole.dll
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
  DLLDESTDIR = $$OUTPUT_DIR/bin/ningePlugins
} else {
  DESTDIR = $$OUTPUT_DIR/bin
}
UI_DIR = $$OUTPUT_DIR/build/ui/$$TARGET
OBJECTS_DIR = $$OUTPUT_DIR/build/obj/$$TARGET
MOC_DIR = $$OUTPUT_DIR/build/moc/$$TARGET
RCC_DIR = $$OUTPUT_DIR/build/rcc/$$TARGET

unix:{
  target.path = $$OUTPUT_DIR/bin/ningePlugins
  INSTALLS += target
}

FORMS += \
    Console.ui

RESOURCES += \
    ningeConsole.qrc
