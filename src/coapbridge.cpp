#include "coapbridge.h"
#include "coapbridge_p.h"

#include <QDebug>

CoapBridgePrivate::CoapBridgePrivate()
{
    qDebug() << "CoapBridgePrivate::CoapBridgePrivate()";
}

CoapBridgePrivate::~CoapBridgePrivate()
{

}

CoapBridge::CoapBridge(QObject *parent) :
    CoapNode(*new CoapBridgePrivate, parent) 
{
    qDebug() << "CoapBridge::CoapBridge(QObject *parent)";
}

CoapBridge::CoapBridge(CoapBridgePrivate &dd, QObject *parent) :
    CoapNode(dd, parent)
{
    qDebug() << "CoapBridge::CoapBridge(CoapBridgePrivate &dd, QObject *parent)";
}

void CoapBridge::processPDU(const CoapPDU &pdu, const QHostAddress &from, quint16 fromPort)
{
    CoapPDU::Code c = pdu.code();
    if (c == CoapPDU::Code::GET) {
        QList<CoapOption> opts = pdu.options();
        qDebug() << opts.length();

        CoapPDU answer;
        answer.setVersion(1);
        answer.setType(CoapPDU::Type::NON_CONFIRMABLE);
        answer.setCode(CoapPDU::Code::CONTENT);
        answer.setMessageId(pdu.messageId());
        answer.setContentFormat(CoapPDU::ContentFormat::APP_LINK);
        answer.setPayload("</sensors/temp>;if=\"sensor\"");

        sendPDU(answer, from, fromPort);
    }
}

void CoapBridge::sendPDU(const CoapPDU &pdu, const QHostAddress &to, quint16 toPort)
{
    CoapNode::sendPDU(pdu, to, toPort);
}

#include "moc_coapbridge.cpp" // intentionally
