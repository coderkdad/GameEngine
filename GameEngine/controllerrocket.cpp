#include "controllerrocket.h"
#include <entityrocket.h>
#include <entity.h>
#include <modelcar.h>
#include <modelrobot.h>
#include <modelplane.h>
#include <modelerth.h>

ControllerRocket::ControllerRocket(QObject *parent) : QObject(parent)
{
    rocket = new EntityRocket;
    thread.setObjectName("thread Rocket NEW");
    rocket->moveToThread(&thread);

    connect(&thread, &QThread::finished, rocket, &EntityRocket::deleteLater);
    connect(this, &ControllerRocket::goWork, rocket, &EntityRocket::goBullet);
    connect(rocket, &EntityRocket::emtUpdate, this, &ControllerRocket::slotUpdate);
    connect(rocket, &EntityRocket::emtDeleteRocket, this, &ControllerRocket::slotDelete);

    thread.start();
}

ControllerRocket::~ControllerRocket()
{
    qDebug("RRRRRR");
    thread.quit();
    thread.wait();
    thread.destroyed();
    qDebug("RRRRRR");
    thread.destroyed();
//    rocket->destroyed();
    qDebug()<<"Rock AA: ";
}

EntityRocket *ControllerRocket::getEntityRocket()
{
    return rocket;
}

void ControllerRocket::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                                          ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada)
{
    rocket->allEntityCollision(erth, listHome, modelCar, modelPlane, modelRobot, modelCollada);
    emit goWork();
}

void ControllerRocket::slotUpdate()
{
    emit emtUpdate();
}

void ControllerRocket::slotDelete(EntityRocket *thisRocket)
{
//    thisRocket->deleteLater();
    emit emtDeleteRocket(this);
}
