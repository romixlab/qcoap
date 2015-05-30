#include <QUdpSocket>
#include "coapnode.h"
#include "coapnode_p.h"
#include "coappdu.h"
#include <QDebug>

CoapNodePrivate::CoapNodePrivate()
{   
    qDebug() << "CoapNodePrivate::CoapNodePrivate()";
}
CoapNodePrivate::~CoapNodePrivate() {}

void CoapNodePrivate::setup()
{
    qDebug() << "CoapNodePrivate::setup()";
    Q_Q(CoapNode);
    udp = new QUdpSocket(q);
    if (!udp->bind(QHostAddress::LocalHost, 5683))
        qWarning() << "Bind failed" << udp->errorString();
    QObject::connect(udp, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            q, SLOT(_q_state_changed(QAbstractSocket::SocketState)));
    QObject::connect(udp, SIGNAL(readyRead()),
            q, SLOT(_q_ready_read()));
    QObject::connect(udp, SIGNAL(error(QAbstractSocket::SocketError)),
            q, SLOT(_q_error(QAbstractSocket::SocketError)));
}

void CoapNodePrivate::_q_state_changed(QAbstractSocket::SocketState state)
{
    qDebug() << "new state:" << state;
    // if (state == QTcpSocket::ConnectedState) {
    //     write_sentence(QStringList() << "/login");
    //     this->state = Router::LOGGING_IN;
    //     Q_Q(Router);
    //     emit q->stateChanged(this->state);
    // }
}

void CoapNodePrivate::_q_ready_read()
{
    Q_Q(CoapNode);
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

void CoapNodePrivate::_q_error(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    // Q_Q(Router);
    // state = Router::DISCONNECTED;
    // emit q->stateChanged(state);
}


CoapNode::CoapNode(QObject *parent) :
    QObject(parent), d_ptr(new CoapNodePrivate)
{
    qDebug() << "CoapNode::CoapNode(QObject *parent)";
    Q_D(CoapNode);
    d->q_ptr = this;
    d->setup();
}

CoapNode::CoapNode(CoapNodePrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    qDebug() << "CoapNode::CoapNode(CoapNodePrivate &dd, QObject *parent)";
    Q_D(CoapNode);
    d->q_ptr = this;
    d->setup();
}

CoapNode::~CoapNode()
{
    Q_D(CoapNode);
    if (d) {
        delete d;
    }
}

void CoapNode::processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort)
{
    Q_UNUSED(pdu);
    Q_UNUSED(from);
    Q_UNUSED(fromPort);
}

void CoapNode::sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort)
{
    Q_D(CoapNode);
    QByteArray packed = pdu.pack();
    qDebug() << "CoapNode::sendPDU(): " << packed.toHex();
    d->udp->writeDatagram(packed, to, toPort);
}

#include "moc_coapnode.cpp" // intentionally
