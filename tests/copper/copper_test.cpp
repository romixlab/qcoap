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

    QByteArray simpleGet = QByteArray::fromHex("40016633b474657374c102");
    QByteArray token;
    QList<CoapOption> options;

    pdu.unpack(simpleGet);
    QVERIFY(pdu.isValid());
    QVERIFY(pdu.version() == 1);
    QVERIFY(pdu.type() == CoapPDU::Type::CONFIRMABLE);
    QVERIFY(pdu.code() == CoapPDU::Code::GET);
    QVERIFY(pdu.messageId() == 26163);
    QVERIFY(pdu.token().size() == 0);
    options = pdu.options();
    QVERIFY(options[0].number() == (quint16)CoapPDU::Option::URI_PATH);
    QVERIFY(options[0].data() == "test");
    QVERIFY(options[1].number() == (quint16)CoapPDU::Option::BLOCK2);
    QVERIFY(options[1].data() == "\02");
}

QTEST_APPLESS_MAIN(CopperTest)

#include "copper_test.moc"
