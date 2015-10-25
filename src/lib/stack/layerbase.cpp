#include "layerbase.h"

LayerBase::LayerBase(QObject *parent) :
    QObject(parent), m_upper(0), m_lower(0)
{ }

void LayerBase::tx(CoapExchange *exchange, CoapPDU &message)
{
    if (m_lower)
        m_lower->tx(exchange, message);
}

void LayerBase::rx(CoapExchange *exchange, CoapPDU &message)
{
    if (m_upper)
        m_upper->rx(exchange, message);
}

void LayerBase::setUpperLayer(LayerBase *upper)
{
    m_upper = upper;
}

void LayerBase::setLowerLayer(LayerBase *lower)
{
    m_lower = lower;
}
