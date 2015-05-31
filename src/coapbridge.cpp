#include "coapbridge.h"
#include "coapbridge_p.h"
#include "dqobject.h"

#include <QDebug>

CoapBridgePrivate::CoapBridgePrivate()
{
    qDebug() << "CoapBridgePrivate::CoapBridgePrivate()";
}

CoapBridgePrivate::~CoapBridgePrivate()
{

}

void CoapBridgePrivate::setup()
{
    qDebug() << "CoapBridgePrivate::setup()";
    Q_Q(CoapBridge);
    dq = new DQObject(q);
    QObject::connect(dq, SIGNAL(catched(QString,QVariantList)),
                     q,  SLOT(_q_signal_catched(QString,QVariantList)));
}

void CoapBridgePrivate::_q_signal_catched(const QString &name, const QVariantList &arguments)
{
    qDebug() << "_q_signal_catched(): " << name << arguments;
}

CoapBridge::CoapBridge(QObject *parent) :
    CoapNode(*new CoapBridgePrivate, parent) 
{
    qDebug() << "CoapBridge::CoapBridge(QObject *parent)";
    Q_D(CoapBridge);
    d->setup();
}

CoapBridge::CoapBridge(CoapBridgePrivate &dd, QObject *parent) :
    CoapNode(dd, parent)
{
    qDebug() << "CoapBridge::CoapBridge(CoapBridgePrivate &dd, QObject *parent)";
    Q_D(CoapBridge);
    d->setup();
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

bool CoapBridge::exportSignal(QObject *sender, const QString &signature, const QString &coapURL)
{
    Q_D(CoapBridge);
    QString slotSignature = QString("10x%1%2").arg((quintptr)sender, QT_POINTER_SIZE * 2, 16, QChar('0'))
                                            .arg(signature.right(signature.length() - 1));
    qDebug() << signature << slotSignature;
    d->dq->addSlot(slotSignature);
    connect(sender, signature.toLocal8Bit().data(), d->dq, slotSignature.toLocal8Bit().data());
}

#include "moc_coapbridge.cpp" // intentionally
