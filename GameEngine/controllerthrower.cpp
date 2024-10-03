#include "controllerthrower.h"
#include "modelrobot.h"
#include "modelcar.h"
#include "modelplane.h"
#include "entitywedge.h"
#include "entity.h"
#include "modelerth.h"

ControllerThrower::ControllerThrower(QObject *parent) : QObject(parent)
{
    wedge = new EntityWedge;
    thread.setObjectName("thread Rocket NEW");
    wedge->moveToThread(&thread);

    connect(&thread, &QThread::finished, wedge, &EntityWedge::deleteLater);
    connect(this, &ControllerThrower::goWork, wedge, &EntityWedge::goWedge);
    connect(wedge, &EntityWedge::emtUpdate, this, &ControllerThrower::slotUpdate);
    connect(wedge, &EntityWedge::emtDeleteWedge, this, &ControllerThrower::slotDelete);
    connect(this, &ControllerThrower::settingWork, wedge, &EntityWedge::settingWedge);
    connect(wedge, &EntityWedge::emtCollisionThat, this, &ControllerThrower::slotCollisionThat);

    thread.start();
    emit settingWork();
}

ControllerThrower::~ControllerThrower()
{
    qDebug("Destry");
    thread.quit();
    thread.wait();
    thread.destroyed();
    thread.destroyed();
//    delete wedge;
//    thread.deleteLater();
//    wedge->deleteLater();
}

EntityWedge *ControllerThrower::getEntityWedge()
{
    return wedge;
}

void ControllerThrower::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome, ModelRobot *modelHuman, ModelCar *modelCar,
                                           ModelPlane *modelPlane, ModelHumanH *modelCollada)
{
    wedge->allEntityCollision(erth, listHome, modelCar, modelPlane, modelHuman);
}

void ControllerThrower::runWedge()
{
    emit goWork();
}

void ControllerThrower::slotUpdate()
{
    emit emtUpdate();
}

void ControllerThrower::slotDelete(EntityWedge *thisWedge)
{
    emit emtDeleteWedge(this);
}

void ControllerThrower::slotCollisionThat(QString name, QVector3D *posColl)
{
    emit emtCollisionThat(name,posColl);
}

