#include <QCoreApplication>
#include <QElapsedTimer>

#include "coap.h"
#include "coapexchange_p.h"
#include "coapendpoint_p.h"

CoapExchangePrivate::CoapExchangePrivate()
    : flags(Flags(0)), timeout(10000)
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
    if (    pdu.type() == Coap::Type::ACKNOWLEDGEMENT &&
            pdu.code() == Coap::Code::EMPTY) { // separate answer
        qDebug() << "Empty message, separate?";
        return;
    }

    pdus.append(pdu);

    if (pdu.type() == Coap::Type::CONFIRMABLE) {
        CoapPDU ack;
        ack.setType(Coap::Type::ACKNOWLEDGEMENT);
        ack.setCode(Coap::Code::EMPTY);
        ack.setToken(pdu.token());
        ack.setMessageId(pdu.messageId());
        endpoint->d_ptr->send_pdu(q, ack);
    }

    if (flags.testFlag(Observe)) {
        qDebug() << "Observed:" << pdu.payload();
    } else {
        status = CoapExchange::Completed;
        if (on_completed)
            on_completed();
    }
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
    if (d->status == InProgress) {
        qWarning() << "CoapExchange already in progress";
        return;
    }
    d->status = InProgress;

    CoapPDU pdu;
//    pdu->addOption(Coap::OptionType::URI_HOST, "coap.me");
    pdu.addOption(Coap::OptionType::URI_PATH, "hello-world");
    pdu.setCode(Coap::Code::GET);
    pdu.setType(Coap::Type::CONFIRMABLE);
    d->pdus.push_back(pdu);
    d->endpoint->d_ptr->send_pdu(this, pdu);
}

void CoapExchange::observe()
{
    if (d->status == InProgress) {
        qWarning() << "CoapExchange already in progress";
        return;
    }
    d->status = InProgress;
    d->flags |= CoapExchangePrivate::Observe;

    CoapPDU pdu;
    pdu.addOption(Coap::OptionType::URI_PATH, "hello-world");
    pdu.addOption(Coap::OptionType::OBSERVE);
    //pdu.addOption(Coap::OptionType::BLOCK2, QByteArray("\x02", 1));
    pdu.setCode(Coap::Code::GET);
    pdu.setType(Coap::Type::CONFIRMABLE);
    d->pdus.push_back(pdu);
    d->endpoint->d_ptr->send_pdu(this, pdu);
}

void CoapExchange::abort()
{
    d->status = Completed;
}

CoapExchange::Status CoapExchange::status() const
{
    return d->status;
}

QVector<CoapPDU> CoapExchange::conversation() const
{
    return d->pdus;
}

CoapPDU CoapExchange::lastPDU() const
{
    if (d->pdus.size() > 0)
        return d->pdus.back();
    return CoapPDU();
}

QByteArray CoapExchange::answer() const
{
    return lastPDU().payload();
}

void CoapExchange::onCompleted(std::function<void ()> lambda)
{
    d->on_completed = lambda;
}
