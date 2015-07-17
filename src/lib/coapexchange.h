#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include "coaplib_global.h"
#include <QExplicitlySharedDataPointer>

class CoapEndpoint;
class CoapExchangePrivate;
class CoapExchange
{
public:
    CoapExchange();
    CoapExchange(CoapEndpoint *throughEndpoint);
    CoapExchange(const CoapExchange &other);
    CoapExchange &operator =(const CoapExchange &other);

    /**
     * @brief get performs GET request
     */
    void get();

    enum Status {
        Invalid,
        Completed,
        TimedOut
    };

private:
    QExplicitlySharedDataPointer<CoapExchangePrivate> d;
};

#endif // COAPEXCHANGE_H
