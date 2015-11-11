#ifndef COAPURI_H
#define COAPURI_H

#include <QHostAddress>
#include <QSharedDataPointer>

#include "coaplib_global.h"

class CoapUriPrivate;
class COAPLIB_SHARED_EXPORT CoapUri
{
    Q_GADGET
    Q_PROPERTY(quint16 port READ port WRITE setPort)
    Q_PROPERTY(QString path READ path WRITE setPath)

public:
    /**
     * @brief CoapUri construct an invalid uri
     */
    CoapUri();
    CoapUri(const QString &fromString);
    CoapUri(const CoapUri &other);
    CoapUri &operator =(const CoapUri &other);
    ~CoapUri();

    void setHost(const QHostAddress &address);
    QHostAddress host() const;
    void setPort(quint16 port);
    quint16 port() const;
    void setPath(const QString &path);
    QString path() const;

private:
    QSharedDataPointer<CoapUriPrivate> d;
};

Q_DECLARE_METATYPE(CoapUri)

#endif // COAPURI_H
