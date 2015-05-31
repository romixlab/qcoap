#ifndef COAPBRIDGEPRIVATE_H
#define COAPBRIDGEPRIVATE_H
#include "coapnode.h"
#include "coapnode_p.h"

class DQObject;
class CoapBridgePrivate: public CoapNodePrivate
{
    Q_DECLARE_PUBLIC(CoapBridge);
public:
    CoapBridgePrivate();
    virtual ~CoapBridgePrivate();

    void setup();

    void _q_signal_catched(const QString &name, const QVariantList &arguments);
    
    DQObject *dq;
};

#endif // COAPBRIDGEPRIVATE_H
