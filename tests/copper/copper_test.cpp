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

    QByteArray simpleGet = QByteArray::fromHex("42013fd5a4feb474657374c102");
    QByteArray token;

    pdu.unpack(simpleGet);
    qDebug() << pdu.errors();
    QVERIFY(pdu.isValid());
    QVERIFY(pdu.version() == 1);
    QVERIFY(pdu.type() == CoapPDU::Type::CONFIRMABLE);
    QVERIFY(pdu.code() == CoapPDU::Code::GET);
    QVERIFY(pdu.messageId() == 16341);
    token = pdu.token();
    QVERIFY((quint8)token.data()[0] == 0xa4);
    QVERIFY((quint8)token.data()[1] == 0xfe);
}

QTEST_APPLESS_MAIN(CopperTest)

#include "copper_test.moc"
