#ifndef COAPEXCHANGEPARAMETERS_H
#define COAPEXCHANGEPARAMETERS_H

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

    void setAckTimeout(quint16 ackTimeout);

    quint16 ackTimeout() const;

    void setRandomFactor(double randomFactor);

    double randomFactor() const;

    void setMaxRetransmit(quint8 maxRetransmit);

    quint8 maxRetransmit() const;

    void setNStart(quint8 NStart);

    quint8 nSTart() const;

    void setProbindRate(quint32 probingRate);

    quint32 probingRate() const;
};

#endif // COAPEXCHANGEPARAMETERS_H
