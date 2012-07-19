#-------------------------------------------------
#
# Project created by QtCreator 2012-07-19T08:55:02
#
#-------------------------------------------------

QT       -= gui
QT += network

TARGET = ZMQtNetwork
TEMPLATE = lib

DEFINES += ZMQTNETWORK_LIBRARY

SOURCES += \
    ZMQtSubscriber_P.cpp \
    ZMQtServerWorker.cpp \
    ZMQtServer_P.cpp \
    ZMQtServer.cpp \
    ZMQtPublisher_P.cpp \
    ZMQtContext.cpp \
    ZMQtClient_P.cpp \
    ZMQtClient.cpp

HEADERS +=\
        ZMQtNetwork_global.h \
    ZMQtSubscriber_P.h \
    ZMQtServerWorker.h \
    ZMQtServer_P.h \
    ZMQtServer.h \
    ZMQtPublisher_P.h \
    ZMQtContext.h \
    ZMQtClient_P.h \
    ZMQtClient.h \
    INetwork.h

OTHER_FILES +=

unix: LIBS += -lzmq

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
RCC_DIR = $$OUTPUT_DIR/build/rcc/$$TARGET

unix:{
  target.path = $$OUTPUT_DIR/bin
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += C:/Developer/ZMQ/lib/libzmq.lib
else:win32:CONFIG(debug, debug|release): LIBS += C:/Developer/ZMQ/lib/libzmqd.lib

win32 {
INCLUDEPATH += C:/Developer/ZMQ/include
DEPENDPATH += C:/Developer/ZMQ/include
}
