#include <QList>
#include <QDebug>

#include "coap.h"

struct CoapPrivate {
    CoapPrivate() {}
    ~CoapPrivate() {}
    QList<CoapEndpoint *> endpoints;
};

Q_GLOBAL_STATIC(CoapPrivate, coap_private)

Coap::Coap()
{
}

CoapEndpoint *Coap::defaultEndpoint()
{
    CoapPrivate *d = coap_private;
    if (!d || d->endpoints.isEmpty()) {
        qWarning() << "Coap::defaultEndpoint(): no default endpoint";
        return 0;
    }
    return d->endpoints.at(0);
}

void Coap::addEndpoint(CoapEndpoint *endpoint)
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::addEndpoint(): failed to add endpoint";
        return;
    }
    d->endpoints.append(endpoint);
}

void Coap::removeEndpoint(CoapEndpoint *endpoint)
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::removeEndpoint(): failed to remove endpoint";
        return;
    }
    d->endpoints.removeOne(endpoint);
}

QList<CoapEndpoint *> Coap::endpoints()
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::endpoints(): there is no endpoints";
        return QList<CoapEndpoint *>();
    }
    return d->endpoints;
}
