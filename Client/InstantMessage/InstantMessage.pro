QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = Message.ico

CONFIG += c++17

# Shadow Build 会自动将构建目录设置为 $$OUT_PWD
DESTDIR = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/.obj
MOC_DIR = $$OUT_PWD/.moc
RCC_DIR = $$OUT_PWD/.rcc
UI_DIR = $$OUT_PWD/.ui

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
}

SOURCES += \
    HttpMgr.cpp \
    clickedlabel.cpp \
    global.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    timerbtn.cpp

HEADERS += \
    HttpMgr.h \
    Singleton.hpp \
    clickedlabel.h \
    global.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h \
    timerbtn.h

FORMS += \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini

config_file.path = $$OUT_PWD/bin
config_file.files = $$PWD/config.ini
CONFIG += file_copies
COPIES += config_file

# 确保UI_DIR目录存在
create_ui_dir.target = $$UI_DIR
create_ui_dir.commands = $$QMAKE_MKDIR $$create_ui_dir.target
QMAKE_EXTRA_TARGETS += create_ui_dir
PRE_TARGETDEPS += $$UI_DIR
