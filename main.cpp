#include <QCoreApplication>
#include "coappdu.h"
#include "coapnode.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


//    CoapPDU pdu;
//    pdu.setVersion(2);
//    pdu.setMessageId(1234);
//    const char token[] = {0x01, 0x02, 0x03, 0x04};
//    pdu.setToken(token, 4);
//    pdu.setContentFormat(CoapPDU::ContentFormat::APP_LINK);
//    const char data[] = {0x05, 0x06, 0x07, 0x08};
//    pdu.setPayload(QByteArray(data, 4));

//    qDebug() << "isValid" << pdu.isValid();
//    qDebug() << pdu.errors().testFlag(CoapPDU::Error::WRONG_VERSION);

//    QByteArray packed = pdu.pack();
//    qDebug() << packed.size();
//    qDebug() << packed.toHex();

    QByteArray packed = QByteArray::fromHex("4201257da4feb474657374c102");

    CoapPDU updu;
    updu.unpack(packed);
    qDebug() << "ver:" << updu.version();
    qDebug() << "msg id:" << updu.messageId();
    qDebug() << "token:" << updu.token().toHex();
    updu.options();
    qDebug() << "payload:" << updu.payload().toHex();

    return 0;
    return a.exec();
}
