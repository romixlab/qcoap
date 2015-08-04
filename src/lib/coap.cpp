#include <QList>
#include <QDebug>

#include "coap.h"

struct CoapPrivate {
    CoapPrivate() {}
    ~CoapPrivate() {}
    QList<CoapEndpoint *> endpoints;
};

Q_GLOBAL_STATIC(CoapPrivate, coap_private)

Coap::Coap()
{
}

CoapEndpoint *Coap::defaultEndpoint()
{
    CoapPrivate *d = coap_private;
    if (!d || d->endpoints.isEmpty()) {
        qWarning() << "Coap::defaultEndpoint(): no default endpoint";
        return 0;
    }
    return d->endpoints.at(0);
}

void Coap::addEndpoint(CoapEndpoint *endpoint)
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::addEndpoint(): failed to add endpoint";
        return;
    }
    d->endpoints.append(endpoint);
}

void Coap::removeEndpoint(CoapEndpoint *endpoint)
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::removeEndpoint(): failed to remove endpoint";
        return;
    }
    d->endpoints.removeOne(endpoint);
}

QList<CoapEndpoint *> Coap::endpoints()
{
    CoapPrivate *d = coap_private;
    if (!d) {
        qWarning() << "Coap::endpoints(): there is no endpoints";
        return QList<CoapEndpoint *>();
    }
    return d->endpoints;
}

QString Coap::toString(Type type)
{
    switch (type) {
    case Type::CONFIRMABLE:
        return QStringLiteral("Confirmable");
    case Type::NON_CONFIRMABLE:
        return QStringLiteral("NonConfirmable");
    case Type::ACKNOWLEDGEMENT:
        return QStringLiteral("Acknowledgement");
    case Type::RESET:
        return QStringLiteral("Reset");
    }
    return QStringLiteral("UnkownType");
}

QString Coap::toString(Code code)
{
    switch (code) {
    case Code::EMPTY:
        return QStringLiteral("Empty");
    case Code::GET:
        return QStringLiteral("Get");
    case Code::POST:
        return QStringLiteral("Post");
    case Code::PUT:
        return QStringLiteral("Put");
    case Code::DELETE:
        return QStringLiteral("Delete");
    case Code::CREATED:
        return QStringLiteral("Created");
    case Code::DELETED:
        return QStringLiteral("Deleted");
    case Code::VALID:
        return QStringLiteral("Valid");
    case Code::CHANGED:
        return QStringLiteral("Changed");
    case Code::CONTENT:
        return QStringLiteral("Content");
    case Code::BAD_REQUEST:
        return QStringLiteral("BadRequest");
    case Code::UNAUTHORIZED:
        return QStringLiteral("Unauthorized");
    case Code::BAD_OPTION:
        return QStringLiteral("BadOption");
    case Code::FORBIDDEN:
        return QStringLiteral("Forbidden");
    case Code::NOT_FOUND:
        return QStringLiteral("NotFound");
    case Code::METHOD_NOT_ALLOWED:
        return QStringLiteral("MethodNotAllowed");
    case Code::NOT_ACCEPTABLE:
        return QStringLiteral("NotAcceptable");
    case Code::PRECONDITION_FAILED:
        return QStringLiteral("PreconditionFailed");
    case Code::REQUEST_ENTITY_TOO_LARGE:
        return QStringLiteral("RequestEntityTooLarge");
    case Code::UNSUPPORTED_CONTENT_FORMAT:
        return QStringLiteral("UnsupportedContentFormat");
    case Code::INTERNAL_SERVER_ERROR:
        return QStringLiteral("InternalServerError");
    case Code::NOT_IMPLEMENTED:
        return QStringLiteral("NotImplemented");
    case Code::BAD_GATEWAY:
        return QStringLiteral("BadGateway");
    case Code::SERVICE_UNAVAILABL:
        return QStringLiteral("ServiceUnavailabl");
    case Code::GATEWAY_TIMEOUT:
        return QStringLiteral("GatewayTimeout");
    case Code::PROXYING_NOT_SUPPORTED:
        return QStringLiteral("ProxyingNotSupported");
    case Code::UNDEFINED_CODE:
        return QStringLiteral("UndefinedCode");
    }
    return QStringLiteral("UnkownCode");
}

bool Coap::isSuccess(Coap::Code code)
{
    quint8 c = (quint8)code;
    return (c >= 65) && (c <= 127);
}

QString Coap::toString(OptionType optionType)
{
    switch (optionType) {
    case OptionType::IF_MATCH:
        return QStringLiteral("IfMatch");
    case OptionType::URI_HOST:
        return QStringLiteral("UriHost");
    case OptionType::ETAG:
        return QStringLiteral("Etag");
    case OptionType::IF_NONE_MATCH:
        return QStringLiteral("IfNoneMatch");
    case OptionType::OBSERVE:
        return QStringLiteral("Observe");
    case OptionType::URI_PORT:
        return QStringLiteral("UriPort");
    case OptionType::LOCATION_PATH:
        return QStringLiteral("LocationPath");
    case OptionType::URI_PATH:
        return QStringLiteral("UriPath");
    case OptionType::CONTENT_FORMAT:
        return QStringLiteral("ContentFormat");
    case OptionType::MAX_AGE:
        return QStringLiteral("MaxAge");
    case OptionType::URI_QUERY:
        return QStringLiteral("UriQuery");
    case OptionType::ACCEPT:
        return QStringLiteral("Accept");
    case OptionType::LOCATION_QUERY:
        return QStringLiteral("LocationQuery");
    case OptionType::BLOCK2:
        return QStringLiteral("Block2");
    case OptionType::BLOCK1:
        return QStringLiteral("Block1");
    case OptionType::SIZE2:
        return QStringLiteral("Size2");
    case OptionType::PROXY_URI:
        return QStringLiteral("ProxyUri");
    case OptionType::PROXY_SCHEME:
        return QStringLiteral("ProxyScheme");
    case OptionType::SIZE1:
        return QStringLiteral("Size1");
    }
    return QStringLiteral("UnkownOptionType");
}

QString Coap::toString(ContentFormat contentFormat)
{
    switch(contentFormat) {
    case ContentFormat::TEXT_PLAIN:
        return QStringLiteral("text/plain");
    case ContentFormat::APP_LINK:
            return QStringLiteral("app/link");
    case ContentFormat::APP_XML:
            return QStringLiteral("app/xml");
    case ContentFormat::APP_OCTET:
            return QStringLiteral("app/octet");
    case ContentFormat::APP_EXI:
            return QStringLiteral("app/exi");
    case ContentFormat::APP_JSON:
        return QStringLiteral("app/json");
    }
    return QStringLiteral("UnkownFormat");
}




