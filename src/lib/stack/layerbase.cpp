#include "ilayer.h"

ILayer::ILayer(QObject *parent) :
    QObject(parent), m_upper(0), m_lower(0)
{ }

void ILayer::tx(CoapExchange *exchange, CoapPDU &message)
{
    if (m_lower)
        m_lower->tx(exchange, message);
}

void ILayer::rx(CoapExchange *exchange, CoapPDU &message)
{
    if (m_upper)
        m_upper->rx(exchange, message);
}

void ILayer::setUpperLayer(ILayer *upper)
{
    m_upper = upper;
}

void ILayer::setLowerLayer(ILayer *lower)
{
    m_lower = lower;
}
