#ifndef COAPEXCHANGEPARAMETERS_H
#define COAPEXCHANGEPARAMETERS_H

#include <QJsonObject>

#include "coaplib_global.h"

class COAPLIB_SHARED_EXPORT CoapExchangeParameters
{
public:
    /**
     * @brief CoapExchangeParameters Constructs object with default values
     * as described in RFC 7252 4.8:
     * +-------------------+---------------+
     * | name              | default value |
     * +-------------------+---------------+
     * | ACK_TIMEOUT       | 2 seconds     |
     * | ACK_RANDOM_FACTOR | 1.5           |
     * | MAX_RETRANSMIT    | 4             |
     * | NSTART            | 1             |
     * | DEFAULT_LEISURE   | 5 seconds     |
     * | PROBING_RATE      | 1 byte/second |
     * +-------------------+---------------+
     */
    CoapExchangeParameters();
    /**
     * @brief unpack unpacks data stored in json object
     * @param packed
     * Used by @see CoapProvision
     */
    void unpack(const QJsonObject &packed);
    /**
     * @brief pack packs all the data to json object
     * @return
     * Used by @see CoapProvision
     */
    QJsonObject pack() const;
    /**
     * @brief setAckTimeout
     * @param ackTimeout
     * Delay before next retransmission:
     * delay = ackTimeout() * 2 ^ i;
     * delay += norm(delay * jitter());
     * where i is is the retransmission number
     */
    void setAckTimeout(quint16 ackTimeout);
    /**
     * @brief ackTimeout
     * @return
     */
    quint16 ackTimeout() const;
    /**
     * @brief setJitter
     * @param jitter
     * @see ackTimeout()
     */
    void setJitter(double jitter);
    /**
     * @brief jitter
     * @return jitter value
     */
    double jitter() const;
    /**
     * @brief setMaxRetransmit setts retransmission events count before giving up on exchange
     * @param maxRetransmit
     */
    void setMaxRetransmit(quint8 maxRetransmit);
    /**
     * @brief maxRetransmit
     * @return retransmission events count before giving up on exchange
     */
    quint8 maxRetransmit() const;
    /**
     * @brief setNStart
     * @param NStart number of simultaneous outstanding interactions to some node
     */
    void setNStart(quint8 NStart);
    quint8 nSTart() const;
    void setProbindRate(quint32 probingRate);
    quint32 probingRate() const;
};

#endif // COAPEXCHANGEPARAMETERS_H
