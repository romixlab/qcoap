#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include "coaplib_global.h"
#include "coapmessage.h"
#include "coapuri.h"

class CoapEndpoint;
class CoapEndpointPrivate;
class CoapExchangePrivate;
/** @file */
/**
 * @brief The CoapExchange class represents a logical conversation between CoAP client and server.
 * Logical means across different message id's and even tokens for block transfer.
 */
class CoapExchange : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString uri READ uriString WRITE setUriString NOTIFY uriChanged)
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
    void setUriString(const QString &uriString);
    QString uriString() const;

    /**
     * @brief The Status enum
     */
    enum Status {
        Ready,    ///< After creation
        InProgress, ///< Performing request, observing
        Completed,  ///< Answer received, or abort() called
        TimedOut    ///< Host not answered even after retransmissions
    };
    Q_ENUM(Status)
    /**
     * @brief status
     * @return status of this exchange
     */
    Status status() const;

    Q_INVOKABLE void get();
    Q_INVOKABLE void observe();
    Q_INVOKABLE void cancel();


    Q_INVOKABLE void onCompleted(const QVariant &jsFunction);
    Q_INVOKABLE void onTimeout(const QVariant &jsFunction);

    /**
     * @brief deleteAfterComplete controls lifetime of CoapExchange object.
     * Call this function to automatically remove exchange after answer war received or timeout occured.
     * Default value is false
     */
    void deleteAfterComplete();

signals:
    void statusChanged();
    void uriChanged();
    void completed();
    void timeout();
protected:
    /**
     * @brief handle is called when PDU, associated with this exchange arrives
     * @param pdu
     */
    virtual void handle(CoapMessage &message);
    virtual void handleError();
    /**
     * @brief send sends pdu to remote server or client
     * @param pdu
     */
    void send(CoapMessage &message);

    CoapExchange(CoapExchangePrivate &dd, QObject *parent = 0);
    CoapExchangePrivate *d_ptr;
private:
    Q_DECLARE_PRIVATE(CoapExchange)

    friend class CoapEndpoint;
    friend class CoapEndpointPrivate;
};

#endif // COAPEXCHANGE_H
