#include <QCoreApplication>
#include <coapendpoint.h>

#include <QUrl>

int dumpPDU(const QByteArray &array)
{
    qDebug() << "--- CoapPDU dump below ---";
    qDebug() << "PDU length:" << array.length() << "bytes";
    CoapPDU pdu;
    pdu.unpack(array);
    bool isValid = pdu.isValid();
    qDebug() << "isValid:" << isValid;
    if (!isValid) {
        CoapPDU::Errors err = pdu.errors();
        if (err.testFlag(CoapPDU::Error::FORMAT_ERROR))
            qDebug() << "FORMAT_ERROR";
        if (err.testFlag(CoapPDU::Error::NOT_ENOUGH_DATA))
            qDebug() << "NOT_ENOUGH_DATA";
        if (err.testFlag(CoapPDU::Error::UNKNOWN_VERSION))
            qDebug() << "UNKNOWN_VERSION";
        if (err.testFlag(CoapPDU::Error::WRONG_OPTION_HEADER))
            qDebug() << "WRONG_OPTION_HEADER";
        if (err.testFlag(CoapPDU::Error::WRONG_PAYLOAD_MARKER))
            qDebug() << "WRONG_PAYLOAD_MARKER";
        if (err.testFlag(CoapPDU::Error::WRONG_TOKEN))
            qDebug() << "WRONG_TOKEN";
        if (err.testFlag(CoapPDU::Error::WRONG_TOKEN_LENGTH))
            qDebug() << "WRONG_TOKEN_LENGTH";
        if (err.testFlag(CoapPDU::Error::WRONG_VERSION))
            qDebug() << "WRONG_VERSION";
        return -1;
    }
    qDebug() << "Type:" << Coap::toString(pdu.type())
             << "Code:" << Coap::toString(pdu.code())
             << "(Token:" << pdu.token().toHex() << ")";
    qDebug() << "Message ID:" << pdu.messageId();
    for (int i = 0; i < pdu.optionsCount(); ++i) {
        QDebug dbg(QtDebugMsg);
        dbg.nospace();
        dbg << "Option:" << i << ", ";
        CoapOption option = pdu.option(i);
        dbg << "isValid:" << option.isValid() << ", ";
        if (!option.isValid())
            continue;
        dbg << "Number:" << (int)option.type() << " - " << Coap::toString(option.type()) << ", ";
        dbg << "Data:" << option.data().toHex();
//        if (option.type() == Coap::OptionType:: || option.type() == 3)
            dbg << " asString: " << option.data();
    }
    if (pdu.payload().isEmpty())
        qDebug() << "No payload";
    else
        qDebug() << "Payload: " << pdu.payload().toHex() << "asString" << pdu.payload();
    qDebug() << "--------------------------";

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    CoapEndpoint endpoint;
//    endpoint.bind(QHostAddress::LocalHost);

//    qDebug() << Coap::defaultEndpoint();

    qDebug()  << QUrl("coap://coap.me/%7Etest").toString();

    return a.exec();
}
