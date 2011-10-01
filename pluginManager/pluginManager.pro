#-------------------------------------------------
#
# Project created by QtCreator 2011-10-01T21:06:50
#
#-------------------------------------------------

QT       -= gui

TARGET = pluginManager
TEMPLATE = lib

DEFINES += PLUGINMANAGER_LIBRARY

SOURCES += PluginManager.cpp

HEADERS += PluginManager.h\
        pluginManager_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE095B11B
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = pluginManager.dll
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
