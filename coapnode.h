#ifndef COAPNODE_H
#define COAPNODE_H

#include <QObject>

class QUdpSocket;

class CoapNode : public QObject
{
    Q_OBJECT
public:
    explicit CoapNode(QObject *parent = 0);
    ~CoapNode();

signals:

public slots:

    void readPending();

private:
    QUdpSocket *udp;
};

#endif // COAPNODE_H
