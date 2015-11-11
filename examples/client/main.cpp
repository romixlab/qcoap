#include <QCoreApplication>

#include <coapendpoint.h>
#include <coaprequest.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CoapEndpoint endpoint(CoapEndpoint::ClientServer);
    endpoint.bind(QHostAddress::LocalHost, 5684);
    CoapRequest req;
    CoapUri uri;
    uri.setHost(QHostAddress::LocalHost);
    uri.setPort(5683);
    req.setUri(uri);
    req.get();

    qDebug() << req.status();

    return a.exec();
}
