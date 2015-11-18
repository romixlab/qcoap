#ifndef COAP_EXCHANGE_P_H
#define COAP_EXCHANGE_P_H

#include "coapexchange.h"
#include "coapuri.h"

#include <functional>
#include <QJSValue>

class CoapEndpoint;
class CoapExchangePrivate
{
    Q_DECLARE_PUBLIC(CoapExchange)
public:
    CoapExchangePrivate();
    virtual ~CoapExchangePrivate();

    void incoming_pdu(const CoapPDU &pdu);

    CoapEndpoint *endpoint;
    CoapUri uri;
    CoapExchange *q_ptr;

    CoapExchange::Status status;
    void setStatus(CoapExchange::Status status);

    QJSValue jsCompleted;
    QJSValue jsTimedout;

    bool isReady();
};

#endif // COAP_EXCHANGE_P_H

