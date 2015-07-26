#include "coaptimerqueue.h"
#include <QDebug>

CoapTimerQueue::CoapTimerQueue(QObject *parent) : QObject(parent)
{
    timer.start(500, this);
}

CoapTimerQueue::~CoapTimerQueue()
{

}

void CoapTimerQueue::timerEvent(QTimerEvent *e)
{
    qDebug() << "timer";
}

