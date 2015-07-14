#include "coap.h"
#include "coapexchange.h"
#include "coapendpoint.h"

class CoapExchangePrivate
{
public:
    CoapEndpoint *endpoint;
    QUrl uri;

};

CoapExchange::CoapExchange(QObject *parent) :
    QObject(parent), d_ptr(new CoapExchangePrivate)
{
    Q_D(CoapExchange);
    d->endpoint = Coap::defaultEndpoint();
}

CoapExchange::CoapExchange(CoapEndpoint *throughEndpoint, QObject *parent) :
    QObject(parent), d_ptr(new CoapExchangePrivate)
{
    Q_D(CoapExchange);
    d->endpoint = throughEndpoint;
}

CoapExchange::CoapExchange(const CoapExchange &other) :
    d_ptr(other.d_ptr)
{
}

CoapExchange::~CoapExchange()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

void CoapExchange::setUri(const QUrl &uri)
{
    Q_D(CoapExchange);
    d->uri = uri;
}

