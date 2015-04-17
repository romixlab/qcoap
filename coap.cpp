#include "coap_private.h"
#include "cantcoap.h"

#include <QDebug>

using namespace qcoap;

CoapPrivate::CoapPrivate(Coap *parent) :
    q_ptr(parent)
{
    pdu = new CoapPDU(buffer, 512);
}

void CoapPrivate::bind(const QHostAddress &address, quint16 port)
{
    Q_Q(Coap);
    udp = new QUdpSocket(q);
    ///@todo error check
    if (!udp->bind(address, port)) {
        qDebug() << udp->errorString();
    }
    connect(udp, &QUdpSocket::readyRead,
            this, &CoapPrivate::readPending);
}

void CoapPrivate::readPending()
{
    ///@todo size?
    char uriBuffer[32];
    while (udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 port;
        quint64 size = udp->readDatagram((char *)buffer, 512,
                          &sender, &port);
        pdu->setPDULength(size);
        qDebug() << "validate:" << pdu->validate();
        pdu->printHuman();

        int recvUriLen;
        pdu->getURI(uriBuffer, 32, &recvUriLen);
        QString uri = QString(QByteArray(uriBuffer, recvUriLen));
        if (uri.length() > 0) {
            CoapPDU *response = new CoapPDU();
            response->setVersion(1);
            response->setMessageID(pdu->getMessageID());
            response->setToken(pdu->getTokenPointer(), pdu->getTokenLength());
            response->setCode(CoapPDU::COAP_CONTENT);
            response->setContentFormat(CoapPDU::COAP_CONTENT_FORMAT_APP_LINK);
            response->setPayload((uint8_t *)"</sensors/temp>", 15);
            udp->writeDatagram(QByteArray((const char*)response->getPDUPointer(),
                                          response->getPDULength()), sender, port);
        }
    }

}


Coap::Coap(QObject *parent) :
    QObject(parent),
    d_ptr(new CoapPrivate(this))
{
    Q_D(Coap);
    d->bind(QHostAddress::LocalHost, 5683);
}

Coap::~Coap()
{
    qDebug() << "removing Private";
    delete d_ptr;
    d_ptr = 0;
}
