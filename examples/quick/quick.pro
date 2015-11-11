TEMPLATE = app

QT += qml quick widgets

QMAKE_CXXFLAGS += -std=c++11
DESTDIR = $$TOP_SRCDIR/bin
LIBS += -L"$$TOP_SRCDIR/bin" -lqcoap
INCLUDEPATH += $$TOP_SRCDIR/src/lib

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
