#ifndef COAP_EXCHANGE_P_H
#define COAP_EXCHANGE_P_H

#include <QSharedData>
#include <functional>

#include "coapexchange.h"
#include "coapuri.h"
#include "coappdu.h"

class CoapEndpoint;
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
    QVector<CoapPDU> pdus;

    enum Flag {
        Observe     = 1,
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    Flags flags;

    quint32 timeout;
    std::function<void ()> on_completed;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CoapExchangePrivate::Flags)

#endif // COAP_EXCHANGE_P_H

