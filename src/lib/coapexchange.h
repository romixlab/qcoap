#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include <QExplicitlySharedDataPointer>
#include <functional>

#include "coaplib_global.h"
#include "coapuri.h"

class CoapEndpoint;
class CoapEndpointPrivate;
class CoapExchangePrivate;
class CoapExchange
{
public:
    CoapExchange();
    CoapExchange(CoapEndpoint *throughEndpoint);
    CoapExchange(const CoapExchange &other);
    CoapExchange &operator =(const CoapExchange &other);
    ~CoapExchange();

    /**
     * @brief setUri setts uri of a resource(-s) we are going to talk to
     * @param uri contains host address, port, path and other data
     * @see CoapUri
     */
    void setUri(const CoapUri &uri);

    CoapUri uri() const;

    /**
     * @brief get performs GET request
     */
    void get();

    enum Status {
        Invalid,
        InProgress,
        Completed,
        TimedOut
    };

    void onCompleted(std::function<void ()> lambda);

    friend class CoapEndpoint;
    friend class CoapEndpointPrivate;
private:
    QExplicitlySharedDataPointer<CoapExchangePrivate> d;
};

#endif // COAPEXCHANGE_H
