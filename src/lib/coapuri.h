#ifndef COAPURI_H
#define COAPURI_H

#include <QHostAddress>
#include <QSharedDataPointer>

#include "coaplib_global.h"

class CoapUriPrivate;
class COAPLIB_SHARED_EXPORT CoapUri
{
public:
    /**
     * @brief CoapUri construct an invalid uri
     */
    CoapUri();
    CoapUri(const CoapUri &other);
    CoapUri &operator =(const CoapUri &other);
    ~CoapUri();

    void setHost(const QHostAddress &host);
    QHostAddress host() const;
    void setPort(quint16 port);
    quint16 port() const;
    void setPath(const QString &path);
    QString path() const;

private:
    QSharedDataPointer<CoapUriPrivate> d;
};

#endif // COAPURI_H
