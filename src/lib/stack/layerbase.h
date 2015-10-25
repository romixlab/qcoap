#ifndef LAYERBASE_H
#define LAYERBASE_H

#include "coappdu.h"

#include <QObject>

class CoapExchange;
class LayerBase : public QObject
{
public:
    LayerBase(QObject *parent = 0);
    /**
     * @brief tx towards network, pass to lower layers
     * @param exchange exchange that message belongs to
     * @param message pdu + address
     */
    virtual void tx(CoapExchange *exchange, CoapPDU &message);
    /**
     * @brief rx from network, pass to upper layers
     * @param exchange
     * @param message
     */
    virtual void rx(CoapExchange *exchange, CoapPDU &message);

    void setUpperLayer(LayerBase *upper);
    void setLowerLayer(LayerBase *lower);
protected:
    LayerBase *m_upper;
    LayerBase *m_lower;
};

#endif // LAYERBASE_H
