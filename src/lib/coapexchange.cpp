#include "coap.h"
#include "coapexchange.h"
#include "coapendpoint.h"

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
};

CoapExchange::CoapExchange() :
    d(new CoapExchangePrivate)
{
    d->endpoint = Coap::defaultEndpoint();
    d->endpoint->addExchange(*this);
}

CoapExchange::CoapExchange(CoapEndpoint *throughEndpoint) :
    d(new CoapExchangePrivate)
{
    d->endpoint = throughEndpoint;
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

