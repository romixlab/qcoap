#include <QUdpSocket>
#include "coap.h"
#include "coapendpoint_p.h"
#include "coapexchange_p.h"
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
        qDebug() << "Processing incoming pdu:" << datagram.toHex();
        if (!pdu.isValid()) {
            qDebug() << "Invalid";
            return;
        }
        QByteArray token = pdu.token();
        CoapExchange *exchange = token2exchange.value(token, 0);
        if (exchange) {
            exchange->d->incoming_pdu(pdu);
        }
    }
}

void CoapEndpointPrivate::_q_error(QAbstractSocket::SocketError error)
{
    qDebug() << error;
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

void CoapEndpointPrivate::send_pdu(CoapExchange *exchange, CoapPDU *pdu)
{
    QByteArray token;
    if (exchange2token.contains(exchange)) {
        token = exchange2token[exchange];
    } else {
        token = generate_token();
        token2exchange.insert(token, exchange);
        exchange2token.insert(exchange, token);
    }

    pdu->setToken(token);
    // set message id only if we are making request
    if (    pdu->type() == Coap::Type::CONFIRMABLE ||
            pdu->type() == Coap::Type::NON_CONFIRMABLE)
        pdu->setMessageId(currentMessageId++);
    CoapUri uri = exchange->uri();
    udp->writeDatagram(pdu->pack(), uri.host(), uri.port());
}

void CoapEndpointPrivate::remove_exchange(CoapExchange *exchange)
{
    QMutableHashIterator<QByteArray, CoapExchange *> it(token2exchange);
    while (it.hasNext()) {
        it.next();
        if (it.value() == exchange) {
            it.remove();
            qWarning() << "CoapExchange was destroyed before it's completion, "
                          "use new and do not delete exchanges yourself";
        }
    }
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

bool CoapEndpoint::bind(const QHostAddress &address, quint16 port)
{
    Q_D(CoapEndpoint);
    d->address = address;
    d->port = port;
    return d->udp->bind(address, port);
}

#include "moc_coapendpoint.cpp" // intentionally, for private slots to work
