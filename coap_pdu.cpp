#include "coap_pdu.h"
#include "cantcoap.h"
#include <QDebug>

qcoap::PDU::PDU()
{
    qDebug() << "PDU()";
    p = new CoapPDU();
}

qcoap::PDU::PDU(const qcoap::PDU &other)
{
    qDebug() << "PDU(&other)";
    p = other.p;
}

qcoap::PDU::~PDU()
{

}

bool qcoap::PDU::setVersion(quint8 version)
{
    p->setVersion(version);
}

quint8 qcoap::PDU::version() const
{
    return p->getVersion();
}
