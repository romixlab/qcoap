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
    cantcoap.cpp \
    coap.cpp \
    coap_pdu.cpp

HEADERS += \
    cantcoap.h \
    dbg.h \
    coap.h \
    coap_private.h \
    coap_pdu.h
