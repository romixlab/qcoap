#include <QUdpSocket>
#include "coap.h"
#include "coapendpoint.h"
#include "coapendpoint_p.h"
#include "coappdu.h"
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
        q->processPDU(pdu, from, fromPort);
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
    Q_D(CoapEndpoint);
    if (d) {
        delete d;
    }
}


bool CoapEndpoint::bind(const QHostAddress &address, quint16 port, Type type, quint16 maxAttempts)
{
    Q_D(CoapEndpoint);
    if (d->udp->state() != QUdpSocket::UnconnectedState) {
        qWarning() << "CoapEndpoint::bind(): socket already initialized";
        return false;
    }
    if (type == ClientServer)
        return d->udp->bind(address, port);
    for (quint16 i = 0; i < maxAttempts; ++i)
        if (d->udp->bind(address, port++))
            return true;
    return false;
}

void CoapEndpoint::processPDU(const CoapPDU &pdu, const QHostAddress &from,quint16 fromPort)
{
    Q_UNUSED(pdu);
    Q_UNUSED(from);
    Q_UNUSED(fromPort);
}

void CoapEndpoint::sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort)
{
    Q_D(CoapEndpoint);
    QByteArray packed = pdu.pack();
    qDebug() << "CoapEndpoint::sendPDU(): " << packed.toHex();
    d->udp->writeDatagram(packed, to, toPort);
}

#include "moc_coapendpoint.cpp" // intentionally, for private slots to work
