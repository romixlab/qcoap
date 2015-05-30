#ifndef COAPBRIDGEPRIVATE_H
#define COAPBRIDGEPRIVATE_H
#include "coapnode.h"
#include "coapnode_p.h"

class CoapBridgePrivate: public CoapNodePrivate
{
    Q_DECLARE_PUBLIC(CoapBridge);
public:
    CoapBridgePrivate();
    virtual ~CoapBridgePrivate();
    
};

#endif // COAPBRIDGEPRIVATE_H
