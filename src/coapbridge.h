#ifndef COAPBRIDGE_H
#define COAPBRIDGE_H
#include "coapnode.h"

class CoapBridgePrivate;
class CoapBridge : public CoapNode
{
    Q_OBJECT
public:
    explicit CoapBridge(QObject *parent = 0);

    virtual void processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort);
    void sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort);

protected:
    CoapBridge(CoapBridgePrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(CoapBridge);
};

#endif // COAPBRIDGE_H
