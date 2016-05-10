#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T11:22:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_MAC_SDK = macosx10.11
CONFIG += c++11

TARGET = DHR_MDP
TEMPLATE = app


SOURCES += main.cpp\
    transport.cpp \
    editor.cpp \
    newcity.cpp \
    delivery.cpp \
    widget.cpp

HEADERS  += \
    multigraph.h \
    transport.h \
    editor.h \
    newcity.h \
    delivery.h \
    widget.h

FORMS    += widget.ui \
    editor.ui \
    newcity.ui \
    delivery.ui
