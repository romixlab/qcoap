#include "coaprequest.h"
#include "coaprequest_p.h"

CoapRequest::CoapRequest(QObject *parent) :
    CoapExchange(*new CoapRequestPrivate, parent)
{

}

CoapRequest::CoapRequest(CoapRequestPrivate &dd, QObject *parent) :
    CoapExchange(dd, parent)
{

}

void CoapRequest::get()
{

}

void CoapRequest::handle(const CoapPDU &message)
{
    qDebug() << "CoapRequest handle()" << message;
}

