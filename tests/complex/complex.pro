#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T18:25:16
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = complex
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += $$TOP_SRCDIR/src/lib
LIBS += -L"$$TOP_SRCDIR/bin" -lqcoap

SOURCES += main.cpp
