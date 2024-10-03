#include "controllerfire.h"
#include <QDebug>

ControllerFire::ControllerFire(QVector3D vecPosFire, int strPriority, QObject *parent)
    : QObject(parent)
{
//    entity->setObjectName("fire"+QString::number(Counts));

    thread.setObjectName("threadFire");
    entity = new EntityEmitterFire(vecPosFire);
    entity->moveToThread(&thread);
//    qDebug()<<"FFF: "<<&thread;
    connect(&thread, &QThread::started, this, &ControllerFire::emtStarted);
    connect(this, &ControllerFire::goWork, entity, &EntityEmitterFire::createPartecal);
    connect(entity, &EntityEmitterFire::emtFinished, this, &ControllerFire::emtDeleted);
    connect(entity, &EntityEmitterFire::emtFinished, this, &ControllerFire::deleteLater);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }
    emit goWork();
}

ControllerFire::~ControllerFire()
{
    thread.quit();
    thread.wait();
    thread.deleteLater();
    entity->deleteLater();
}

void ControllerFire::emtStarted()
{
    emit emtCreateFire(entity);
}
void ControllerFire::emtDeleted()
{
    emit emtDeleterFire(entity, this);
}


