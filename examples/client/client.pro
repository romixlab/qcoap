#-------------------------------------------------
#
# Project created by QtCreator 2015-06-30T22:36:45
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = client
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++11
DESTDIR = $$TOP_SRCDIR/bin

LIBS += -L"$$TOP_SRCDIR/bin" -lqcoap
INCLUDEPATH += $$TOP_SRCDIR/src/lib

TEMPLATE = app

SOURCES += main.cpp
