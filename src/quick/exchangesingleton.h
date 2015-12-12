#ifndef EXCHANGESINGLETON_H
#define EXCHANGESINGLETON_H

#include <QObject>

class ExchangeSingleton : public QObject
{
    Q_OBJECT
public:
    explicit ExchangeSingleton(QObject *parent = 0);

signals:

public slots:
};

#endif // EXCHANGESINGLETON_H
