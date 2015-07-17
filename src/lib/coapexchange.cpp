#include "coap.h"
#include "coapexchange.h"
#include "coapendpoint.h"
#include "coapendpoint_p.h"

class CoapExchangePrivate : public QSharedData
{
public:
    CoapExchangePrivate()
    {

    }

    CoapExchangePrivate(const CoapExchangePrivate &other) :
        QSharedData(other)
    {
    }

    ~CoapExchangePrivate()
    {

    }

    CoapEndpoint *endpoint;
    CoapExchange::Status status;
};

CoapExchange::CoapExchange() :
    d(new CoapExchangePrivate)
{
    d->endpoint = Coap::defaultEndpoint();
    d->status = Invalid;
    d->endpoint->addExchange(*this);
    d->endpoint->d_ptr->test();
}

CoapExchange::CoapExchange(CoapEndpoint *throughEndpoint) :
    d(new CoapExchangePrivate)
{
    d->endpoint = throughEndpoint;
    d->status = Invalid;
}

CoapExchange::CoapExchange(const CoapExchange &other) :
    d(other.d)
{
}

CoapExchange &CoapExchange::operator =(const CoapExchange &other)
{
    d = other.d;
    return *this;
}

