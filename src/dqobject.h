#ifndef DQOBJECT_H
#define DQOBJECT_H

#include <QObject>

class DQObjectPrivate;

class DQObject : public QObject
{
public:
    explicit DQObject(QObject *parent = 0);
    ~DQObject();

    /**
     * @brief addSlot create virtual slot
     * @param slot use SLOT(signature)
     * @return true if slot created
     * Usage:
     * connect(dqobject, SIGNAL(catched(QString,QVariantList)),
     *      reciever, SLOT(processSignal(QString,QVariantList)));
     * dqobject->addSlot("my_slot(int)");
     * connect(sender, SIGNAL(sig(int)), dqobject, SLOT(my_slot(int)));
     * when sender->sig will be emitted, dqobject will emit catched(name, args)
     * signal, with name = 'my_slot(int)', args = QVariantList
     */
    bool addSlot(const QString &slot);
    /**
     * @brief removeSlot removes previously added slot
     * @param slot SLOT(signature)
     * @return true if removed
     * note that, when you remove the slot, connections to it still exists!
     * use dqobject->connect() instead QObject::connect() or disconnect manually
     */
    bool removeSlot(const QString &slot);
    /**
     * @brief addSignal
     * @param signal use SIGNAL()
     * @return true if signal added
     * Usage:
     * SignalTrap st;
     * dqobject->addSignal(SIGNAL(sig(int,QString)));
     * QObject::connect(dqobject, SIGNAL(sig(int,QString)), &st, SLOT(sl(int,QString)));
     * dqobject->activateSignal("sig", QVariantList() << 12.3 << 123);
     */
    bool addSignal(const QString &signal);
    /**
     * @brief activateSignal activate's previously added signal
     * @param signal name of signal
     * @param args arguments (if types differ QVariant::convert will be used)
     */
    void activateSignal(const QString &signal, const QVariantList &args);
    /**
     * @brief removeSignal remove previously added signal
     * @param signal name used when addSignal() called
     * @return true if signal removed
     * note that, when you remove the signal, connections to it still exists!
     * use dqobject->connect() instead QObject::connect() or disconnect manually
     */
    bool removeSignal(const QString &signal);

//    bool connect(const QObject *sender, const char *signal,
//                 const QObject *receiver, const char *member,
//                 Qt::ConnectionType type = Qt::AutoConnection);

    virtual const QMetaObject * metaObject() const;
    int qt_metacall(QMetaObject::Call call, int id, void **args);
    virtual void *qt_metacast(const char *className);

private:
//    void disconnect_signal(const QString &signal);
    DQObjectPrivate *d;
};

#endif // DQOBJECT_H
