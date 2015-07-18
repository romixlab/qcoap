#ifndef COAPENDPOINT_P_H
#define COAPENDPOINT_P_H

#include <QObject>
#include <QHostAddress>
#include "coapendpoint.h"

class QUdpSocket;
class CoapExchange;
class CoapEndpointPrivate
{
    Q_DECLARE_PUBLIC(CoapEndpoint)
public:
    CoapEndpointPrivate();
    virtual ~CoapEndpointPrivate();

    void setup();
    void _q_state_changed(QAbstractSocket::SocketState state);
    void _q_ready_read();
    void _q_error(QAbstractSocket::SocketError error);

    QByteArray generate_token();
    void send_pdu(CoapExchange *exchange, const CoapPDU &pdu);
    /**
     * @brief remove_exchange removes exchange from internal data structures
     * @param exchange
     * Called from ~CoapExchange().
     * @todo Exchange inspection, smth like valgrind for exchanges
     */
    void remove_exchange(CoapExchange *exchange);

    QHostAddress address;
    quint16 port;
    QUdpSocket *udp;
    QString name;
    CoapEndpoint *q_ptr;
    quint16 currentMessageId;
    CoapEndpoint::Type type;

    QHash<QByteArray, CoapExchange *> token2exchange; // on pdu rx used to find exchange
    QHash<CoapExchange *, QByteArray> exchange2token; // on pdu tx used to find token
    QHash<quint16, CoapExchange *> id2exchange; // for separate answer without token


};
#endif // COAPENDPOINT_P_H
