#include <QBasicTimer>
#include <QDateTime>
#include <QDebug>

#include "coaptimerqueue.h"

typedef struct {
    QDateTime dt;
    QObject *object;
    const char *method;
} coap_timer_t;

class CoapTimerQueuePrivate {
public:
    QBasicTimer timer;
    QList<coap_timer_t> queue;
};

CoapTimerQueue::CoapTimerQueue(QObject *parent) :
    QObject(parent), d(new CoapTimerQueuePrivate)
{

}

CoapTimerQueue::~CoapTimerQueue()
{
    if (d) {
        delete d;
        d = 0;
    }
}

void CoapTimerQueue::addTimer(quint32 msec, QObject *receiver, const char *method)
{
    QDateTime dt = QDateTime::currentDateTime().addMSecs(msec);
    int idx = 0;
    for (; idx < d->queue.size(); ++idx) {
        if (dt < d->queue[idx].dt)
            break;
        if (idx == d->queue.size() - 1)
            idx += 1; // after last one
    }
    coap_timer_t timer{dt, receiver, method};
    d->queue.insert(idx, timer);
    if (idx == 0)
        d->timer.start(msec, this);
}

void CoapTimerQueue::removeTimers(QObject *receiver)
{
    for (int i = 0; i < d->queue.size(); ++i)
        if (d->queue[i].object == receiver)
            d->queue[i].object = 0;
}

void CoapTimerQueue::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    coap_timer_t &timer = d->queue.front();
    if (timer.object)
        QMetaObject::invokeMethod(timer.object, timer.method);
    d->timer.stop();
    d->queue.pop_front();
    if (!d->queue.isEmpty()) {
        qint64 msec = QDateTime::currentDateTime().msecsTo(d->queue.front().dt);
        d->timer.start(msec, this);
    }
}

