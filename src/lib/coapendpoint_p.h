#ifndef COAPENDPOINT_P_H
#define COAPENDPOINT_P_H

#include <QObject>
#include <QHostAddress>
#include "coapendpoint.h"

class QUdpSocket;

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

    QHostAddress address;
    quint16 port;
    QUdpSocket *udp;

    QString name;

    CoapEndpoint *q_ptr;
};
#endif // COAPENDPOINT_P_H
