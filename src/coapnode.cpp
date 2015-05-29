#include "coapnode.h"
#include <QUdpSocket>
#include "coappdu.h"
#include <QDebug>

CoapNode::CoapNode(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket();
    udp->bind(QHostAddress::LocalHost, 5683);
    connect(udp, SIGNAL(readyRead()), this, SLOT(readPending()));
}

CoapNode::~CoapNode()
{

}

void CoapNode::readPending()
{
    while (udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 port;
        udp->readDatagram(datagram.data(), datagram.size(),
                          &sender, &port);

        CoapPDU pdu;
        pdu.unpack(datagram);
        qDebug() << pdu.errors();
        qDebug() << pdu.version();
        qDebug() << (quint8)pdu.type();
        qDebug() << pdu.token();
        qDebug() << pdu.messageId();
       // qDebug() << pdu.options();
        qDebug() << pdu.payload();

    }
}

