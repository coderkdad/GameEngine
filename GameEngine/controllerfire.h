#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include "entityemitterfire.h"
#include <QMap>

class ControllerFire : public QObject
{
    Q_OBJECT
public:
    explicit ControllerFire(QVector3D vecPosFire, int strPriority=1, QObject *parent = nullptr);
    ~ControllerFire();

signals:
    void goWork();
    void emtCreateFire(EntityEmitterFire *);
    void emtDeleterFire(EntityEmitterFire *, ControllerFire *);

public slots:
    void emtDeleted();
    void emtStarted();

private:
    QThread thread ;
    EntityEmitterFire *entity;

};

#endif // CONTROLLER_H
