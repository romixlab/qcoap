#include "coapexchange.h"
#include "coapexchange_p.h"
#include "coapendpoint_p.h"
#include "coap.h"

#include <QJSValue>

CoapExchangePrivate::CoapExchangePrivate() :
    status(CoapExchange::Ready), deleteAfterComplete(false), observe(false)
{
}

CoapExchangePrivate::~CoapExchangePrivate()
{

}

void CoapExchangePrivate::setStatus(CoapExchange::Status status)
{
    Q_Q(CoapExchange);
    this->status = status;
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
    if (!parent)
        setParent(d->endpoint);
}

CoapExchange::CoapExchange(CoapExchangePrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    Q_D(CoapExchange);
    d->q_ptr = this;
    d->endpoint = Coap::defaultEndpoint();
    if (!parent)
        setParent(d->endpoint);
}

CoapExchange::~CoapExchange()
{
    qDebug() << "Exchange to" << uriString() << "is destroyed";
    if (d_ptr)
        delete d_ptr;
    //Q_D(CoapExchange);
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
    //Q_D(const CoapExchange);
    return "CoapUri(TODO make normal output)"; /// TODO make normal output
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

    CoapMessage get;
    get.setCode(CoapMessage::Code::Get);
    get.setType(CoapMessage::Type::Confirmable);
    get.setUri(d->uri);
    send(get);
}

void CoapExchange::observe()
{
    Q_D(CoapExchange);
    if (!d->isReady())
        return;
    d->setStatus(InProgress);
    d->observe = true;

    CoapMessage get;
    get.setCode(CoapMessage::Code::Get);
    get.setType(CoapMessage::Type::Confirmable);
    get.addOption(CoapMessage::OptionType::Observe);
    get.setUri(d->uri);
    send(get);
}

void CoapExchange::cancel()
{
    Q_D(CoapExchange);
    d->endpoint->d_ptr->removeExchange(this);
    d->setStatus(Ready);
}

void CoapExchange::onCompleted(const QVariant &jsFunction)
{
    Q_D(CoapExchange);
    d->jsCompleted = jsFunction.value<QJSValue>();
}

void CoapExchange::onTimeout(const QVariant &jsFunction)
{
    Q_D(CoapExchange);
    d->jsTimeout = jsFunction.value<QJSValue>();
}

void CoapExchange::deleteAfterComplete()
{
    Q_D(CoapExchange);
    d->deleteAfterComplete = true;
}

void CoapExchange::handle(CoapMessage &message)
{
    Q_D(CoapExchange);
    if (message.code() == CoapMessage::Code::Content) {
        if (d->jsCompleted.isCallable())
            d->jsCompleted.call();
        //d->lambdaCompleted();
        if (!d->observe) {
            emit completed();
            d->setStatus(Completed);
        }
    } else {
        if (!d->observe) {
            emit completed();
            d->setStatus(Completed);
        }
    }

    if (d->status == Completed && d->deleteAfterComplete)
        deleteLater();
}

void CoapExchange::handleError()
{
    Q_D(CoapExchange);
    if (d->jsTimeout.isCallable())
        d->jsTimeout.call();
    emit timeout();
    d->setStatus(TimedOut);

    if (d->deleteAfterComplete)
        deleteLater();
}

void CoapExchange::send(CoapMessage &message)
{
    Q_D(CoapExchange);
    if (!d->endpoint) {
        qWarning() << "Can't send a message without CoapEndpoint, create it first";
        return;
    }
    if (message.isRequest())
        d->lastRequest = message;
    d->endpoint->d_ptr->tx(this, message);
}
