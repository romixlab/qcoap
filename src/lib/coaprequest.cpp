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
    CoapPDU pdu;
    pdu.setCode(CoapPDU::Code::Get);
    pdu.setType(CoapPDU::Type::Confirmable);
    pdu.addOption(CoapPDU::OptionType::UriPath, "/");
    CoapExchange::send(pdu);
}

