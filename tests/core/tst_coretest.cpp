#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

class CoreTest : public QObject
{
    Q_OBJECT

public:
    CoreTest();

private Q_SLOTS:
    void testCoapTimerQueue();

private:

};

CoreTest::CoreTest()
{
}



void CoreTest::testCoapTimerQueue()
{

}

QTEST_MAIN(CoreTest)

#include "tst_coretest.moc"
