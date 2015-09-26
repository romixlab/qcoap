#include <QCoreApplication>

#include <coapendpoint.h>
#include <coaprequest.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CoapEndpoint *endpoint = new CoapEndpoint(CoapEndpoint::ClientServer);
    endpoint->bind(QHostAddress::Any, 5688);

    CoapRequest *req = new CoapRequest();
    CoapUri uri;
    uri.setHost(QHostAddress("127.0.0.1"));
    uri.setPort(5683);
    uri.setPath("test");
    req->setUri(uri);
    req->get();

    return a.exec();
}
