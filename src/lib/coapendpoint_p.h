#ifndef COAPENDPOINT_P_H
#define COAPENDPOINT_P_H

#include "coapendpoint.h"
#include "stack/udplayer.h"
#include "stack/classifierlayer.h"

#include <QObject>
#include <QHostAddress>

class CoapExchange;
class CoapEndpointPrivate
{
    Q_DECLARE_PUBLIC(CoapEndpoint)
public:
    CoapEndpointPrivate();
    virtual ~CoapEndpointPrivate();

    void setup();

    void send(CoapMessage &message);

    CoapEndpoint *q_ptr;
    ClassifierLayer *classifierLayer;
    UDPLayer *udpLayer;
};
#endif // COAPENDPOINT_P_H
