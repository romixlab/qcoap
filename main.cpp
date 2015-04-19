#include <QCoreApplication>
#include "coap.h"
#include "coap_pdu.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    qcoap::Coap coap;

    qcoap::PDU pdu;
    pdu.setVersion(0);
    qcoap::PDU pducopy(pdu);
    qDebug() << pducopy.version();

    return a.exec();
}
