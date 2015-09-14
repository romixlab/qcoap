#ifndef COAP_EXCHANGE_P_H
#define COAP_EXCHANGE_P_H

#include <functional>

#include "coapexchange.h"
#include "coapuri.h"
#include "coappdu.h"

class CoapEndpoint;
class CoapExchangePrivate
{
public:
    CoapExchangePrivate();
    ~CoapExchangePrivate();

    void incoming_pdu(const CoapPDU &pdu);

    CoapExchange *q;
    CoapEndpoint *endpoint;
    CoapExchange::Status status;
    CoapUri uri;
    QVector<CoapPDU> pdus;
    quint8 retransmitCount = 0;

    enum Flag {
        Observe     = 1,
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    Flags flags;

    std::function<void ()> on_completed;
    std::function<void ()> on_error;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CoapExchangePrivate::Flags)

#endif // COAP_EXCHANGE_P_H

