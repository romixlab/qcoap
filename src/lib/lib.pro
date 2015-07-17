QT       += network
QT       -= gui

TARGET = qcoap
TEMPLATE = lib
DEFINES += MAKE_COAPLIB
QMAKE_CXXFLAGS += -std=c++11
DESTDIR = $$PWD/../../lib

SOURCES += \
    coap.cpp \
    coapendpoint.cpp \
    coappdu.cpp \
    coapexchange.cpp \
    coapendpointinfo.cpp \
    coapexchangeparameters.cpp \
    coapuri.cpp

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
    coapexchange_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
