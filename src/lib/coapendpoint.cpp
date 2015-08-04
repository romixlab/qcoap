#include <QUdpSocket>
#include <QTimer>
#include <QDebug>

#include "coap.h"
#include "coapendpoint_p.h"
#include "coapexchange_p.h"
#include "coappdu.h"

CoapEndpointPrivate::CoapEndpointPrivate()
{   
    currentMessageId = 0;
}
CoapEndpointPrivate::~CoapEndpointPrivate() {}

void CoapEndpointPrivate::setup()
{
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
    //Q_Q(CoapEndpoint);
    while (udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress from;
        quint16 fromPort;
        udp->readDatagram(datagram.data(), datagram.size(),
                          &from, &fromPort);
        CoapPDU pdu;
        pdu.unpack(datagram);
        qDebug() << "Processing incoming pdu:" << datagram.toHex();
        if (!pdu.isValid()) {
            qDebug() << "Invalid";
            return;
        }
        QByteArray token = pdu.token();
        CoapExchange *exchange = token2exchange.value(token, 0);
        if (!exchange) {
            exchange = id2exchange.value(pdu.messageId(), 0);
            if (exchange) {
                qDebug() << "Found by id";
                // id is changing from request to request, no need for this entry
                id2exchange.remove(pdu.messageId());
            } else {
                qDebug() << "Strange message, RST it";
                CoapPDU rst;
                rst.setType(Coap::Type::RESET);
                rst.setCode(Coap::Code::EMPTY);
                rst.setMessageId(pdu.messageId());
                udp->writeDatagram(rst.pack(), from, fromPort);
                return;
            }
        }
        exchange->d->incoming_pdu(pdu);
    }
}

void CoapEndpointPrivate::_q_error(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error" << error;
}

QByteArray CoapEndpointPrivate::generate_token()
{
    QByteArray token;
    /// @todo token size from config
    token.resize(2);
    do {
        quint8 *d = (quint8 *)token.data();
        for (int i = 0; i < token.size(); ++i)
            d[i] = rand() % 255;
    } while (token2exchange.contains(token));
    return token;
}

void CoapEndpointPrivate::send_pdu(CoapExchange *exchange, const CoapPDU &pdu)
{
    QByteArray token;
    if (exchange2token.contains(exchange)) {
        token = exchange2token[exchange];
    } else {
        token = generate_token();
        token2exchange.insert(token, exchange);
        exchange2token.insert(exchange, token);
    }

    CoapPDU pducopy = pdu;
    pducopy.setToken(token);
    // set message id only if we are making request
    if (    pducopy.type() == Coap::Type::CONFIRMABLE ||
            pducopy.type() == Coap::Type::NON_CONFIRMABLE) {
        quint16 id = currentMessageId++;
        pducopy.setMessageId(id);
        id2exchange.insert(id, exchange);
    }
    CoapUri uri = exchange->uri();
    udp->writeDatagram(pducopy.pack(), uri.host(), uri.port());
}

void CoapEndpointPrivate::remove_exchange(CoapExchange *exchange)
{
    qDebug() << "Deleting exchange from endpoint";
    if (exchange->status() != CoapExchange::Completed)
        qWarning() << "CoapExchange was destroyed before it's completion";
    QMutableHashIterator<QByteArray, CoapExchange *> it(token2exchange);
    while (it.hasNext()) {
        it.next();
        if (it.value() == exchange)
            it.remove();
    }
    exchange2token.remove(exchange);
    QMutableHashIterator<quint16, CoapExchange *> it2(id2exchange);
    while (it.hasNext()) {
        it.next();
        if (it.value() == exchange)
            it.remove();
    }
}

CoapEndpoint::CoapEndpoint(Type endpointType, const QString &endpointName, QObject *parent) :
    QObject(parent), d_ptr(new CoapEndpointPrivate)
{
    Q_D(CoapEndpoint);
    d->q_ptr = this;
    d->name = endpointName;
    d->type = endpointType;
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
    d->address = address;
    if (d->type == Client) {
        d->port = 42400;
        while (!d->udp->bind(address, d->port))
            d->port++;
        return true;
    } else {
        if (port != 0)
            d->port = port;
        else
            d->port = 5683;
    }
    return d->udp->bind(address, d->port);
}

#include "moc_coapendpoint.cpp" // intentionally, for private slots to work
