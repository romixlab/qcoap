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
    void send_pdu(CoapExchange *exchange, CoapPDU *pdu);
    void remove_exchange(CoapExchange *exchange); /// called from ~CoapExchange() only

    QHostAddress address;
    quint16 port;
    QUdpSocket *udp;
    QString name;
    CoapEndpoint *q_ptr;
    quint16 currentMessageId;

    QHash<QByteArray, CoapExchange *> token2exchange;
    QHash<CoapExchange *, QByteArray> exchange2token;


};
#endif // COAPENDPOINT_P_H
