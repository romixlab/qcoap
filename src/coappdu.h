#ifndef COAP_PDU_H
#define COAP_PDU_H

#include <QtGlobal>
#include <QByteArray>

struct CoapPDUPrivate;

class CoapOption {
public:
    CoapOption();
    CoapOption(quint16 number, const QByteArray &data);

    quint16 number() const;
    QByteArray data() const;

    bool operator ==(const CoapOption &other);
private:
    quint16 m_number;
    QByteArray m_data;
};

class CoapPDU
{
public:
    enum class Type : quint8 {
    CONFIRMABLE                = 0x00,
    NON_CONFIRMABLE            = 0x10,
    ACKNOWLEDGEMENT            = 0x20,
    RESET                      = 0x30
    };

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

    enum class Option : quint16 {
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

    enum class ContentFormat : quint16 {
    TEXT_PLAIN                 = 0,
    APP_LINK                   = 40,
    APP_XML                    = 41,
    APP_OCTET                  = 42,
    APP_EXI                    = 47,
    APP_JSON                   = 50
    };

    enum class Error {
        FORMAT_ERROR            = 1,
        UNKNOWN_VERSION         = 2,
        WRONG_TOKEN_LENGTH      = 4,
        WRONG_PAYLOAD_MARKER    = 8,
        WRONG_OPTION_HEADER     = 16,
        NOT_ENOUGH_DATA         = 32,
        WRONG_VERSION           = 64,
        WRONG_TOKEN             = 128,


    };
    Q_DECLARE_FLAGS(Errors, Error)

    CoapPDU();
    CoapPDU(const QByteArray &array);
    CoapPDU(const CoapPDU &other);
    ~CoapPDU();

    void setVersion(quint8 version);
    quint8 version() const;

    void setType(Type type);
    Type type() const;

    void setCode(Code code);
    Code code() const;

    void setToken(const QByteArray &token);
    void setToken(const char *token, quint8 length);
    QByteArray token() const;

    void setMessageId(quint16 id);
    quint16 messageId() const;

    void addOption(quint16 optionNumber, const QByteArray &value);
    QList<CoapOption> options() const;
    void setUri(const QString &uri);
    QString uri() const;

    void setContentFormat(ContentFormat format);

    void setPayload(const QByteArray &payload);
    QByteArray payload() const;

    QByteArray pack() const;
    void unpack(const QByteArray &packed);

    CoapPDU::Errors errors() const;
    bool isValid() const;

protected:
    CoapPDUPrivate *d_ptr;
private:
    void detach();
    quint8 *pack_option(quint8 *p, quint16 optionNumber, const QByteArray &value, bool write) const;
    Q_DECLARE_PRIVATE(CoapPDU)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CoapPDU::Errors)

#endif // COAP_PDU_H
