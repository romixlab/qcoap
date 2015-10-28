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
    classifierLayer = new ClassifierLayer(q);
    udpLayer = new UDPLayer(0, q);
    classifierLayer->setLowerLayer(udpLayer);
    udpLayer->setUpperLayer(classifierLayer);
}

void CoapEndpointPrivate::send(CoapExchange *exchange, CoapPDU &message)
{
    classifierLayer->tx(exchange, message);
}

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
