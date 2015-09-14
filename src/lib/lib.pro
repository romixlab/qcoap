QT       += network
QT       -= gui

TARGET = qcoap
TEMPLATE = lib
DEFINES += MAKE_COAPLIB
CONFIG += c++11
DESTDIR = $$TOP_SRCDIR/bin

SOURCES += \
    coap.cpp \
    coapendpoint.cpp \
    coappdu.cpp \
    coapexchange.cpp \
    coapendpointinfo.cpp \
    coapexchangeparameters.cpp \
    coapuri.cpp \
    coaptimerqueue.cpp

HEADERS += \
    coaplib_global.h \
    coap.h \
    coapendpoint_p.h \
    coapendpoint.h \
    coappdu.h \
    endianhelper.h \
    coapexchange.h \
    coapendpointinfo.h \
    coapexchangeparameters.h \
    coapuri.h \
    coapexchange_p.h \
    coaptimerqueue.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
