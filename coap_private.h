#ifndef COAP_PRIVATE_H
#define COAP_PRIVATE_H

#include <QObject>
#include <QUdpSocket>
#include "coap.h"

class CoapPDU;

namespace qcoap {

class CoapPrivate : public QObject {
    Q_OBJECT
public:
    CoapPrivate(Coap *parent);

    void bind(const QHostAddress &address, quint16 port);

private slots:
    void readPending();

public:
    QUdpSocket *udp;
    CoapPDU *pdu;
    quint8 buffer[512];

    Coap * const q_ptr;
    Q_DECLARE_PUBLIC(Coap)
};

} // qcoap

#endif // COAP_PRIVATE_H
