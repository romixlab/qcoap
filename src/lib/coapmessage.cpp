#include "coapmessage.h"
#include "endianhelper.h"

#include <QAtomicInt>
#include <QDebug>
#include <QMetaEnum>

CoapOption::CoapOption() : m_type((CoapMessage::OptionType)0) {}
CoapOption::CoapOption(CoapMessage::OptionType optionType, const QByteArray &data):
    m_type(optionType), m_data(data)
{}

CoapMessage::OptionType CoapOption::type() const
{
    Q_D(const CoapMessage);
    return m_type;
}

QByteArray CoapOption::data() const
{
    Q_D(const CoapMessage);
    return m_data;
}

bool CoapOption::isValid() const
{
    Q_D(const CoapMessage);
    return (int)m_type != 0;
}

bool CoapOption::operator ==(const CoapOption &other)
{
    return (m_type == other.m_type) && (m_data == other.m_data);
}

class CoapMessagePrivate
{
public:
    CoapMessagePrivate() :
        version(1),
        type(CoapMessage::Type::Reset),
        code(CoapMessage::Code::Empty),
        message_id(0)
    {
        // TODO   d->errors =
    }

    quint8 version;
    CoapMessage::Type type;
    CoapMessage::Code code;
    quint16 message_id;
    QByteArray token;
    QList<CoapOption> options;
    QByteArray payload;
    QHostAddress address;
    quint16 port;
    CoapMessage::Errors errors;
};

CoapMessage::CoapMessage()
    : d(new CoapMessagePrivate)
{

}

CoapMessage::~CoapMessage()
{
    delete d_ptr;
}

void CoapMessage::setVersion(quint8 version)
{
    Q_D(CoapMessage);
    Q_D(CoapMessage);
    d->version = version;
    if (version != 1)
        d->errors |= Error::WRONG_VERSION;
}

quint8 CoapMessage::version() const
{
    Q_D(const CoapMessage);
    return d->version;
}

void CoapMessage::setType(CoapMessage::Type type)
{
    Q_D(CoapMessage);
    if (d->type == type)
        return;
    d->type = type;
}

CoapMessage::Type CoapMessage::type() const
{
    Q_D(const CoapMessage);
    return d->type;
}

void CoapMessage::setCode(CoapMessage::Code code)
{
    Q_D(CoapMessage);
    if (d->code == code)
        return;
    d->code = code;
}

CoapMessage::Code CoapMessage::code() const
{
    Q_D(const CoapMessage);
    return d->code;
}

bool CoapMessage::isEmpty() const
{
    Q_D(const CoapMessage);
    return d->code == Code::Empty;
}

bool CoapMessage::isRequest() const
{
    Q_D(const CoapMessage);
    return ((quint8)d->code >= 0x01 && (quint8)d->code <= 0x04);
}

bool CoapMessage::isResponse() const
{
    Q_D(const CoapMessage);
    return (!isEmpty() && !isRequest());
}

void CoapMessage::setToken(const QByteArray &token)
{
    Q_D(CoapMessage);
    if (d->token == token)
        return;
    d->token = token;
    if (token.length() > 8)
        d->errors |= Error::WRONG_TOKEN;
}

void CoapMessage::setToken(const char *token, quint8 length)
{
    Q_D(CoapMessage);
    QByteArray t(token, length);
    if (d->token == t)
        return;
    d->token = t;
    if (length > 8)
        d->errors |= Error::WRONG_TOKEN;
}

QByteArray CoapMessage::token() const
{
    Q_D(const CoapMessage);
    return d->token;
}

void CoapMessage::setMessageId(quint16 id)
{
    Q_D(CoapMessage);
    if (d->message_id == id)
        return;
    d->message_id = id;
}

quint16 CoapMessage::messageId() const
{
    Q_D(const CoapMessage);
    return d->message_id;
}

void CoapMessage::addOption(CoapMessage::OptionType optionType, const QByteArray &value)
{
    Q_D(CoapMessage);
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

QList<CoapOption> CoapMessage::options() const
{
    Q_D(const CoapMessage);
    return d->options;
}

int CoapMessage::optionsCount() const
{
    Q_D(const CoapMessage);
    return d->options.length();
}

CoapOption CoapMessage::option(int idx) const
{
    Q_D(const CoapMessage);
    if (idx < 0 || idx >= d->options.length())
        return CoapOption();
    return d->options[idx];
}

void CoapMessage::setContentFormat(CoapMessage::ContentFormat format)
{
    Q_D(CoapMessage);
    CoapMessage::OptionType optionContentFormat = OptionType::ContentFormat;
    if (format == ContentFormat::TextPlain) {
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

void CoapMessage::setPayload(const QByteArray &payload)
{
    Q_D(CoapMessage);
    if (d->payload == payload)
        return;
    d->payload = payload;
}

QByteArray CoapMessage::payload() const
{
    Q_D(const CoapMessage);
    return d->payload;
}

void pack_option(quint8 *p, quint16 optionNumber, const QByteArray &value, bool write) const
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

QByteArray CoapMessage::pack() const
{
    Q_D(const CoapMessage);

//    const_cast<CoapMessagePrivate *>(d)->errors = Errors(0);
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

void CoapMessage::unpack(const QByteArray &packed)
{
    Q_D(CoapMessage);
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

    d->type = (CoapMessage::Type)( (p[0] & 0x30) >> 4 );
    quint8 tokenLength = (p[0] & 0xf);
    d->code = (CoapMessage::Code)p[1];
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
        CoapOption option((CoapMessage::OptionType)optionNumber, QByteArray((const char *)p, optionLength));
        d->options.append(option);
        p += optionLength;
    } while (p <= pend);

    if (p > pend + 1)
        d->errors |= Error::NOT_ENOUGH_DATA;
}

QHostAddress CoapMessage::address() const
{
    Q_D(const CoapMessage);
    return d->address;
}

void CoapMessage::setAddress(const QHostAddress &address)
{
    Q_D(CoapMessage);
    d->address = adress;
}

quint16 CoapMessage::port() const
{
    Q_D(const CoapMessage);
    return d->port;
}

void CoapMessage::setPort(quint16 port)
{
    Q_D(CoapMessage);
    d->port = port;
}

CoapMessage::Errors CoapMessage::errors() const
{
    Q_D(const CoapMessage);
    return d->errors;
}

bool CoapMessage::isValid() const
{
    Q_D(const CoapMessage);
    return d->errors == Errors(0);
}


QDebug operator<<(QDebug debug, const CoapMessage &pdu)
{
    QDebugStateSaver saver(debug);
    debug.nospace();
    if (!pdu.isValid()) {
        debug << "CoapMessage(Invalid)";
        return debug;
    }
    int typeEnum = pdu.staticMetaObject.indexOfEnumerator("Type");
    int codeEnum = pdu.staticMetaObject.indexOfEnumerator("Code");
    int optionTypeEnum = pdu.staticMetaObject.indexOfEnumerator("OptionType");
    int contentFormatEnum = pdu.staticMetaObject.indexOfEnumerator("ContentFormat");
    debug << "CoapMessage(" << pdu.staticMetaObject.enumerator(typeEnum).key((int)pdu.type());
    debug << pdu.staticMetaObject.enumerator(codeEnum).key((int)pdu.code()) << " ";
    debug << pdu.staticMetaObject.enumerator(1).key(123) << " ";
    debug << "Token:" << pdu.token().toHex() << " MID:" << pdu.messageId() << " ";

//    for (int i = 0; i < pdu.optionsCount(); ++i) {
//        QDebug dbg(QtDebugMsg);
//        dbg.nospace();
//        dbg << "Option:" << i << ", ";
//        CoapOption option = pdu.option(i);
//        dbg << "isValid:" << option.isValid() << ", ";
//        if (!option.isValid())
//            continue;
//        dbg << "Number:" << (int)option.type() << " - " << Coap::toString(option.type()) << ", ";
//        dbg << "Data:" << option.data().toHex();
//        //        if (option.type() == Coap::OptionType:: || option.type() == 3)
//        dbg << " asString: " << option.data();
//    }
    if (pdu.payload().isEmpty())
        debug << "No payload ";
    else
        debug << "Payload:" << pdu.payload().toHex() << " asString:" << pdu.payload();

    debug << ")";
    return debug;
}
