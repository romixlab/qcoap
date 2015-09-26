#ifndef COAP_PDU_H
#define COAP_PDU_H

#include "coaplib_global.h"
#include "coap.h"

#include <QObject>
#include <QByteArray>
#include <QSharedDataPointer>

struct CoapPDUPrivate;
class CoapOption;
class COAPLIB_SHARED_EXPORT CoapPDU
{
    Q_GADGET
public:
    CoapPDU();
    CoapPDU(const QByteArray &array);
    CoapPDU(const CoapPDU &other);
    CoapPDU &operator =(const CoapPDU &other);
    ~CoapPDU();

    enum class Type : quint8 {
    Confirmable     = 0x00,
    NonConfirmable  = 0x01,
    Acknowledgement = 0x02,
    Reset           = 0x03
    };
    Q_ENUM(Type)

    enum class Code : quint8 {
    Empty                    = 0x00,
    Get                      = 0x01,
    Post                     = 0x02,
    Put                      = 0x03,
    Delete                   = 0x04,
    Created                  = 0x41,
    Deleted                  = 0x42,
    Valid                    = 0x43,
    Changed                  = 0x44,
    Content                  = 0x45,
    BadRequest               = 0x80,
    Unauthorized             = 0x81,
    BadOption                = 0x82,
    Forbidden                = 0x83,
    NotFound                 = 0x84,
    MethodNotAllowed         = 0x85,
    NotAcceptable            = 0x86,
    PreconditionFailed       = 0x8c,
    RequestEntityTooLarge    = 0x8d,
    UnsupportedContentFormat = 0x8f,
    InternalServerError      = 0xa0,
    NotImplemented           = 0xa1,
    BadGateway               = 0xa2,
    ServiceUnavailabl        = 0xa3,
    GatewayTimeout           = 0xa4,
    ProxyingNotSupported     = 0xa5,
    UndefinedCode            = 0xff
    };
    Q_ENUM(Code)

    enum class OptionType : quint16 {
    IfMatch       = 1,
    UriHost       = 3,
    Etag          = 4,
    IfNoneMatch   = 5,
    Observe       = 6,
    UriPort       = 7,
    LocationPath  = 8,
    UriPath       = 11,
    ContentFormat = 12,
    MaxAge        = 14,
    UriQuery      = 15,
    Accept        = 17,
    LocationQuery = 20,
    Block2        = 23,
    Block1        = 27,
    Size2         = 28,
    ProxyUri      = 35,
    ProxyScheme   = 39,
    Size1         = 60
    };
    Q_ENUM(OptionType)

    enum class ContentFormat : quint16 {
    TextPlain = 0,
    AppLink   = 40,
    AppXml    = 41,
    AppOctet  = 42,
    AppExi    = 47,
    AppJson   = 50
    };
    Q_ENUM(ContentFormat)

    void setVersion(quint8 version);
    quint8 version() const;

    void setType(Type type);
    Type type() const;

    void setCode(Code code);
    Code code() const;
    bool isRequest() const;
    bool isResponse() const;

    void setToken(const QByteArray &token);
    void setToken(const char *token, quint8 length);
    QByteArray token() const;

    void setMessageId(quint16 id);
    quint16 messageId() const;

    void addOption(OptionType optionType, const QByteArray &data = QByteArray());
    QList<CoapOption> options() const;
    int optionsCount() const;
    CoapOption option(int idx) const;

    void setUri(const QString &uri);
    QString uri() const;

    void setContentFormat(ContentFormat format);
    ContentFormat contentFormat() const;

    void setPayload(const QByteArray &payload);
    QByteArray payload() const;

    QByteArray pack() const;
    void unpack(const QByteArray &packed);

    enum class Error {
        FORMAT_ERROR           = 1,
        UNKNOWN_VERSION        = 2,
        WRONG_TOKEN_LENGTH     = 4,
        WRONG_PAYLOAD_MARKER   = 8,
        WRONG_OPTION_HEADER    = 16,
        NOT_ENOUGH_DATA        = 32,
        WRONG_VERSION          = 64,
        WRONG_TOKEN            = 128,
    };
    Q_DECLARE_FLAGS(Errors, Error)
    CoapPDU::Errors errors() const;
    QString errorString() const;
    bool isValid() const;
    bool isNull() const;


private:
    QSharedDataPointer<CoapPDUPrivate> d;
    quint8 *pack_option(quint8 *p, quint16 optionNumber, const QByteArray &value, bool write) const;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(CoapPDU::Errors)

class COAPLIB_SHARED_EXPORT CoapOption {
public:
    CoapOption();
    CoapOption(CoapPDU::OptionType optionType, const QByteArray &data);

    CoapPDU::OptionType type() const;
    QByteArray data() const;
    bool isValid() const;

    bool operator ==(const CoapOption &other);
private:
    CoapPDU::OptionType m_type;
    QByteArray m_data;
};

QDebug operator<<(QDebug debug, const CoapPDU &pdu);

#endif // COAP_PDU_H
