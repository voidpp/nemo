#-------------------------------------------------
#
# Project created by QtCreator 2016-02-07T13:26:10
#
#-------------------------------------------------

QT += core gui widgets

TARGET = Nemo
TEMPLATE = app

CONFIG += c++11

include(qxtglobalshortcut5/qxt.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    filelogger.cpp

HEADERS  += mainwindow.h \
    config.h \
    filelogger.h

FORMS    += mainwindow.ui

LIBS += -luser32

RESOURCES += \
    resources.qrc
