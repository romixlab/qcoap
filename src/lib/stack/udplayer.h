#ifndef UDPLAYER_H
#define UDPLAYER_H

#include "layerbase.h"

#include <QHostAddress>

class UDPLayerPrivate;
class UDPLayer : public LayerBase
{
    Q_OBJECT
public:
    UDPLayer(LayerBase *upper, QObject *parent);

    void tx(CoapExchange *exchange, CoapPDU &message);

    void bind(const QHostAddress &address, quint16 port);

private slots:
    void onReadyRead();
private:
    UDPLayerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(UDPLayer)
};

#endif // UDPLAYER_H
