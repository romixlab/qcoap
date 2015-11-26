QT       += network qml
QT       -= gui

TARGET = qcoap
TEMPLATE = lib
DEFINES += MAKE_COAPLIB
CONFIG += c++11
DESTDIR = $$TOP_SRCDIR/bin

SOURCES += \
    coap.cpp \
    coapexchange.cpp \
    coapuri.cpp \
    coapendpoint.cpp \
    coapexchangeparameters.cpp \
    coaprequest.cpp \
    nodeswatcher.cpp \
    coapmessage.cpp \
    timerqueue.cpp

HEADERS += \
    coaplib_global.h \
    coap.h \
    coapexchange.h \
    coapexchange_p.h \
    coapuri.h \
    coapendpoint_p.h \
    coapendpoint.h \
    coapexchangeparameters.h \
    coapendpointinfo.h \
    coaprequest.h \
    coaprequest_p.h \
    nodeswatcher.h \
    coapmessage.h \
    timerqueue.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
