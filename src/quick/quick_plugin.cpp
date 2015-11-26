#include "quick_plugin.h"

#include "coapexchange.h"
#include "coapendpoint.h"

#include <qqml.h>

void QuickPlugin::registerTypes(const char *uri)
{
    // @uri CoAP
    Q_ASSERT(uri == QLatin1String("CoAP"));
    qmlRegisterType<CoapExchange>(uri, 1, 0, "CoapExchange");
    qmlRegisterType<CoapEndpoint>(uri, 1, 0, "CoapEndpoint");
}


