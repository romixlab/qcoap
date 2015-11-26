#include <QBasicTimer>
#include <QDateTime>
#include <QDebug>

#include "timerqueue.h"

typedef struct {
    QDateTime dt;
    QByteArray key;
} coap_timer_t;

class TimerQueuePrivate {
public:
    QBasicTimer timer;
    QList<coap_timer_t> queue;
};

TimerQueue::TimerQueue(QObject *parent) :
    QObject(parent), d(new TimerQueuePrivate)
{

}

TimerQueue::~TimerQueue()
{
    if (d) {
        delete d;
        d = 0;
    }
}

void TimerQueue::addTimer(quint32 msec, const QByteArray &key)
{
    QDateTime dt = QDateTime::currentDateTime().addMSecs(msec);
    int idx = 0;
    for (; idx < d->queue.size(); ++idx) {
        if (dt < d->queue[idx].dt)
            break;
        if (idx == d->queue.size() - 1)
            idx += 1; // after last one
    }
    coap_timer_t timer{dt, key};
    d->queue.insert(idx, timer);
    if (idx == 0)
        d->timer.start(msec, this);
}

void TimerQueue::removeTimer(const QByteArray &key)
{
    int i = 0;
    for (; i < d->queue.size(); ++i)
        if (d->queue[i].key == key)
            break;
    if (i == d->queue.size())
        return;
    if (i != 0) {
        d->queue.removeAt(i);
        return;
    }
    d->queue.pop_front();
    if (d->queue.isEmpty()) {
        d->timer.stop();
    } else {
        qint64 msec = QDateTime::currentDateTime().msecsTo(d->queue.front().dt);
        d->timer.start(msec, this);
    }
}

void TimerQueue::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    coap_timer_t &timer = d->queue.front();
    emit timeout(timer.key);
    d->timer.stop();
    d->queue.pop_front();
    if (!d->queue.isEmpty()) {
        qint64 msec = QDateTime::currentDateTime().msecsTo(d->queue.front().dt);
        d->timer.start(msec, this);
    }
}

