#include "coapuri.h"

#include <QRegularExpression>

class CoapUriPrivate : public QSharedData
{
public:
    CoapUriPrivate()
    {
        port = 0;
    }

    CoapUriPrivate(const CoapUriPrivate &other) :
        QSharedData(other)
    {
    }

    ~CoapUriPrivate()
    {
    }

    QHostAddress host;
    quint16 port;
    QString path;
};

CoapUri::CoapUri() :
    d(new CoapUriPrivate)
{  
}

CoapUri::CoapUri(const QString &fromString)
{
    QRegularExpression re("(coap|coaps)")
}

CoapUri::CoapUri(const CoapUri &other) :
    d(other.d)
{
}

CoapUri &CoapUri::operator =(const CoapUri &other)
{
    d = other.d;
    return *this;
}

CoapUri::~CoapUri()
{
}

void CoapUri::setHost(const QHostAddress &address)
{
    d->host = address;
}

QHostAddress CoapUri::host() const
{
    return d->host;
}

void CoapUri::setPort(quint16 port)
{
    d->port = port;
}

quint16 CoapUri::port() const
{
    return d->port;
}

void CoapUri::setPath(const QString &path)
{
    d->path = path;
}

QString CoapUri::path() const
{
    return d->path;
}

