#include "reliabilitylayer.h"

#include "midaddressportkey.h"

class ReliabilityLayerPrivate
{
    Q_DECLARE_PUBLIC(ReliabilityLayer)
public:
    ReliabilityLayerPrivate()
    { }

    ReliabilityLayer *q_ptr;
};

ReliabilityLayer::ReliabilityLayer(QObject *parent) :
    ILayer(parent), d_ptr(new ReliabilityLayerPrivate)
{
    Q_D(ReliabilityLayer);
    d->q_ptr = this;
}

ReliabilityLayer::~ReliabilityLayer()
{
    delete d_ptr;
}

void ReliabilityLayer::txRequest(CoapMessage &message)
{
    if (message.type() == CoapMessage::Type::Confirmable) {

    }
}

void ReliabilityLayer::txResponse(CoapMessage &response)
{

}

void ReliabilityLayer::txEmpty(CoapMessage &empty)
{

}

void ReliabilityLayer::rxRequest(CoapMessage &request)
{

}

void ReliabilityLayer::rxResponse(CoapMessage &response)
{

}

void ReliabilityLayer::rxEmpty(CoapMessage &empty)
{

}
