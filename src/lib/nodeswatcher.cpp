#include "nodeswatcher.h"

#include "coapexchange.h"

#include <QTimer>

class NodesWatcherPrivate
{
    Q_DECLARE_PUBLIC(NodesWatcher)
public:
    NodesWatcherPrivate(NodesWatcher *q) :
        q_ptr(q), interval(2000), queueIndex(0)
    {
        exchange = new CoapExchange(q);
        QObject::connect(exchange, &CoapExchange::completed, q, &NodesWatcher::onExchangeCompleted);

        timer = new QTimer(q);
        timer->setSingleShot(true);
        QObject::connect(timer, &QTimer::timeout, q, &NodesWatcher::onTimerTimeout);
    }

    NodesWatcher *q_ptr;
    CoapExchange *exchange;
    QTimer *timer;
    quint32 interval;
    QList<QString> queue;
    int queueIndex;
};

NodesWatcher::NodesWatcher(QObject *parent) :
    QObject(parent)
{
    d_ptr = new NodesWatcherPrivate(this);
}

NodesWatcher::~NodesWatcher()
{
    delete d_ptr;
}

quint32 NodesWatcher::watchInterval() const
{
    Q_D(const NodesWatcher);
    return d->interval;
}

void NodesWatcher::setWatchInterval(quint32 interval)
{
    Q_D(NodesWatcher);
    d->interval = interval;
    emit watchIntervalChanged();
}

void NodesWatcher::watch(const QString &uriString)
{
    Q_D(NodesWatcher);
    if (!d->queue.contains(uriString))
        d->queue.append(uriString);
    if (!d->timer->isActive())
        d->timer->start(d->interval);
}

void NodesWatcher::forget(const QString &uriString)
{
    Q_D(NodesWatcher);
    d->queue.removeOne(uriString);
    if (d->queue.isEmpty())
        d->timer->stop();
}

void NodesWatcher::onTimerTimeout()
{
    Q_D(NodesWatcher);
    d->exchange->setUriString(d->queue.at(d->queueIndex));
    d->exchange->get();
}

void NodesWatcher::onExchangeCompleted()
{
    Q_D(NodesWatcher);
    qInfo() << "NodesWatcher:" << d->queue.at(d->queueIndex) << "is online";
    if (++d->queueIndex == d->queue.size())
        d->queueIndex = 0;
    d->timer->start(d->interval);
}

