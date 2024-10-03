#include "controllercollision.h"
#include "collision.h"
#include <qmath.h>
#include "math.h"
#include <QDebug>
#include <QTimer>
#include "vaoentity.h"
#include "entityrocket.h"
#include "entity.h"
#include "modelcar.h"
#include "modelplane.h"
#include "modelrobot.h"
#include "modelerth.h"
#include "modelhome.h"


ControllerCollision::ControllerCollision()
{
}
ControllerCollision::~ControllerCollision()
{
    thread.quit();
    thread.wait();
    thread.deleteLater();
    col->deleteLater();
}

void ControllerCollision::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                                             ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada)
{
    m_erth = erth;
    m_listHome = listHome;
    m_modelCar = modelCar;
    m_modelRobot = modelRobot;
    m_modelCollada = modelCollada;
    m_modelPlane = modelPlane;
}

void ControllerCollision::stopControl()
{
//    qDebug("emt Ro");
    emit goWork(false);
}

void ControllerCollision::startControl()
{
    emit goWork(true);
}
void ControllerCollision::stateControl(bool state)
{
//    qDebug()<<"Stat "<<col->objectName()<<":"<<state;
    emit goWork(state);
}
void ControllerCollision::stateControl2(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
    emit goWork2(posX,posY,posZ, rotX, rotY, rotZ);
}

void ControllerCollision::setStateGrv(bool state)
{
    stateGrav = state;
    col->setStateGravy(state);
}
///=========== Collision Plane ===========///

void ControllerCollision::startColPlane(int strPriority)
{
    col = new Collision();
    col->setObjectName("colPlane");
    col->allEntityCollision(m_erth,m_listHome, m_modelCar, m_modelPlane, m_modelRobot, m_modelCollada);
    col->moveToThread(&thread);

//    connect(this, &ControllerCollision::goWork, col, &Collision::workCollisionPlane);
    connect(this, &ControllerCollision::goWork2, col, &Collision::slotTimerPlane);
    connect(col,  &Collision::emtCollPlaneWithErth,  this, &ControllerCollision::slotCollPlaneWithErth);
    connect(col,  &Collision::emtCollPlaneWithHome,  this, &ControllerCollision::slotCollPlaneWithHome);
    connect(col,  &Collision::emtCollPlaneWithCar,   this, &ControllerCollision::slotCollPlaneWithCar);
    connect(col,  &Collision::emtCollPlaneWithHuman, this, &ControllerCollision::slotCollPlaneWithHuman);
    connect(col,  &Collision::emtFinished,           this, &ControllerCollision::deleteLater);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }

//    emit goWork(true);

}

void ControllerCollision::slotCollPlaneWithErth(ModelPlane *p, Entity *e)
{
    emit emtConCollPlaneErth(p, e, this);
}
void ControllerCollision::slotCollPlaneWithHome(ModelPlane *p, Entity *h)
{
    emit emtConCollPlaneHome(p, h, this);
}
void ControllerCollision::slotCollPlaneWithCar(ModelPlane *p, ModelCar *car)
{
    emit emtConCollPlaneCar(p, car, this);
}
void ControllerCollision::slotCollPlaneWithHuman(ModelPlane *p, ModelRobot *human)
{
    emit emtConCollPlaneHuman(p, human, this);
}

///=========== Collision Car ===========///

void ControllerCollision::startColCar(int strPriority)
{
    thread.setObjectName("collCar");
    col = new Collision();
    col->moveToThread(&thread);
    col->setObjectName("colCar");
//    qDebug()<<"ReName"<<&thread;
    col->allEntityCollision(m_erth,m_listHome, m_modelCar, m_modelPlane, m_modelRobot, m_modelCollada);


//    connect(this, &ControllerCollision::goWork, col, &Collision::workCollisionCar);
    connect(this, &ControllerCollision::goWork2, col, &Collision::slotTimerCar);
    connect(col,  &Collision::emtCollCarWithErth,  this, &ControllerCollision::slotCollCarWithErth);
    connect(col,  &Collision::emtCollCarWithHome,  this, &ControllerCollision::slotCollCarWithHome);
    connect(col,  &Collision::emtCollCarWithPlane,   this, &ControllerCollision::slotCollCarWithPlane);
    connect(col,  &Collision::emtCollCarWithHuman, this, &ControllerCollision::slotCollCarWithHuman);
    connect(col,  &Collision::emtFinished,           this, &ControllerCollision::deleteLater);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }
}

void ControllerCollision::slotCollCarWithErth(ModelCar *c, Entity *e)
{
    emit emtConCollCarErth(c, e, this);
}
void ControllerCollision::slotCollCarWithHome(ModelCar *c, Entity *h)
{
    emit emtConCollCarHome(c, h, this);
}
void ControllerCollision::slotCollCarWithPlane(ModelCar *c, ModelPlane *plane)
{
    emit emtConCollCarPlane(c, plane, this);
}
void ControllerCollision::slotCollCarWithHuman(ModelCar *c, ModelRobot *human)
{
    emit emtConCollCarHuman(c, human, this);
}

///=========== Collision Human ===========///

void ControllerCollision::startColHuman(int strPriority)
{
    thread.setObjectName("collHuman Thread");
    col = new Collision();
    col->setObjectName("colHuman");
    col->allEntityCollision(m_erth,m_listHome, m_modelCar, m_modelPlane, m_modelRobot, m_modelCollada);
    col->moveToThread(&thread);
//    qDebug()<<"NameTHRED : "<<thread.objectName();

//    connect(this, &ControllerCollision::goWork, col, &Collision::workCollisionHuman);
    connect(this, &ControllerCollision::goWork2, col, &Collision::slotTimerHuman);
    connect(col,  &Collision::emtCollHumanWithErth,  this, &ControllerCollision::slotCollHumanWithErth);
    connect(col,  &Collision::emtCollHumanWithHome,  this, &ControllerCollision::slotCollHumanWithHome);
    connect(col,  &Collision::emtCollHumanWithPlane,   this, &ControllerCollision::slotCollHumanWithPlane);
    connect(col,  &Collision::emtCollHumanWithCar, this, &ControllerCollision::slotCollHumanWithCar);
    connect(col,  &Collision::emtFinished,           this, &ControllerCollision::deleteLater);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }
}


void ControllerCollision::startColCollada(int strPriority)
{
    thread.setObjectName("collCollada Thread");
    col = new Collision();
    col->setObjectName("colCollada");
    col->allEntityCollision(m_erth,m_listHome, m_modelCar, m_modelPlane, m_modelRobot, m_modelCollada);
    col->moveToThread(&thread);
//    qDebug()<<"NameTHRED : "<<thread.objectName();

//    connect(this, &ControllerCollision::goWork, col, &Collision::workCollisionHuman);
    connect(this, &ControllerCollision::goWork2, col, &Collision::slotTimerCollada);
    connect(col,  &Collision::emtCollisionCollada,  this, &ControllerCollision::slotColCollada);
    connect(col,  &Collision::emtNotCollisionCollada,  this, &ControllerCollision::slotNotCollCollada);
//    connect(col,  &Collision::emtCollHumanWithPlane,   this, &ControllerCollision::slotCollHumanWithPlane);
//    connect(col,  &Collision::emtCollHumanWithCar, this, &ControllerCollision::slotCollHumanWithCar);
    connect(col,  &Collision::emtFinished,           this, &ControllerCollision::deleteLater);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }
}


void ControllerCollision::slotCollHumanWithErth(ModelRobot *h, Entity *e)
{
    emit emtConCollHumanErth(h, e, this);
}
void ControllerCollision::slotCollHumanWithHome(ModelRobot *h, Entity *home)
{   
    emit emtConCollHumanHome(h, home, this);
}
void ControllerCollision::slotCollHumanWithPlane(ModelRobot *h, ModelPlane *plane)
{
    emit emtConCollHumanPlane(h, plane, this);
}
void ControllerCollision::slotCollHumanWithCar(ModelRobot *h, ModelCar *car)
{
    emit emtConCollHumanCar(h, car, this);
}

void ControllerCollision::slotColCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth)
{
    emit emtCollisionCollada(posx, posy, posz, rotx, roty, rotz, onErth);
}

void ControllerCollision::slotNotCollCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth)
{
    emit emtNotCollisionCollada(posx, posy, posz, rotx, roty, rotz, onErth);
}


