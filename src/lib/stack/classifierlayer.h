#ifndef CLASSIFIERLAYER_H
#define CLASSIFIERLAYER_H

#include "ilayer.h"

class ClassifierLayerPrivate;
class ClassifierLayer : public ILayer
{
    Q_OBJECT
public:
    ClassifierLayer(QObject *parent = 0);
    ~ClassifierLayer();

    void tx(CoapExchange *exchange, CoapPDU &message) Q_DECL_OVERRIDE;
    void txRequest(CoapExchange *exchange, CoapPDU &message);
    void txResponse(CoapExchange *exchange, CoapPDU &message);
    void txEmpty(CoapExchange *exchange, CoapPDU &message);

    void rx(CoapExchange *exchange, CoapPDU &message) Q_DECL_OVERRIDE;
    void rxRequest(CoapExchange *exchange, CoapPDU &message);
    void rxResponse(CoapExchange *exchange, CoapPDU &message);
    void rxEmpty(CoapExchange *exchange, CoapPDU &message);

private:
    ClassifierLayerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ClassifierLayer)
};

#endif // CLASSIFIERLAYER_H
