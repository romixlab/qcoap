#include "ilayer.h"

ILayer::ILayer(QObject *parent) :
    QObject(parent), m_upper(0), m_lower(0)
{ }

ILayer::~ILayer()
{ }

void ILayer::tx(CoapMessage &message)
{
    if (message.isRequest())
        txRequest(message);
    else if (message.isResponse())
        txResponse(message);
    else
        txEmpty(message);
}

void ILayer::txRequest(CoapMessage &request)
{
    if (m_lower)
        m_lower->tx(request);
}

void ILayer::txResponse(CoapMessage &response)
{
    if (m_lower)
        m_lower->tx(response);
}

void ILayer::txEmpty(CoapMessage &empty)
{
    if (m_lower)
        m_lower->tx(empty);
}

void ILayer::rx(CoapMessage &message)
{
    if (message.isRequest())
        rxRequest(message);
    else if (message.isResponse())
        rxResponse(message);
    else
        rxEmpty(message);
}

void ILayer::rxRequest(CoapMessage &request)
{
    if (m_upper)
        m_upper->rx(request);
}

void ILayer::rxResponse(CoapMessage &response)
{
    if (m_upper)
        m_upper->rx(response);
}

void ILayer::rxEmpty(CoapMessage &empty)
{
    if (m_upper)
        m_upper->rx(empty);
}

void ILayer::setUpperLayer(ILayer *upper)
{
    m_upper = upper;
}

void ILayer::setLowerLayer(ILayer *lower)
{
    m_lower = lower;
}
