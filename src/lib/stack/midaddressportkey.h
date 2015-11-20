#ifndef MIDADDRESSPORTKEY_H
#define MIDADDRESSPORTKEY_H

#include <QHostAddress>

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

#endif // MIDADDRESSPORTKEY_H

