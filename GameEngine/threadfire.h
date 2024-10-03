#ifndef THREADFIRE_H
#define THREADFIRE_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "entityemitterfire.h"
#include <QDebug>
#include <QMutex>

class EntityEmitterFire;

class ThreadFire : public QThread
{
    Q_OBJECT
public:
    ThreadFire(EntityEmitterFire *entitysEmitter);

    void run() override {
        mMutex.lock();
        qDebug()<<"BBBB"<<entityEmitter;
        mMutex.unlock();
    }
signals:
    void emtDeleteEmitterFire(EntityEmitterFire*);

private slots:
    void timers();

private:
    EntityEmitterFire *entityEmitter;
    QMutex mMutex;

};

#endif // THREADFIRE_H
