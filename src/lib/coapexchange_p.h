#ifndef COAP_EXCHANGE_P_H
#define COAP_EXCHANGE_P_H

#include <QSharedData>
#include <functional>

#include "coapexchange.h"
#include "coapuri.h"

class CoapEndpoint;
class CoapPDU;
class CoapExchangePrivate : public QSharedData
{
public:
    CoapExchangePrivate();
    CoapExchangePrivate(const CoapExchangePrivate &other);
    ~CoapExchangePrivate();

    void incoming_pdu(const CoapPDU &pdu);

    CoapExchange *q;
    CoapEndpoint *endpoint;
    CoapExchange::Status status;
    CoapUri uri;
    QVector<CoapPDU *> pdus;
    std::function<void ()> on_completed;
};

#endif // COAP_EXCHANGE_P_H

