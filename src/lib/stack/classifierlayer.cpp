#include "classifierlayer.h"

#include "coapexchange.h"

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

class ClassifierLayerPrivate
{
public:
    QByteArray generateToken();

    quint32 currentMid;
    QHash<MidAddressPortKey, CoapExchange *> exchangeByMid;
    QHash<QByteArray, CoapExchange *> exchangeByToken;
};

ClassifierLayer::ClassifierLayer(QObject *parent) :
    ILayer(parent), d_ptr(new ClassifierLayerPrivate)
{
    Q_D(ClassifierLayer);
    d->currentMid = rand();
}

QByteArray ClassifierLayerPrivate::generateToken()
{
    QByteArray token;
    // TODO token size from config
    token.resize(2);
    do {
        quint8 *d = (quint8 *)token.data();
        for (int i = 0; i < token.size(); ++i)
            d[i] = rand() % 255;
    } while (exchangeByToken.contains(token));
    return token;
}

ClassifierLayer::~ClassifierLayer()
{
    delete d_ptr;
}

void ClassifierLayer::tx(CoapExchange *exchange, CoapPDU &message)
{
    if (message.isRequest())
        txRequest(exchange, message);
    else if (message.isResponse())
        txResponse(exchange, message);
    else
        txEmpty(exchange, message);
}

void ClassifierLayer::txRequest(CoapExchange *exchange, CoapPDU &message)
{
    Q_D(ClassifierLayer);
    if (message.messageId() == 0)
        message.setMessageId((d->currentMid++) % 65536);

    MidAddressPortKey midKey(message.messageId());
    d->exchangeByMid.insert(midKey, exchange);

    if (message.token().isEmpty()) {
        QByteArray token = d->generateToken();
        message.setToken(token);
        d->exchangeByToken.insert(token, exchange);
    } else {
        /// TODO ongoing exchanges may reuse token, don't show warning in this case
        if (d->exchangeByToken.contains(message.token()))
            qWarning() << "Token reusing" << message.token().toHex();
    }

    m_lower->tx(exchange, message);
}

void ClassifierLayer::txResponse(CoapExchange *exchange, CoapPDU &message)
{

}

void ClassifierLayer::txEmpty(CoapExchange *exchange, CoapPDU &message)
{

}

void ClassifierLayer::rx(CoapExchange *exchange, CoapPDU &message)
{
    if (message.isRequest())
        rxRequest(exchange, message);
    else if (message.isResponse())
        rxResponse(exchange, message);
    else
        rxEmpty(exchange, message);
}

void ClassifierLayer::rxRequest(CoapExchange *exchange, CoapPDU &message)
{

}

void ClassifierLayer::rxResponse(CoapExchange *exchange, CoapPDU &message)
{
    Q_D(ClassifierLayer);

    exchange = d->exchangeByToken.value(message.token(), 0);
    if (exchange) {
        qDebug() << "found exchange" << exchange;
        exchange->handle(message);
    }
}

void ClassifierLayer::rxEmpty(CoapExchange *exchange, CoapPDU &message)
{

}

