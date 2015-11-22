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

    void txRequest(CoapMessage &request) Q_DECL_OVERRIDE;
    void txResponse(CoapMessage &response) Q_DECL_OVERRIDE;
    void txEmpty(CoapMessage &empty) Q_DECL_OVERRIDE;

    void rxRequest(CoapMessage &request) Q_DECL_OVERRIDE;
    void rxResponse(CoapMessage &response) Q_DECL_OVERRIDE;
    void rxEmpty(CoapMessage &empty) Q_DECL_OVERRIDE;

private:
    ClassifierLayerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ClassifierLayer)
};

#endif // CLASSIFIERLAYER_H
