#-------------------------------------------------
#
# Project created by QtCreator 2011-10-30T15:35:55
#
#-------------------------------------------------

QT       += phonon

TARGET = ningePlayer
TEMPLATE = lib

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

#-------------------------------------------------
#
# PS:应将此配置放置在pro文件的最后, 否则可能会不起作用
#
# 以下配置为程序输出目录配置模板, 根据需要自行修改
# 其中, 默认认为整个工程使用SubDir工程, 且子工程为非SubDir工程.
# 如有不同, 请根据情况进行修改
#
#
# 主要就是配置 TARGET(目标名字) 以及 DESTDIR(目标目录), DLLDESTDIR(动态库自动复制目录)
# 将TARGET在debug状况下加上d后缀
# 将DESTDIR设置到bin目录下,而在目标为动态库时修改为放入lib目录下
# 将DLLDESTDIR设置为bin目录, 用于将动态库的自动发布复制
#
# 配置后, release以及debug目录将会出现在目标路径的output文件夹下
# 并且会将子工程的ui生成文件放入子工程相应的ui目录下
#
#-------------------------------------------------

#设置UI文件目录
UI_DIR = ./ui

CONFIG(debug, debug|release) {
  #设置debug配置下编译生成文件的路径
  TARGET = $$join(TARGET,,,d)   #为debug版本生成的文件增加d的后缀
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/debug/lib        #将库放在lib文件夹下
    DLLDESTDIR = ../output/debug/bin     #将动态库自动拷贝至bin目录下
  } else {
    DESTDIR = ../output/debug/bin        #将应用程序放在bin目录下
  }
  OBJECTS_DIR = ./debug/obj     #将生成的对象文件放在专门的obj文件夹下
  MOC_DIR = ./debug/moc         #将QT自动生成的对象放在moc文件夹下
} else {
  #设置release配置下编译生成文件的路径
  contains(TEMPLATE, "lib") {
    DESTDIR = ../output/release/lib        #将库放在lib文件夹下
    DLLDESTDIR = ../output/release/bin     #将动态库自动拷贝至bin目录下
  } else {
    DESTDIR = ../output/release/bin        #将应用程序放在bin目录下
  }
  OBJECTS_DIR = ./release/obj   #将生成的对象文件放在专门的obj文件夹下
  MOC_DIR = ./release/moc       #将QT自动生成的对象放在moc文件夹下
}

FORMS += \
    Player.ui

RESOURCES += \
    ningePlayerRC.qrc






