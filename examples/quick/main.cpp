#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "coapendpoint.h"
#include "coapexchange.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CoapEndpoint endpoint(CoapEndpoint::ClientServer);
    endpoint.bind(QHostAddress::LocalHost, 5684);
    qmlRegisterType<CoapExchange>("CoAP", 1, 0, "CoapExchange");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
