#include "coapexchange.h"
#include "coapexchange_p.h"
#include "coapendpoint_p.h"
#include "coap.h"

#include <QJSValue>

CoapExchangePrivate::CoapExchangePrivate() :
    status(CoapExchange::Ready)
{
}

CoapExchangePrivate::~CoapExchangePrivate()
{

}

void CoapExchangePrivate::setStatus(CoapExchange::Status status)
{
    Q_Q(CoapExchange);
    status = status;
    emit q->statusChanged();
}

bool CoapExchangePrivate::isReady()
{
    if (status == CoapExchange::InProgress) {
        qWarning() << "CoapExchange: Cannot make new requests when status is `InProgress`";
        return false;
    } else {
        return true;
    }
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



//CoapExchange::CoapExchange(CoapEndpoint *throughEndpoint, QObject *parent) :
//    QObject(parent), d(new CoapExchangePrivate)
//{
//    d->q = this;
//    d->endpoint = throughEndpoint;
//    d->status = Invalid;
//}

CoapExchange::~CoapExchange()
{
    Q_D(CoapExchange);
//    d->endpoint->d_ptr->remove_exchange(this);
}

void CoapExchange::setUri(const CoapUri &uri)
{
    Q_D(CoapExchange);
    d->uri = uri;
    emit uriChanged();
}

CoapUri CoapExchange::uri() const
{
    Q_D(const CoapExchange);
    return d->uri;
}

void CoapExchange::setUriString(const QString &uriString)
{
    Q_D(CoapExchange);
    d->uri = CoapUri(uriString);
}

QString CoapExchange::uriString() const
{
    Q_D(const CoapExchange);
    return "CoapUri()"; /// TODO make normal output
}

CoapExchange::Status CoapExchange::status() const
{
    Q_D(const CoapExchange);
    return d->status;
}

void CoapExchange::get()
{
    Q_D(CoapExchange);
    if (!d->isReady())
        return;
    d->setStatus(InProgress);

    CoapPDU pdu;
    pdu.setCode(CoapPDU::Code::Get);
    pdu.setType(CoapPDU::Type::Confirmable);
    pdu.addOption(CoapPDU::OptionType::UriPath, "/");
    send(pdu);
}

void CoapExchange::onCompleted(const QVariant &jsFunction)
{
    Q_D(CoapExchange);
    d->jsCompleted = jsFunction.value<QJSValue>();
}

void CoapExchange::handle(const CoapPDU &pdu)
{
    Q_D(CoapExchange);
    if (pdu.code() == CoapPDU::Code::Content) {
        if (d->jsCompleted.isCallable())
            d->jsCompleted.call();
        //d->lambdaCompleted();
        emit completed();
        d->setStatus(Completed);
    } else {
        emit completed();
        d->setStatus(Completed);
    }
}

void CoapExchange::send(CoapPDU &message)
{
    Q_D(CoapExchange);
    d->endpoint->d_ptr->send(this, message);
}
