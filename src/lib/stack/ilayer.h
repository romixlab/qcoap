#ifndef LAYERBASE_H
#define LAYERBASE_H

#include "coapmessage.h"

#include <QObject>

class CoapExchange;
class ILayer : public QObject
{
    Q_OBJECT
public:
    ILayer(QObject *parent = 0);
    virtual ~ILayer();
    /**
     * @brief tx towards network, pass to lower layers
     * @param exchange exchange that message belongs to
     * @param message pdu + address
     */
    virtual void tx(CoapMessage &message);
    virtual void txRequest(CoapMessage &request);
    virtual void txResponse(CoapMessage &response);
    virtual void txEmpty(CoapMessage &empty);
    /**
     * @brief rx from network, pass to upper layers
     * @param exchange
     * @param message
     */
    virtual void rx(CoapMessage &message);
    virtual void rxRequest(CoapMessage &request);
    virtual void rxResponse(CoapMessage &response);
    virtual void rxEmpty(CoapMessage &empty);

    void setUpperLayer(ILayer *upper);
    void setLowerLayer(ILayer *lower);
protected:
    ILayer *m_upper;
    ILayer *m_lower;
};

#endif // LAYERBASE_H
