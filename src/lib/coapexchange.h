#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include <functional>

#include <QExplicitlySharedDataPointer>

#include "coaplib_global.h"
#include "coapuri.h"

class CoapEndpoint;
class CoapEndpointPrivate;
class CoapExchangePrivate;
class CoapTimerQueue;
class CoapPDU;
/** @file */
/**
 * @brief The CoapExchange class
 */
class CoapExchange
{
public:
    CoapExchange();
    CoapExchange(CoapEndpoint *throughEndpoint);

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

    void observe();
    void abort();

    /**
     * @brief The Status enum
     */
    enum Status {
        Invalid,    ///< After creation
        InProgress, ///< Performing request, observing
        Completed,  ///< Answer received, or abort() called
        TimedOut    ///< Host not answered even after retransmissions
    };
    /**
     * @brief status
     * @return status of this exchange
     */
    Status status() const;
    /**
     * @brief conversation returns all important PDU's
     * @return
     * For GET [ack pdu, answer pdu] or [ack pdu] if exchange is timed out
     */
    QVector<CoapPDU> conversation() const;
    /**
     * @brief lastPDU returns last PDU
     * @return
     * For GET reqeust this will be PDU with answer
     */
    CoapPDU lastPDU() const;
    /**
     * @brief answer returns last PDU payload
     * @return lastPDU().payload()
     */
    QByteArray answer() const;
    /**
     * @brief onCompleted provides a way to monitor exchange status.
     * @param lambda called when status is changed to Completed
     */
    void onCompleted(std::function<void ()> lambda);

    friend class CoapEndpoint;
    friend class CoapEndpointPrivate;
    friend class CoapTimerQueue;
private:
    CoapExchange(const CoapExchange &other);
    CoapExchange &operator =(const CoapExchange &other);
    QExplicitlySharedDataPointer<CoapExchangePrivate> d;
};

#endif // COAPEXCHANGE_H
