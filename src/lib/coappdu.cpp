#include "coappdu.h"
#include <QAtomicInt>
#include "endianhelper.h"
#include <QDebug>

CoapOption::CoapOption() : m_type((Coap::OptionType)0) {}
CoapOption::CoapOption(Coap::OptionType optionType, const QByteArray &data):
    m_type(optionType), m_data(data)
{}

Coap::OptionType CoapOption::type() const
{
    return m_type;
}

QByteArray CoapOption::data() const
{
    return m_data;
}

bool CoapOption::isValid() const
{
    return (int)m_type != 0;
}

bool CoapOption::operator ==(const CoapOption &other)
{
    return (m_type == other.m_type) && (m_data == other.m_data);
}

class CoapPDUPrivate : public QSharedData
{
public:
    CoapPDUPrivate() :
        version(1),
        type(Coap::Type::RESET),
        code(Coap::Code::EMPTY),
        message_id(0)
    {
        //@todo   d->errors =
    }

    ~CoapPDUPrivate()
    { }

    CoapPDUPrivate(const CoapPDUPrivate &other) :
        QSharedData(other),
        version(other.version),
        type(other.type),
        code(other.code),
        message_id(other.message_id),
        token(other.token),
        options(other.options),
        payload(other.payload),
        errors(other.errors)
    { }

    quint8 version;
    Coap::Type type;
    Coap::Code code;
    quint16 message_id;
    QByteArray token;
    QList<CoapOption> options;
    QByteArray payload;
    CoapPDU::Errors errors;
};

CoapPDU::CoapPDU()
    : d(new CoapPDUPrivate)
{

}

CoapPDU::CoapPDU(const CoapPDU &other)
{
    d = other.d;
}

CoapPDU &CoapPDU::operator =(const CoapPDU &other)
{
    d = other.d;
    return *this;
}

CoapPDU::~CoapPDU()
{}

void CoapPDU::setVersion(quint8 version)
{
    if (d->version == version)
        return;
    d->version = version;
    if (version != 1)
        d->errors |= Error::WRONG_VERSION;
}

quint8 CoapPDU::version() const
{
    return d->version;
}

void CoapPDU::setType(Coap::Type type)
{
    if (d->type == type)
        return;
    d->type = type;
}

Coap::Type CoapPDU::type() const
{
    return d->type;
}

void CoapPDU::setCode(Coap::Code code)
{
    if (d->code == code)
        return;
    d->code = code;
}

Coap::Code CoapPDU::code() const
{
    return d->code;
}

void CoapPDU::setToken(const QByteArray &token)
{
    if (d->token == token)
        return;
    d->token = token;
    if (token.length() > 8)
        d->errors |= Error::WRONG_TOKEN;
}

void CoapPDU::setToken(const char *token, quint8 length)
{
    QByteArray t(token, length);
    if (d->token == t)
        return;
    d->token = t;
    if (length > 8)
        d->errors |= Error::WRONG_TOKEN;
}

QByteArray CoapPDU::token() const
{
    return d->token;
}

void CoapPDU::setMessageId(quint16 id)
{
    if (d->message_id == id)
        return; 
    d->message_id = id;
}

quint16 CoapPDU::messageId() const
{
    return d->message_id;
}

void CoapPDU::addOption(Coap::OptionType optionType, const QByteArray &value)
{
    int idx = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        if (optionType > d->options[i].type())
            idx++;
    }
    CoapOption option(optionType, value);
    if (d->options.contains(option))
        return;
    d->options.insert(idx, option);
}

QList<CoapOption> CoapPDU::options() const
{
    return d->options;
}

int CoapPDU::optionsCount() const
{
    return d->options.length();
}

CoapOption CoapPDU::option(int idx) const
{
    if (idx < 0 || idx >= d->options.length())
        return CoapOption();
    return d->options[idx];
}

void CoapPDU::setContentFormat(Coap::ContentFormat format)
{
    Coap::OptionType optionContentFormat = Coap::OptionType::CONTENT_FORMAT;
    if (format == Coap::ContentFormat::TEXT_PLAIN) {
        addOption(optionContentFormat, QByteArray());
    } else if ((quint16)format < 256) {
        quint8 f = (quint8)format;
        addOption(optionContentFormat, QByteArray((const char*)&f, 1));
    } else {
        quint16 f = (quint16)format;
        QByteArray data;
        data.resize(2);
        endian_store16(data.data(), f);
        addOption(optionContentFormat, data);
    }
}

void CoapPDU::setPayload(const QByteArray &payload)
{
    if (d->payload == payload)
        return;
    d->payload = payload;
}

QByteArray CoapPDU::payload() const
{
    return d->payload;
}

quint8 *CoapPDU::pack_option(quint8 *p, quint16 optionNumber, const QByteArray &value, bool write) const
{
    quint8 *h = p;
    p++;
    if (optionNumber <= 12) {
        if (write)
            *h = (quint8)optionNumber << 4;
    } else if (optionNumber > 12 && optionNumber <= 255) {
        if (write) {
            *h = 13 << 4;
            *p = (quint8)optionNumber - 13;
        }
        p++;
    } else if (optionNumber > 255) {
        *h = 14 << 4;
        // write me!!!

    }

    if (value.length() <= 12) {
        if (write)
            *h |= (quint8)value.length();
    } else if (value.length() > 12 && value.length() <= 255) {
        if (write) {
            *h |= 13;
            *p = (quint8)value.length();
        }
        p++;
    } else if (value.length() > 255) {
        *h |= 14;
        //write me
    }

    if (write)
        memcpy(p, value.data(), value.length());
    p += value.length();

    return p;
}

QByteArray CoapPDU::pack() const
{

//    const_cast<CoapPDUPrivate *>(d)->errors = Errors(0);
    quint32 pduSize = 4; // header
    pduSize += d->token.length();
    quint8 *p = 0;
    quint16 optionDelta = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        quint16 optionNumber = (quint16)d->options[i].type() - optionDelta;
        p = pack_option(p, optionNumber, d->options[i].data(), false);
        optionDelta = optionNumber;
    }
    pduSize += p - (quint8 *)0;

    if (d->payload.size() > 0)
        pduSize += 1; // payload marker
    pduSize += d->payload.length();

    QByteArray packed;
    packed.resize(pduSize);
    p = (quint8 *)packed.data();
    p[0] = d->version << 6;
    p[0] |= (quint8)d->type << 4;
    p[0] |= (quint8)d->token.length();
    p[1] = (quint8)d->code;
    endian_store16((p + 2), d->message_id);
    p += 4;
    memcpy(p, d->token.data(), d->token.length());
    p += d->token.length();
    optionDelta = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        quint16 optionNumber = (quint16)d->options[i].type() - optionDelta;
        p = pack_option(p, optionNumber, d->options[i].data(), true);
        optionDelta = optionNumber;
    }
    *(p++) = 0xff;
    memcpy(p, d->payload, d->payload.length());

    return packed;
}

void CoapPDU::unpack(const QByteArray &packed)
{
    d->errors = Errors(0);
    d->options.clear();
    quint8 *p = (quint8 *)packed.data();
    quint8 *pend = (quint8 *)packed.data() + packed.size() - 1;

    if (packed.size() < 4) {
        d->errors |= Error::FORMAT_ERROR;
        return;
    }

    d->version = (p[0] & 0xc0) >> 6;
    if (d->version != 1)
        d->errors |= Error::UNKNOWN_VERSION;

    d->type = (Coap::Type)( (p[0] & 0x30) >> 4 );
    quint8 tokenLength = (p[0] & 0xf);
    d->code = (Coap::Code)p[1];
    if (tokenLength > 8)
        d->errors |= Error::WRONG_TOKEN_LENGTH;
    else
        d->token = QByteArray((const char *)p + 4, tokenLength);

    d->message_id = endian_load16(quint16, (p + 2));

    if (packed.size() == 4) // only header present, ok
        return;

    p += tokenLength;
    p += 4;

    quint16 optionNumber = 0;
    do {
        if (*p == 0xff) {
            quint32 payloadSize = pend - p;
            if (payloadSize == 0)
                d->errors |= Error::WRONG_PAYLOAD_MARKER;
            d->payload = QByteArray((const char *)p + 1, payloadSize);
            return;
        }

        quint16 optionDelta = (*p & 0xf0) >> 4;
        quint16 optionLength = (*p & 0xf);
        p++;
        if (optionDelta == 13) {
            optionDelta = *p + 13;
            p++;
        } else if (optionDelta == 14) {
            optionDelta = endian_load16(quint16, p);
            p += 2;
        } else if (optionDelta == 15) {
            d->errors |= Error::WRONG_PAYLOAD_MARKER;
            return;
        }
        if (optionLength == 13) {
            optionLength = *p + 13;
            p++;
        } else if (optionLength == 14) {
            optionLength = endian_load16(quint16, p);
            p += 2;
        } else if (optionLength == 15) {
            d->errors |= Error::WRONG_PAYLOAD_MARKER;
            return;
        }

        optionNumber += optionDelta;
        CoapOption option((Coap::OptionType)optionNumber, QByteArray((const char *)p, optionLength));
        d->options.append(option);
        p += optionLength;
    } while (p <= pend);

    if (p > pend + 1)
        d->errors |= Error::NOT_ENOUGH_DATA;
}

CoapPDU::Errors CoapPDU::errors() const
{
    return d->errors;
}

bool CoapPDU::isValid() const
{
    return d->errors == Errors(0);
}
