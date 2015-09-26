QT       += network
QT       -= gui

TARGET = qcoap
TEMPLATE = lib
DEFINES += MAKE_COAPLIB
QMAKE_CXXFLAGS += -std=c++11
DESTDIR = $$PWD/../../lib

SOURCES += \
    coappdu.cpp \
    coap.cpp \
    coapexchange.cpp \
    coapuri.cpp \
    coapendpoint.cpp \
    coapexchangeparameters.cpp \
    coaprequest.cpp

HEADERS += \
    coaplib_global.h \
    coappdu.h \
    coap.h \
    coapexchange.h \
    coapexchange_p.h \
    coapuri.h \
    coapendpoint_p.h \
    coapendpoint.h \
    coapexchangeparameters.h \
    coapendpointinfo.h \
    coaprequest.h \
    coaprequest_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
