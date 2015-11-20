#ifndef RELIABILITYLAYER_H
#define RELIABILITYLAYER_H

#include "coaplib_global.h"
#include "ilayer.h"

class ReliabilityLayerPrivate;
class COAPLIB_SHARED_EXPORT ReliabilityLayer : public ILayer
{
public:
    ReliabilityLayer(QObject *parent = 0);
    ~ReliabilityLayer();

    void txRequest(CoapMessage *request) Q_DECL_OVERRIDE;
    void txResponse(CoapMessage *response) Q_DECL_OVERRIDE;
    void txEmpty(CoapMessage *empty) Q_DECL_OVERRIDE;

    void rxRequest(CoapMessage *request) Q_DECL_OVERRIDE;
    void rxResponse(CoapMessage *response) Q_DECL_OVERRIDE;
    void rxEmpty(CoapMessage *empty) Q_DECL_OVERRIDE;

private:
    ReliabilityLayerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ReliabilityLayer)
};

#endif // RELIABILITYLAYER_H
