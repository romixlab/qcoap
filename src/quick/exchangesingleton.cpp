#include "exchangesingleton.h"

#include "coapexchange.h"

ExchangeSingleton::ExchangeSingleton(QObject *parent) : QObject(parent)
{

}

QObject *ExchangeSingleton::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new ExchangeSingleton;
}

CoapExchange *ExchangeSingleton::exchange() const
{
    CoapExchange *exchange = new CoapExchange;
    exchange->deleteAfterComplete();
    return exchange;
}

