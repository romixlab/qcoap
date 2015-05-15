#include "coapnode.h"
#include <QUdpSocket>
#include "coap_pdu.h"

CoapNode::CoapNode(QObject *parent) : QObject(parent)
{
    udp = new QUdpSocket();
    udp->bind(QHostAddress::LocalHost, 5683);
}

CoapNode::~CoapNode()
{

}

void CoapNode::readPending()
{
//    while (udp->hasPendingDatagrams()) {
//        QByteArray datagram;
//        datagram.resize(udp->pendingDatagramSize());
//        QHostAddress sender;
//        quint16 port;
//        quint64 size = udp->readDatagram(datagram.data(), datagram.size(),
//                          &sender, &port);

//        CoapPDU pdu;
//        pdu.setVersion(1);
//        pdu.setV

//    }
}

