#ifndef COAP_H
#define COAP_H

#include "coaplib_global.h"
#include <QString>

class COAPLIB_SHARED_EXPORT Coap
{
public:
    enum class Type : quint8 {
    CONFIRMABLE                = 0x00,
    NON_CONFIRMABLE            = 0x01,
    ACKNOWLEDGEMENT            = 0x02,
    RESET                      = 0x03
    };
    static QString toString(Type type);
    
    enum class Code : quint8 {
    EMPTY                      = 0x00,
    GET                        = 0x01,
    POST                       = 0x02,
    PUT                        = 0x03,
    DELETE                     = 0x04,
    CREATED                    = 0x41,
    DELETED                    = 0x42,
    VALID                      = 0x43,
    CHANGED                    = 0x44,
    CONTENT                    = 0x45,
    BAD_REQUEST                = 0x80,
    UNAUTHORIZED               = 0x81,
    BAD_OPTION                 = 0x82,
    FORBIDDEN                  = 0x83,
    NOT_FOUND                  = 0x84,
    METHOD_NOT_ALLOWED         = 0x85,
    NOT_ACCEPTABLE             = 0x86,
    PRECONDITION_FAILED        = 0x8c,
    REQUEST_ENTITY_TOO_LARGE   = 0x8d,
    UNSUPPORTED_CONTENT_FORMAT = 0x8f,
    INTERNAL_SERVER_ERROR      = 0xa0,
    NOT_IMPLEMENTED            = 0xa1,
    BAD_GATEWAY                = 0xa2,
    SERVICE_UNAVAILABL         = 0xa3,
    GATEWAY_TIMEOUT            = 0xa4,
    PROXYING_NOT_SUPPORTED     = 0xa5,
    UNDEFINED_CODE             = 0xff
    };
    static QString toString(Code code);
    static bool isSuccess(Code code);

    enum class OptionType : quint16 {
    IF_MATCH                   = 1,
    URI_HOST                   = 3,
    ETAG                       = 4,
    IF_NONE_MATCH              = 5,
    OBSERVE                    = 6,
    URI_PORT                   = 7,
    LOCATION_PATH              = 8,
    URI_PATH                   = 11,
    CONTENT_FORMAT             = 12,
    MAX_AGE                    = 14,
    URI_QUERY                  = 15,
    ACCEPT                     = 17,
    LOCATION_QUERY             = 20,
    BLOCK2                     = 23,
    BLOCK1                     = 27,
    SIZE2                      = 28,
    PROXY_URI                  = 35,
    PROXY_SCHEME               = 39,
    SIZE1                      = 60
    };
    static QString toString(OptionType optionType);

    enum class ContentFormat : quint16 {
    TEXT_PLAIN                 = 0,
    APP_LINK                   = 40,
    APP_XML                    = 41,
    APP_OCTET                  = 42,
    APP_EXI                    = 47,
    APP_JSON                   = 50
    };
    static QString toString(ContentFormat contentFormat);

private:
    Coap();
};

#endif // COAP_H
