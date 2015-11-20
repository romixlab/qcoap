#include "classifierlayer.h"

#include "midaddressportkey.h"
#include "coapexchange.h"

#include <QHostAddress>

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

