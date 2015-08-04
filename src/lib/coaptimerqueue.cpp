#include <QBasicTimer>
#include <QDateTime>
#include <QDebug>

#include "coaptimerqueue.h"

typedef struct {
    QDateTime dt;
    quint8 id;
} coap_timer_t;

class CoapTimerQueuePrivate {
public:
    QBasicTimer timer;
    QList<coap_timer_t> queue;
    quint32 delta;
};

CoapTimerQueue::CoapTimerQueue(QObject *parent) :
    QObject(parent), d(new CoapTimerQueuePrivate)
{
    d->delta = 100;
}

CoapTimerQueue::~CoapTimerQueue()
{
    if (d) {
        delete d;
        d = 0;
    }
}

void CoapTimerQueue::addTimer(quint32 msec, quint8 id)
{
    QDateTime dt = QDateTime::currentDateTime().addMSecs(msec);
    int idx = 0;
    for (; idx < d->queue.size(); ++idx) {
        if (dt < d->queue[idx].dt)
            break;
        if (idx == d->queue.size() - 1)
            idx += 1; // after last one
    }
    coap_timer_t timer{dt, id};
    d->queue.insert(idx, timer);
    if (idx == 0)
        d->timer.start(msec, this);
}

void CoapTimerQueue::timerEvent(QTimerEvent *e)
{
    qDebug() << "timeout" << d->queue.front().id;
    d->timer.stop();
    d->queue.pop_front();
    if (!d->queue.isEmpty()) {
        qint64 msec = QDateTime::currentDateTime().msecsTo(d->queue.front().dt);
        d->timer.start(msec, this);
    }
}

