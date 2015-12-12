#include "quick_plugin.h"

#include "coapexchange.h"
#include "coapendpoint.h"
#include "exchangesingleton.h"

#include <qqml.h>

void QuickPlugin::registerTypes(const char *uri)
{
    // @uri CoAP
    Q_ASSERT(uri == QLatin1String("Coap"));
    qmlRegisterType<CoapExchange>(uri, 1, 0, "CoapExchange");
    qmlRegisterType<CoapEndpoint>(uri, 1, 0, "CoapEndpoint");
    qmlRegisterSingletonType<ExchangeSingleton>(uri, 1, 0, "Exchange", &ExchangeSingleton::qmlInstance);
}


