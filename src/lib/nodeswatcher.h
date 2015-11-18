#ifndef NODESWATCHER_H
#define NODESWATCHER_H

#include "coaplib_global.h"

#include <QObject>

class NodesWatcherPrivate;
class COAPLIB_SHARED_EXPORT NodesWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 watchInterval READ watchInterval WRITE setWatchInterval NOTIFY watchIntervalChanged)
public:
    explicit NodesWatcher(QObject *parent = 0);
    ~NodesWatcher();

    quint32 watchInterval() const;
    void setWatchInterval(quint32 interval);

signals:
    //void nodeAlive(const CoapNodeInfo &nodeInfo);
    //void nodeLeft(const CoapNodeInfo &info);
    void watchIntervalChanged();

public slots:
    void watch(const QString &uriString);
    void forget(const QString &uriString);

private slots:
    void onTimerTimeout();
    void onExchangeCompleted();
private:
    NodesWatcherPrivate *d_ptr;
    Q_DECLARE_PRIVATE(NodesWatcher)
};

#endif // NODESWATCHER_H
