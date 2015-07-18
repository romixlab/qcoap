#ifndef COAP_PDU_H
#define COAP_PDU_H

#include <QtGlobal>
#include <QByteArray>
#include <QSharedDataPointer>

#include "coaplib_global.h"
#include "coap.h"

class COAPLIB_SHARED_EXPORT CoapOption {
public:
    CoapOption();
    CoapOption(Coap::OptionType optionType, const QByteArray &data);

    Coap::OptionType type() const;
    QByteArray data() const;
    bool isValid() const;

    bool operator ==(const CoapOption &other);
private:
    Coap::OptionType m_type;
    QByteArray m_data;
};

struct CoapPDUPrivate;
class COAPLIB_SHARED_EXPORT CoapPDU
{
public:
    CoapPDU();
    CoapPDU(const QByteArray &array);
    CoapPDU(const CoapPDU &other);
    CoapPDU &operator =(const CoapPDU &other);
    ~CoapPDU();

    void setVersion(quint8 version);
    quint8 version() const;

    void setType(Coap::Type type);
    Coap::Type type() const;

    void setCode(Coap::Code code);
    Coap::Code code() const;

    void setToken(const QByteArray &token);
    void setToken(const char *token, quint8 length);
    QByteArray token() const;

    void setMessageId(quint16 id);
    quint16 messageId() const;

    void addOption(Coap::OptionType optionType, const QByteArray &data = QByteArray());
    QList<CoapOption> options() const;
    int optionsCount() const;
    CoapOption option(int idx) const;

    void setUri(const QString &uri);
    QString uri() const;

    void setContentFormat(Coap::ContentFormat format);
    Coap::ContentFormat contentFormat() const;

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

#endif // COAP_PDU_H
