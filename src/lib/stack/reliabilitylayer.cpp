#include "reliabilitylayer.h"

#include "midaddressportkey.h"

class ReliabilityLayerPrivate
{
    Q_DECLARE_PUBLIC(ReliabilityLayer)
public:
    ReliabilityLayerPrivate(ReliabilityLayer *q) :
        q_ptr(q)
    { }

    ReliabilityLayer *q_ptr;
};

ReliabilityLayer::ReliabilityLayer(QObject *parent) :
    ILayer(parent), d_ptr(new ReliabilityLayerPrivate)
{

}

void ReliabilityLayer::txRequest(CoapExchange *exchange, CoapPDU &message)
{
    if (message.type() == CoapPDU::Type::Confirmable) {

    }
}
