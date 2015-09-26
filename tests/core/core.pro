#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T17:57:15
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_coretest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_coretest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$TOP_SRCDIR/src/lib
LIBS += -L"$$TOP_SRCDIR/bin" -lqcoap
