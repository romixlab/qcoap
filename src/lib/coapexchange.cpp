#include "coap.h"
#include "coapexchange_p.h"
#include "coapendpoint_p.h"

CoapExchangePrivate::CoapExchangePrivate()
{

}

CoapExchangePrivate::CoapExchangePrivate(const CoapExchangePrivate &other) :
    QSharedData(other)
{
}

CoapExchangePrivate::~CoapExchangePrivate()
{

}

void CoapExchangePrivate::incoming_pdu(const CoapPDU &pdu)
{
    if (pdu.code() == Coap::Code::EMPTY) // separate answer
        return;



    CoapPDU ack;
    ack.setType(Coap::Type::ACKNOWLEDGEMENT);
    ack.setCode(Coap::Code::EMPTY);
    ack.setToken(pdu.token());
    ack.setMessageId(pdu.messageId());
    endpoint->d_ptr->send_pdu(q, &ack);

    qDebug() << "CoapExchangePrivate::incoming_pdu()" << pdu.payload();
    if (on_completed)
        on_completed();
}

CoapExchange::CoapExchange() :
    d(new CoapExchangePrivate)
{
    d->q = this;
    d->endpoint = Coap::defaultEndpoint();
    d->status = Invalid;
}

CoapExchange::CoapExchange(CoapEndpoint *throughEndpoint) :
    d(new CoapExchangePrivate)
{
    d->q = this;
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

CoapExchange::~CoapExchange()
{
    d->endpoint->d_ptr->remove_exchange(this);
}

void CoapExchange::setUri(const CoapUri &uri)
{
    d->uri = uri;
}

CoapUri CoapExchange::uri() const
{
    return d->uri;
}

void CoapExchange::get()
{
    if (d->status == InProgress)
        return;
    d->status = InProgress;

    CoapPDU *pdu = new CoapPDU;
    pdu->addOption(Coap::OptionType::URI_HOST, "coap.me");
    pdu->addOption(Coap::OptionType::URI_PATH, "separate");
    pdu->setCode(Coap::Code::GET);
    pdu->setType(Coap::Type::CONFIRMABLE);
    d->pdus.push_back(pdu);
    d->endpoint->d_ptr->send_pdu(this, pdu);
}

void CoapExchange::onCompleted(std::function<void ()> lambda)
{
    d->on_completed = lambda;
}
