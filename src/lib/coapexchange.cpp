#include "coapexchange.h"
#include "coapexchange_p.h"
#include "coapendpoint_p.h"
#include "coap.h"

CoapExchangePrivate::CoapExchangePrivate()
{
}

CoapExchangePrivate::~CoapExchangePrivate()
{

}

CoapExchange::CoapExchange(QObject *parent) :
    QObject(parent), d_ptr(new CoapExchangePrivate)
{
    Q_D(CoapExchange);
    d->q_ptr = this;
    d->endpoint = Coap::defaultEndpoint();
}

CoapExchange::CoapExchange(CoapExchangePrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    Q_D(CoapExchange);
    d->q_ptr = this;
    d->endpoint = Coap::defaultEndpoint();
}

CoapExchange::~CoapExchange()
{
    Q_D(CoapExchange);
    d->endpoint->d_ptr->remove_exchange(this);
}

void CoapExchange::setUri(const CoapUri &uri)
{
    Q_D(CoapExchange);
    d->uri = uri;
}

CoapUri CoapExchange::uri() const
{
    Q_D(const CoapExchange);
    return d->uri;
}

void CoapExchange::handle(const CoapPDU &pdu)
{
    Q_UNUSED(pdu);
    CoapPDU rst;
    rst.setCode(CoapPDU::Code::NotFound);
    send(rst);
}

void CoapExchange::send(const CoapPDU &pdu)
{
    Q_D(CoapExchange);
    d->endpoint->d_ptr->send(this, pdu);
}
