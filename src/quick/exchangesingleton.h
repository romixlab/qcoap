#ifndef EXCHANGESINGLETON_H
#define EXCHANGESINGLETON_H

#include <QObject>

class QQmlEngine;
class QJSEngine;
class CoapExchange;
class ExchangeSingleton : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ExchangeSingleton)
public:
    explicit ExchangeSingleton(QObject *parent = 0);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE CoapExchange *exchange() const;

signals:

public slots:
};

#endif // EXCHANGESINGLETON_H
