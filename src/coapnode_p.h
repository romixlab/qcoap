#ifndef COAPNODEPRIVATE_H
#define COAPNODEPRIVATE_H
#include <QObject>
#include <QHostAddress>
#include "coapnode.h"

class QUdpSocket;

class CoapNodePrivate
{
    Q_DECLARE_PUBLIC(CoapNode)
public:
    CoapNodePrivate();
    virtual ~CoapNodePrivate();

    void setup();
    void _q_state_changed(QAbstractSocket::SocketState state);
    void _q_ready_read();
    void _q_error(QAbstractSocket::SocketError error);

    QHostAddress address;
    quint16 port;
    QUdpSocket *udp;

    CoapNode * q_ptr;
};
#endif // COAPNODEPRIVATE_H
