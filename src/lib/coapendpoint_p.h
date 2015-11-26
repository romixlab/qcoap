#ifndef COAPENDPOINT_P_H
#define COAPENDPOINT_P_H

#include "coapendpoint.h"

#include <QObject>
#include <QUdpSocket>

class MidAddressPortKey
{
public:
    MidAddressPortKey(quint32 messageId,
                      const QHostAddress &address = QHostAddress(),
                      quint16 port = 0) :
        m_messageId(messageId), m_address(address), m_port(port)
    { }


private:
    quint32 m_messageId;
    QHostAddress m_address;
    quint16 m_port;

    friend bool operator==(const MidAddressPortKey &m1, const MidAddressPortKey &m2);
    friend inline uint qHash(const MidAddressPortKey &key, uint seed);
};

inline bool operator==(const MidAddressPortKey &m1, const MidAddressPortKey &m2)
{
    return (m1.m_messageId == m2.m_messageId) &&
           (m1.m_address   == m2.m_address) &&
           (m1.m_port      == m2.m_port);
}

inline uint qHash(const MidAddressPortKey &key, uint seed)
{
    return qHash(key.m_address, seed) ^ key.m_messageId ^ key.m_port;
}

class TimerQueue;
class CoapExchange;
class CoapEndpointPrivate
{
    Q_DECLARE_PUBLIC(CoapEndpoint)
public:
    CoapEndpointPrivate();
    virtual ~CoapEndpointPrivate();

    void setup();

    /**
     * @brief tx towards network, pass to lower layers
     * @param exchange exchange that message belongs to
     * @param message pdu + address
     */
    void tx(CoapExchange *fromExchange, CoapMessage &message);
    void txRequest(CoapExchange *fromExchange, CoapMessage &request);
    void txResponse(CoapExchange *fromExchange, CoapMessage &response);
    void txEmpty(CoapExchange *fromExchange, CoapMessage &empty);
    /**
     * @brief rx from network, pass to upper layers
     * @param exchange
     * @param message
     */
    void rx(CoapMessage &message);
    void rxRequest(CoapMessage &request);
    void rxResponse(CoapMessage &response);
    void rxEmpty(CoapMessage &empty);
    CoapEndpoint *q_ptr;

    void removeExchange(CoapExchange *exchange);

    // Network
    QUdpSocket *udp;
    void _q_on_udp_ready_read();
    void sendMessage(CoapMessage &message);
    QHostAddress interface;
    quint16 port;
    bool bind();

    // Classification
    QByteArray generateUniqueToken();
    quint32 currentMid;
    QHash<MidAddressPortKey, CoapExchange *> exchangeByMid;
    QHash<QByteArray, CoapExchange *> exchangeByToken;

    // Reliability
    TimerQueue *timerQueue;
    void _q_on_timeout(const QByteArray &key);
};
#endif // COAPENDPOINT_P_H
