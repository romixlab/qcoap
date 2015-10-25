#include <QUdpSocket>
#include <QTimer>
#include <QDebug>

#include "coap.h"
#include "coapendpoint_p.h"
#include "coapexchange_p.h"
#include "coappdu.h"

CoapEndpointPrivate::CoapEndpointPrivate()
{   

}
CoapEndpointPrivate::~CoapEndpointPrivate() {}

void CoapEndpointPrivate::setup()
{
    Q_Q(CoapEndpoint);
    Coap::addEndpoint(q);
    udpLayer = new UDPLayer(0, q);
}

void CoapEndpointPrivate::send(CoapExchange *exchange, CoapPDU &message)
{
    udpLayer->tx(exchange, message);
}

//QByteArray CoapEndpointPrivate::generate_token()
//{
//    QByteArray token;
//    /// @todo token size from config
//    token.resize(2);
//    do {
//        quint8 *d = (quint8 *)token.data();
//        for (int i = 0; i < token.size(); ++i)
//            d[i] = rand() % 255;
//    } while (token2exchange.contains(token));
//    return token;
//}

CoapEndpoint::CoapEndpoint(Type endpointType, const QString &endpointName, QObject *parent) :
    QObject(parent), d_ptr(new CoapEndpointPrivate)
{
    Q_D(CoapEndpoint);
    d->q_ptr = this;
    d->setup();
}

CoapEndpoint::CoapEndpoint(CoapEndpointPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    Q_D(CoapEndpoint);
    d->q_ptr = this;
    d->setup();
}

CoapEndpoint::~CoapEndpoint()
{
    qDebug() << "Endpoint deleted";
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

bool CoapEndpoint::bind(const QHostAddress &address, quint16 port)
{
    Q_D(CoapEndpoint);
    d->udpLayer->bind(address, port);
}
