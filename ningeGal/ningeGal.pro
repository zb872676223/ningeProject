#-------------------------------------------------
#
# Project created by QtCreator 2011-12-15T15:03:41
#
#-------------------------------------------------

QT       += xml opengl

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
    Gal.ui

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
