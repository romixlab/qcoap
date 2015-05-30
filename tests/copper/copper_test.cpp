#include <QtTest>
#include "coappdu.h"
#include <QDebug>

class CopperTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_get();
};

void CopperTest::test_get()
{
    CoapPDU pdu;

    QByteArray simpleGet = QByteArray::fromHex("4201fd21a4feb4746573741101");
    QByteArray token;
    QList<CoapOption> options;

    pdu.unpack(simpleGet);
    QVERIFY(pdu.isValid());
    QVERIFY(pdu.version() == 1);
    QVERIFY(pdu.type() == CoapPDU::Type::CONFIRMABLE);
    QVERIFY(pdu.code() == CoapPDU::Code::GET);
    QVERIFY(pdu.messageId() == 64801);
    token = pdu.token();
    QVERIFY(token.size() == 2);
    QVERIFY(token == "\xa4\xfe");
    options = pdu.options();
    QVERIFY(options.length() == 2);
    QVERIFY(options[0].number() == (quint16)CoapPDU::Option::URI_PATH);
    QVERIFY(options[0].data() == "test");
    QVERIFY(options[1].number() == (quint16)CoapPDU::Option::CONTENT_FORMAT);
    QVERIFY(options[1].data() == "\x01");
    QVERIFY(pdu.payload().isEmpty());

    QByteArray simpleGetAnswer = QByteArray::fromHex("6245fd23a4fec0211eff5479"
        "70653a20302028434f4e290a436f64653a20312028474554290a4d49443a203634383"
        "0330a546f6b656e3a2061346665");
    pdu.unpack(simpleGetAnswer);
    QVERIFY(pdu.isValid());
    QVERIFY(pdu.version() == 1);
    QVERIFY(pdu.type() == CoapPDU::Type::ACKNOWLEDGEMENT);
    QVERIFY(pdu.code() == CoapPDU::Code::CONTENT);
    QVERIFY(pdu.messageId() == 64803);
    token = pdu.token();
    QVERIFY(token.size() == 2);
    QVERIFY(token == "\xa4\xfe");
    options = pdu.options();
    QVERIFY(options.length() == 2);
    QVERIFY(options[0].number() == (quint16)CoapPDU::Option::CONTENT_FORMAT);
    QVERIFY(options[0].data().isEmpty());
    QVERIFY(options[1].number() == (quint16)CoapPDU::Option::MAX_AGE);
    QVERIFY(options[1].data() == "\x1e");
    QVERIFY(pdu.payload() == "Type: 0 (CON)\nCode: 1 (GET)\nMID: 64803\nToken: a4fe");
}

QTEST_APPLESS_MAIN(CopperTest)

#include "copper_test.moc"
