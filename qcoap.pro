#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T23:03:02
#
#-------------------------------------------------

QT       += core network
QT       -= gui
CONFIG += c++11

TARGET = qcoap
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    coap_pdu.cpp \
    coapnode.cpp

HEADERS += \
    coap_pdu.h \
    sysdep.h \
    coapnode.h
