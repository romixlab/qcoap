#ifndef COAP_H
#define COAP_H

#include <QObject>

namespace qcoap {

class CoapPrivate;

class Coap : public QObject
{
    Q_OBJECT
public:
    explicit Coap(QObject *parent = 0);
    ~Coap();

signals:

public slots:

protected:
    CoapPrivate * d_ptr;

private:
    Q_DECLARE_PRIVATE(Coap)
};

} // qcoap

#endif // COAP_H
