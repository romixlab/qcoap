#include "coappdu.h"
#include <QAtomicInt>
#include "sysdep.h"

#include <QDebug>

CoapOption::CoapOption() {}
CoapOption::CoapOption(quint16 number, const QByteArray &data):
    m_number(number), m_data(data)
{}

quint16 CoapOption::number() const
{
    return m_number;
}

QByteArray CoapOption::data() const
{
    return m_data;
}

bool CoapOption::operator ==(const CoapOption &other)
{
    return (m_number == other.m_number) && (m_data == other.m_data);
}

struct CoapPDUPrivate {
    QAtomicInt ref;

    quint8 version;
    CoapPDU::Type type;
    CoapPDU::Code code;
    quint16 message_id;
    QByteArray token;
    QList<CoapOption> options;
    QByteArray payload;

    CoapPDU::Errors errors;
};

CoapPDU::CoapPDU() : d_ptr(new CoapPDUPrivate)
{
    Q_D(CoapPDU);
    d->ref.ref();
    d->version = 1;
    d->type = Type::RESET;
    d->code = Code::EMPTY;
    d->message_id = 0;
}

CoapPDU::CoapPDU(const CoapPDU &other)
{
    d_ptr = other.d_ptr;
    d_ptr->ref.ref();
}

CoapPDU::~CoapPDU()
{
    if (!d_ptr->ref.deref()) {
        delete d_ptr;
        d_ptr = 0;
    }
}

void CoapPDU::detach()
{
    if (d_ptr->ref.load() == 1)
        return;

    if (!d_ptr->ref.deref())
        delete d_ptr;

    CoapPDUPrivate *new_dptr = new CoapPDUPrivate;
    new_dptr->ref.ref();
    new_dptr->version       = d_ptr->version;
    new_dptr->type          = d_ptr->type;
    new_dptr->code          = d_ptr->code;
    new_dptr->message_id    = d_ptr->message_id;
    new_dptr->token         = d_ptr->token;
    new_dptr->options       = d_ptr->options;
    new_dptr->payload       = d_ptr->payload;
    new_dptr->errors        = d_ptr->errors;
    d_ptr = new_dptr;
}

void CoapPDU::setVersion(quint8 version)
{
    Q_D(CoapPDU);
    if (d->version == version)
        return;
    detach();
    d->version = version;
    if (version != 1)
        d->errors |= Error::WRONG_VERSION;
}

quint8 CoapPDU::version() const
{
    Q_D(const CoapPDU);
    return d->version;
}

void CoapPDU::setType(CoapPDU::Type type)
{
    Q_D(CoapPDU);
    if (d->type == type)
        return;
    detach();
    d->type = type;
}

CoapPDU::Type CoapPDU::type() const
{
    Q_D(const CoapPDU);
    return d->type;
}

void CoapPDU::setCode(CoapPDU::Code code)
{
    Q_D(CoapPDU);
    if (d->code == code)
        return;
    detach();
    d->code = code;
}

CoapPDU::Code CoapPDU::code() const
{
    Q_D(const CoapPDU);
    return d->code;
}

void CoapPDU::setToken(const QByteArray &token)
{
    Q_D(CoapPDU);
    if (d->token == token)
        return;
    detach();
    d->token = token;
    if (token.length() > 8)
        d->errors |= Error::WRONG_TOKEN;
}

void CoapPDU::setToken(const char *token, quint8 length)
{
    Q_D(CoapPDU);
    QByteArray t(token, length);
    if (d->token == t)
        return;
    detach();
    d->token = t;
    if (length > 8)
        d->errors |= Error::WRONG_TOKEN;
}

QByteArray CoapPDU::token() const
{
    Q_D(const CoapPDU);
    return d->token;
}

void CoapPDU::setMessageId(quint16 id)
{
    Q_D(CoapPDU);
    if (d->message_id == id)
        return;
    detach();
    d->message_id = id;
}

quint16 CoapPDU::messageId() const
{
    Q_D(const CoapPDU);
    return d->message_id;
}

void CoapPDU::addOption(quint16 optionNumber, const QByteArray &value)
{
    Q_D(CoapPDU);
    int idx = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        if (optionNumber > d->options[i].number())
            idx++;
    }
    CoapOption option(optionNumber, value);
    if (d->options.contains(option))
        return;
    detach();
    d->options.insert(idx, option);
}

QList<CoapOption> CoapPDU::options() const
{
    Q_D(const CoapPDU);
    return d->options;
}

void CoapPDU::setContentFormat(CoapPDU::ContentFormat format)
{
    quint16 optionContentFormat = (quint16)Option::CONTENT_FORMAT;
    if (format == ContentFormat::TEXT_PLAIN) {
        addOption(optionContentFormat, QByteArray());
    } else if ((quint16)format < 256) {
        quint8 f = (quint8)format;
        addOption(optionContentFormat, QByteArray((const char*)&f, 1));
    } else {
        quint16 f = (quint16)format;
        QByteArray data;
        data.resize(2);
        _msgpack_store16(data.data(), f);
        addOption(optionContentFormat, data);
    }
}

void CoapPDU::setPayload(const QByteArray &payload)
{
    Q_D(CoapPDU);
    if (d->payload == payload)
        return;
    detach();
    d->payload = payload;
}

QByteArray CoapPDU::payload() const
{
    Q_D(const CoapPDU);
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
    Q_D(const CoapPDU);
//    const_cast<CoapPDUPrivate *>(d)->errors = Errors(0);
    quint32 pduSize = 4; // header
    pduSize += d->token.length();
    quint8 *p = 0;
    quint16 optionDelta = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        quint16 optionNumber = d->options[i].number() - optionDelta;
        p = pack_option(p, optionNumber, d->options[i].data(), false);
        optionDelta = optionNumber;
    }
    pduSize += p - (quint8 *)0;
//    for (int i = 0; i < d->options.length(); ++i) {
//        qDebug() << "option" << d->options[i].first << d->options[i].second.toHex();
//    }
//    qDebug() << "options size" << (p - (quint8 *)0);
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
    _msgpack_store16((p + 2), d->message_id);
    p += 4;
    memcpy(p, d->token.data(), d->token.length());
    p += d->token.length();
    optionDelta = 0;
    for (int i = 0; i < d->options.length(); ++i) {
        quint16 optionNumber = d->options[i].number() - optionDelta;
        p = pack_option(p, optionNumber, d->options[i].data(), true);
        optionDelta = optionNumber;
    }
    *(p++) = 0xff;
    memcpy(p, d->payload, d->payload.length());

    return packed;
}

void CoapPDU::unpack(const QByteArray &packed)
{
    Q_D(CoapPDU);
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

    d->type = (Type)( (p[0] & 0x30) >> 4 );
    quint8 tokenLength = (p[0] & 0xf);
    d->code = (Code)p[1];
    if (tokenLength > 8)
        d->errors |= Error::WRONG_TOKEN_LENGTH;
    else
        d->token = QByteArray((const char *)p + 4, tokenLength);

    d->message_id = _msgpack_load16(quint16, (p + 2));

    if (packed.size() == 4) // only header present, ok
        return;

    p += tokenLength;
    p += 4;

    quint16 optionNumber = 0;
    do {
        if (*p == 0xff) {
            quint32 payloadSize = pend - p;
            qDebug() << payloadSize;
            if (payloadSize == 0)
                d->errors |= Error::WRONG_PAYLOAD_MARKER;
            d->payload = QByteArray((const char *)p + 1, payloadSize);
            return;
        }

        quint16 optionDelta = (*p & 0xf0) >> 4;
        quint16 optionLength = (*p & 0xf);
//        qDebug() << "optoinDelta" << optionDelta;
//        //qDebug() << "optionNumber" << optionNumber;
//        qDebug() << "optionLength" << optionLength;
        p++;
        if (optionDelta == 13) {
            optionDelta = *p + 13;
            p++;
        } else if (optionDelta == 14) {
            optionDelta = _msgpack_load16(quint16, p);
            p += 2;
        } else if (optionDelta == 15) {
            d->errors |= Error::WRONG_PAYLOAD_MARKER;
            return;
        }
        if (optionLength == 13) {
            optionLength = *p + 13;
            p++;
        } else if (optionLength == 14) {
            optionLength = _msgpack_load16(quint16, p);
            p += 2;
        } else if (optionLength == 15) {
            d->errors |= Error::WRONG_PAYLOAD_MARKER;
            return;
        }

        optionNumber += optionDelta;
        CoapOption option(optionNumber, QByteArray((const char *)p, optionLength));
        d->options.append(option);
        p += optionLength;
    } while (p <= pend);

    if (p > pend + 1)
        d->errors |= Error::NOT_ENOUGH_DATA;
}

CoapPDU::Errors CoapPDU::errors() const
{
    Q_D(const CoapPDU);
    return d->errors;
}

bool CoapPDU::isValid() const
{
    Q_D(const CoapPDU);
    return d->errors == Errors(0);
}

