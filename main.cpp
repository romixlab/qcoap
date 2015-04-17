#include <QCoreApplication>
#include "coap.h"
#include "coap_pdu.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    qcoap::Coap coap;

    qcoap::PDU::Type t = qcoap::PDU::Type::CONFIRMABLE;
    quint8 val = static_cast<quint8>(t);

    return a.exec();
}
