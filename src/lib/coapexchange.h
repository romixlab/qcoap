#ifndef COAPEXCHANGE_H
#define COAPEXCHANGE_H

#include "coaplib_global.h"
#include <QObject>
#include <QUrl>

class CoapEndpoint;
class CoapExchangePrivate;
class CoapExchange : public QObject
{
    Q_OBJECT
public:
    CoapExchange(QObject *parent = 0);
    CoapExchange(CoapEndpoint *throughEndpoint, QObject *parent = 0);
    CoapExchange(const CoapExchange &other);
    ~CoapExchange();

    void setUri(const QUrl &uri);

    void get();


private:
    CoapExchangePrivate *d_ptr;
    Q_DECLARE_PRIVATE(CoapExchange)
};

#endif // COAPEXCHANGE_H
