#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <functional>

#include <QObject>

class TimerQueuePrivate;
class TimerQueue : public QObject
{
    Q_OBJECT
public:
    explicit TimerQueue(QObject *parent = 0);
    ~TimerQueue();


    /**
     * @brief addTimer adds timer with msec timeout after current time
     * @param msec
     */
    void addTimer(quint32 msec, const QByteArray &key);
    void removeTimer(const QByteArray &key);

signals:
    void timeout(const QByteArray &key);

public slots:

protected:
    void timerEvent(QTimerEvent *e);

private:
    TimerQueuePrivate *d;
};

#endif // TIMERQUEUE_H
