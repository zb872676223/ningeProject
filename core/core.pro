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

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

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

unix:{
  target.path = $$OUTPUT_DIR/bin
  INSTALLS += target
}
