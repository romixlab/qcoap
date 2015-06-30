#ifndef COAPENDPOINT_H
#define COAPENDPOINT_H

#include <QObject>
#include <QHostAddress>
#include "coaplib_global.h"
#include "coapendpoint.h"
#include "coappdu.h"

class CoapEndpointPrivate;
class COAPLIB_SHARED_EXPORT CoapEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit CoapEndpoint(const QString &endpointName = QStringLiteral("default"), QObject *parent = 0);
    virtual ~CoapEndpoint();

    virtual void processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort);
    void sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort);

protected:
    CoapEndpointPrivate * d_ptr;
    CoapEndpoint(CoapEndpointPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(CoapEndpoint)
    Q_PRIVATE_SLOT(d_func(), void _q_state_changed(QAbstractSocket::SocketState))
    Q_PRIVATE_SLOT(d_func(), void _q_ready_read())
    Q_PRIVATE_SLOT(d_func(), void _q_error(QAbstractSocket::SocketError))
};

#endif // COAPENDPOINT_H
