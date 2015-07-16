#include <QUdpSocket>
#include "coap.h"
#include "coapendpoint.h"
#include "coapendpoint_p.h"
#include "coappdu.h"
#include <QTimer>
#include <QDebug>

CoapEndpointPrivate::CoapEndpointPrivate()
{   
    qDebug() << "CoapEndpointPrivate::CoapEndpointPrivate()";
}
CoapEndpointPrivate::~CoapEndpointPrivate() {}

void CoapEndpointPrivate::setup()
{
    qDebug() << "CoapEndpointPrivate::setup()";
    Q_Q(CoapEndpoint);
    udp = new QUdpSocket(q);
    QObject::connect(udp, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                     q,   SLOT(_q_state_changed(QAbstractSocket::SocketState)));
    QObject::connect(udp, SIGNAL(readyRead()),
                     q,   SLOT(_q_ready_read()));
    QObject::connect(udp, SIGNAL(error(QAbstractSocket::SocketError)),
                     q,   SLOT(_q_error(QAbstractSocket::SocketError)));

    Coap::addEndpoint(q);
}

void CoapEndpointPrivate::_q_state_changed(QAbstractSocket::SocketState state)
{
    qDebug() << "new state:" << state;
}

void CoapEndpointPrivate::_q_ready_read()
{
    Q_Q(CoapEndpoint);
    while (udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress from;
        quint16 fromPort;
        udp->readDatagram(datagram.data(), datagram.size(),
                          &from, &fromPort);
        CoapPDU pdu;
        pdu.unpack(datagram);
        qDebug() << "_q_ready_read():" << datagram.toHex();
        //q->processPDU(pdu, from, fromPort);
    }
}

void CoapEndpointPrivate::_q_error(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}


CoapEndpoint::CoapEndpoint(Type endpointType, const QString &endpointName, QObject *parent) :
    QObject(parent), d_ptr(new CoapEndpointPrivate)
{
    qDebug() << "CoapEndpoint::CoapEndpoint(QObject *parent)";
    Q_D(CoapEndpoint);
    d->q_ptr = this;
    d->name = endpointName;
    d->setup();
}

CoapEndpoint::CoapEndpoint(CoapEndpointPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    qDebug() << "CoapEndpoint::CoapEndpoint(CoapEndpointPrivate &dd, QObject *parent)";
    Q_D(CoapEndpoint);
    d->q_ptr = this;
    d->setup();
}

CoapEndpoint::~CoapEndpoint()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

void CoapEndpoint::addExchange(CoapExchange &exchange)
{

}

#include "moc_coapendpoint.cpp" // intentionally, for private slots to work
