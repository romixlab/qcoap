#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include "coaplib_global.h"
#include "coappdu.h"
#include "coapuri.h"

class CoapEndpoint;
class CoapEndpointPrivate;
class CoapExchangePrivate;
class CoapTimerQueue;
class CoapPDU;
/** @file */
/**
 * @brief The CoapExchange class represents a logical conversation between CoAP client and server.
 * Logical means across different message id's and even tokens for block transfer.
 * You can inherit this class and reimplement handle() method for full controll over exchange.
 * @see Request
 * @see LambdaRequest
 * @see Observer
 */
class CoapExchange : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief CoapExchange through default endpoint
     */
    CoapExchange(QObject *parent = 0);
    virtual ~CoapExchange();
    /**
     * @brief setUri setts uri of a resource(-s) we are going to talk to.
     * @param uri contains host address, port, path and other data
     * @see CoapUri
     */
    void setUri(const CoapUri &uri);
    /**
     * @brief uri returns uri of a resource we are talking with.
     * @return
     */
    CoapUri uri() const;

//    /**
//     * @brief The Status enum
//     */
//    enum Status {
//        Invalid,    ///< After creation
//        InProgress, ///< Performing request, observing
//        Completed,  ///< Answer received, or abort() called
//        TimedOut    ///< Host not answered even after retransmissions
//    };
//    Q_ENUM(Status)
//    /**
//     * @brief status
//     * @return status of this exchange
//     */
//    Status status() const;
protected:
    /**
     * @brief handle is called when PDU, associated with this exchange arrives
     * @param pdu
     * Default implementation sends 4.04 Not Found error back and calls removeLater()
     */
    virtual void handle(const CoapPDU &pdu);
    /**
     * @brief send sends pdu to remote server or client
     * @param pdu
     */
    void send(CoapPDU &message);

    CoapExchange(CoapExchangePrivate &dd, QObject *parent = 0);
    CoapExchangePrivate *d_ptr;
private:
    Q_DECLARE_PRIVATE(CoapExchange)
};

#endif // COAPEXCHANGE_H
