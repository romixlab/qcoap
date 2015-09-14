#include <QCoreApplication>

#include <coaptimerqueue.h>

#include <QDateTime>
#include <QDebug>

CoapTimerQueue tq;
QTime startTime;
const int eps = 150;

void check(const QString &timerName, int shouldBe)
{
    int dt = startTime.msecsTo(QTime::currentTime());
    bool ok = qAbs(dt - shouldBe) < eps;
    if (ok)
        qDebug() << timerName << ": ok";
    else
        qDebug() << timerName << ": failed, should be" << shouldBe << "ms from start, real" << dt << "diff" << qAbs(dt - shouldBe) << QTime::currentTime();
}

void timer4test(quint8) {
    check("4", 2000);
    tq.addTimer(2000, 0, [](quint8){ check("5", 4000); });
}

void timer1test(quint8) {
    check("1", 1000);

    tq.addTimer(1000, 0, timer4test);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    startTime = QTime::currentTime();
    qDebug() << "Starting test at:" << QTime::currentTime();

    /**
     *    0s     1s      2s      3s      4s      5s      6s      7s
     * N  .......|.......|.......|.......|.......|.......|.......|
     * 1: x------>
     * 4:        x------->
     * 5:                x--------------->
     * 2: x---------------------->
     * 3: x------------------------------------------------------>
     *
     */
    tq.addTimer(1000, 0, timer1test);

    tq.addTimer(3000, 0, [=](quint8){
        check("2", 3000);
    });
    tq.addTimer(7000, 0, [=](quint8){
        check("3", 7000);
        exit(0);
    });

    return a.exec();
}
