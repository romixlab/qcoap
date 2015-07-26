#ifndef COAPTIMERQUEUE_H
#define COAPTIMERQUEUE_H

#include <QObject>
#include <QBasicTimer>

class CoapTimerQueue : public QObject
{
    Q_OBJECT
public:
    explicit CoapTimerQueue(QObject *parent = 0);
    ~CoapTimerQueue();

signals:

public slots:

protected:
    void timerEvent(QTimerEvent *e);

private:
    QBasicTimer timer;
};

#endif // COAPTIMERQUEUE_H
