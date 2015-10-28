#include "udplayer.h"
#include "coapexchange.h"

#include <QUdpSocket>

class UDPLayerPrivate
{
public:
    QUdpSocket *socket;
};

UDPLayer::UDPLayer(ILayer *upper, QObject *parent) :
    ILayer(parent), d_ptr(new UDPLayerPrivate)
{
    setUpperLayer(upper);
    Q_D(UDPLayer);
    d->socket = new QUdpSocket(parent);
    connect(d->socket, SIGNAL(readyRead()),
            this,      SLOT(onReadyRead()));
}

void UDPLayer::tx(CoapExchange *exchange, CoapPDU &message)
{
    Q_D(UDPLayer);
    QByteArray packed = message.pack();
    qDebug() << "Sending datagram to" << exchange->uri().host() << exchange->uri().port();
    d->socket->writeDatagram(packed, exchange->uri().host(), exchange->uri().port());
}

void UDPLayer::bind(const QHostAddress &address, quint16 port)
{
    Q_D(UDPLayer);
    if (!d->socket->bind(address, port)) {
        qWarning() << "Failed to bind" << d->socket->errorString();
    }
    //    Q_D(CoapEndpoint);
    //    d->address = address;
    //    if (d->type == Client) {
    //        d->port = 42400;
    //        while (!d->udp->bind(address, d->port))
    //            d->port++;
    //        return true;
    //    } else {
    //        if (port != 0)
    //            d->port = port;
    //        else
    //            d->port = 5683;
    //    }
    //    return d->udp->bind(address, d->port);
}

void UDPLayer::onReadyRead()
{
    Q_D(UDPLayer);
    while (d->socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(d->socket->pendingDatagramSize());
        QHostAddress from;
        quint16 fromPort;
        d->socket->readDatagram(datagram.data(), datagram.size(),
                          &from, &fromPort);
        CoapPDU pdu;
        pdu.unpack(datagram);
        qDebug() << "Processing incoming pdu from:" << from.toString() << pdu;
        if (pdu.isValid())
            rx(0, pdu); // pass to upper level
    }
}

