#ifndef COAPREQUEST_H
#define COAPREQUEST_H

#include "coapexchange.h"

class CoapRequestPrivate;
class CoapRequest : public CoapExchange
{
public:
    CoapRequest(QObject *parent = 0);

    void get();

protected:
    void handle(CoapMessage &message) Q_DECL_OVERRIDE;

private:
    CoapRequest(CoapRequestPrivate &dd, QObject *parent = 0);
};

#endif // COAPREQUEST_H
