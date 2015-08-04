#ifndef COAPTIMERQUEUE_H
#define COAPTIMERQUEUE_H

#include <QObject>

class CoapTimerQueuePrivate;
class CoapTimerQueue : public QObject
{
    Q_OBJECT
public:
    explicit CoapTimerQueue(QObject *parent = 0);
    ~CoapTimerQueue();


    /**
     * @brief addTimer adds timer with msec timeout after current time
     * @param msec
     */
    void addTimer(quint32 msec, quint8 id);

signals:

public slots:

protected:
    void timerEvent(QTimerEvent *e);

private:
    CoapTimerQueuePrivate *d;
};

#endif // COAPTIMERQUEUE_H
