#ifndef COAPBRIDGE_H
#define COAPBRIDGE_H
#include "coapnode.h"

class CoapBridgePrivate;
class COAP_SHARED_EXPORT CoapBridge : public CoapNode
{
    Q_OBJECT
public:
    explicit CoapBridge(QObject *parent = 0);

    bool exportSignal(QObject *sender, const QString &signature, const QString &coapURL);

protected:
    CoapBridge(CoapBridgePrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(CoapBridge);
    Q_PRIVATE_SLOT(d_func(), void _q_signal_catched(QString,QVariantList));

    virtual void processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort);
    void sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort);
};

#endif // COAPBRIDGE_H
