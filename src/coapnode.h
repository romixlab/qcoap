#ifndef COAPNODE_H
#define COAPNODE_H

#include <QObject>
#include <QHostAddress>
#include "coap_global.h"
#include "coappdu.h"

class CoapNodePrivate;
class COAP_SHARED_EXPORT CoapNode : public QObject
{
    Q_OBJECT
public:
    explicit CoapNode(QObject *parent = 0);
    virtual ~CoapNode();

    virtual void processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort);
    void sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort);

protected:
    CoapNodePrivate * d_ptr;
    CoapNode(CoapNodePrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(CoapNode)
    Q_PRIVATE_SLOT(d_func(), void _q_state_changed(QAbstractSocket::SocketState))
    Q_PRIVATE_SLOT(d_func(), void _q_ready_read())
    Q_PRIVATE_SLOT(d_func(), void _q_error(QAbstractSocket::SocketError))
};

#endif // COAPNODE_H
