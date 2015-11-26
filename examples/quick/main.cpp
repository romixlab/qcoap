#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "coapendpoint.h"
#include "coapexchange.h"
#include "nodeswatcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
