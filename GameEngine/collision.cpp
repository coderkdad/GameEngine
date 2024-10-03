#include "collision.h"
#include <QDebug>
#include <QTimer>
#include "vaoentity.h"
#include <qmath.h>
#include "math.h"
#include "modelrobot.h"
#include "modelcar.h"
#include "modelplane.h"
#include "modelerth.h"
#include "entityrocket.h"
#include "entity.h"
#include <QElapsedTimer>
#include <QThread>
#include "modelhome.h"
#include "modelhumanh.h"

Collision::Collision(QObject *parent) : QObject(parent)
{
    timerP = new QTimer(this);
//    connect(timerP, &QTimer::timeout, this, &Collision::slotTimerPlane);
    timerC = new QTimer(this);
//    connect(timerC, &QTimer::timeout, this, &Collision::slotTimerCar);
    timerH = new QTimer(this);
//    connect(timerH, &QTimer::timeout, this, &Collision::slotTimerHuman);
    timerGrav = new QTimer(this);
    connect(timerGrav, &QTimer::timeout, this, &Collision::slotTimerGrav);

    QVector3D rotA = QVector3D(90.0f, 90.0f, 90.0f);
    QCubeLimited *limi = new QCubeLimited;;
    limi->limitedTop = QVector3D(4,2,10);
    limi->limitedBottom = QVector3D(-4,0,-3);
//    qDebug("***********************************************");
    calcRotLimit2(limi, rotA);
}
Collision::~Collision()
{
    timerR->stop();
}

void Collision::allEntityCollision(ModelErth *erth,
                                   QList<ModelHome *> *listHome, ModelCar *modelCar, ModelPlane *modelPlane,
                                   ModelRobot *modelRobot, ModelHumanH *modelCollada)
{
    entityErth = erth;
    listEntityHome = listHome;
    m_modelRobot = modelRobot;
    m_modelCollada = modelCollada;
    m_modelCar = modelCar;
    m_modelPlane = modelPlane;
}


void Collision::workCollisionPlane(bool state)
{
    if(state)
    {
        timerP->start(10);
    }
    else
    {
        timerP->stop();
//        emit emtFinished();
    }
}
void Collision::workCollisionCar(bool state)
{
    if(state)
    {
        timerC->start(10);
    }
    else
    {
        timerC->stop();
    }
}
void Collision::workCollisionHuman(bool state)
{
    if(state)
    {
        timerH->start(10);
    }
    else
    {
        timerH->stop();
    }
}
void Collision::slotTimerGrav()
{
    QVector3D vecPoss(m_modelCollada->getModelPosition());
    QVector3D vecRots(m_modelCollada->getModelRotate());
    qDebug("timer>>>>>>");
    slotTimerCollada(vecPoss.x(),vecPoss.y(),vecPoss.z(), vecRots.x(),vecRots.y(),vecRots.z()); //#
}

void Collision::slotTimerPlane(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
    QVector3D posPlane(posX, posY, posZ) ;
    QCubeLimited limitPlane = calcRotLimit(m_modelPlane->getModelLimited(), rotY);

    /// ============= CHECKED Erth ============= ///
    QVector3D posErth = entityErth->getModelPosition();
    if(posPlane.y()+fabs(m_modelPlane->getModelLimited()->limitedBottom.y()) < posErth.y()+fabs(entityErth->getModelLimited()->limitedTop.y()))
    {
        m_modelPlane->setModelPosition(QVector3D(m_modelPlane->getModelPosition().x(),
                                           entityErth->getModelPosition().y()+entityErth->getModelLimited()->limitedTop.y() +
                                           m_modelPlane->getModelLimited()->limitedBottom.y()/*+1.6f*/, //speedPlane&newY in Plane
                                           m_modelPlane->getModelPosition().z()));
        // emit emtCollPlaneWithErth(m_modelPlane, entityErth);
//        qDebug()<<"Col P+Erth !!";
    }


    /// ============= CHECKED Home ============= ///
    for(int h=0; h<listEntityHome->count(); ++h)
    {
        QVector3D posHome = listEntityHome->at(h)->getModelPosition();
        QCubeLimited limitHome = calcRotLimit(listEntityHome->at(h)->getModelLimited(),
                                              listEntityHome->at(h)->getModelRotate().y());
    if(checkedCollision(posPlane, limitPlane, posHome, limitHome))
    {

    for(int ePlan=0; ePlan<m_modelPlane->getListEntity().count(); ++ePlan)
    {
        QVector3D posEntityPlane = calcRotPosition(m_modelPlane->getListEntity().at(ePlan)->getPosition(),
                                              m_modelPlane->getModelRotate().y());
        QCubeLimited limitEntityPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimited(),
                                                m_modelPlane->getModelRotate().y()+
                                                m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

        if(checkedCollision(posPlane+posEntityPlane, limitEntityPlane, posHome, limitHome))
        {
            for(int pPlane=0; pPlane<m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->count(); ++pPlane)
            {
                QCubeLimited limitPartPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->at(pPlane),
                                                           m_modelPlane->getListEntity().at(ePlan)->getRotation().y()+
                                                           m_modelPlane->getModelRotate().y());
                if(checkedCollision(posPlane+posEntityPlane, limitPartPlane, posHome, limitHome))
                {
                    // emit emtCollPlaneWithHome(m_modelPlane, listEntityHome->at(h));
                    m_modelPlane->sendCollisionPlane(QVector3D(posX,posY,posZ), QVector3D(rotX,rotY,rotZ));
//                    qDebug()<<"Col P+H : "<<ePlan<<"*"<<h;
                }
            }
        }

    }
    }

    }


    /// ============= CHECKED Car ============= ///
    QVector3D posCar = m_modelCar->getModelPosition();
    QCubeLimited limitCar = calcRotLimit(m_modelCar->getModelLimited(),
                                           m_modelCar->getModelRotate().y());
    if(checkedCollision(posPlane, limitPlane, posCar, limitCar))
    {

        for(int ePlan=0; ePlan<m_modelPlane->getListEntity().count(); ++ePlan)
        {
            QVector3D posEntityPlane = calcRotPosition(m_modelPlane->getListEntity().at(ePlan)->getPosition(),
                                                  m_modelPlane->getModelRotate().y());
            QCubeLimited limitEntityPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimited(),
                                                    m_modelPlane->getModelRotate().y()+
                                                    m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

            for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
            {
                QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                         m_modelCar->getModelRotate().y());
                QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                           m_modelCar->getModelRotate().y()+
                                                           m_modelCar->getListEntity().at(eCar)->getRotation().y());

        if(checkedCollision(posPlane+posEntityPlane, limitEntityPlane, posCar+posEntityCar, limitEntityCar))
        {
            for(int pPlane=0; pPlane<m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->count(); ++pPlane)
            {
                QCubeLimited limitPartPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->at(pPlane),
                                                           m_modelPlane->getModelRotate().y()+
                                                           m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

                for(int pCar=0; pCar<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++pCar)
                {
                    QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimitedParts()->at(pCar),
                                                             m_modelCar->getModelRotate().y()+
                                                             m_modelCar->getListEntity().at(eCar)->getRotation().y());

                    if(checkedCollision(posPlane+posEntityPlane, limitPartPlane, posCar+posEntityCar, limitPartCar))
                    {
                        // emit emtCollPlaneWithCar(m_modelPlane, m_modelCar);
//                            qDebug()<<"Col P+C : "<<ePlan<<"*"<<eCar;
                    }
                }
            }
        }
    }
    }
    }


    /// ============= CHECKED Human ============= ///
    if(!m_modelRobot->checkOn())
    {
        QVector3D posHuman = m_modelRobot->getModelPosition();
        QCubeLimited limitHuman = calcRotLimit(m_modelRobot->getModelLimited(),
                                               m_modelRobot->getModelRotate().y());
        if(checkedCollision(posPlane, limitPlane, posHuman, limitHuman))
        {

            for(int ePlan=0; ePlan<m_modelPlane->getListEntity().count(); ++ePlan)
            {
                QVector3D posEntityPlane = calcRotPosition(m_modelPlane->getListEntity().at(ePlan)->getPosition(),
                                                      m_modelPlane->getModelRotate().y());
                QCubeLimited limitEntityPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimited(),
                                                        m_modelPlane->getModelRotate().y()+
                                                        m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

                if(checkedCollision(posPlane+posEntityPlane, limitEntityPlane, posHuman, limitHuman))
                {
                    for(int pPlane=0; pPlane<m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->count(); ++pPlane)
                    {
                        QCubeLimited limitPartPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->at(pPlane),
                                                                   m_modelPlane->getModelRotate().y()+
                                                                   m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

                        if(checkedCollision(posPlane+posEntityPlane, limitPartPlane, posHuman, limitHuman))
                        {
//                             emit emtCollPlaneWithHuman(m_modelPlane, m_modelHuman);
//                            qDebug()<<"Col P+Hu : "<<ePlan<<"*";
                        }

                    }
                }
            }
        }
    }
}

void Collision::slotTimerCar(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
    QVector3D posCar(posX,posY,posZ);
    QVector3D rotCar(rotX,rotY,rotZ);
    QCubeLimited limitCar = calcRotLimit(m_modelCar->getModelLimited(), rotCar.y());

    /// ============= CHECKED Erth ============= ///

    /// Gravity Base Erth
    if(posCar.y()+limitCar.limitedBottom.y() <=
       entityErth->getModelPosition().y()+rngGrav)
    {
        posCar = QVector3D(posCar.x(),
                         fabs(limitCar.limitedBottom.y())+rngGrav,
                         posCar.z());
        posY = posCar.y();//
        m_modelCar->setModelPosition(posCar);//#
    } else {
        posCar = QVector3D(posCar.x(),
                         posCar.y()-rngGrav,
                         posCar.z());
        posY = posCar.y();//
        m_modelCar->setModelPosition(posCar);//#
        // Return Coll Grav
        if(posCar.y()+limitCar.limitedBottom.y() <=
           entityErth->getModelPosition().y()+rngGrav)
        {
            posCar = QVector3D(posCar.x(),
                             fabs(limitCar.limitedBottom.y())+rngGrav,
                             posCar.z());
            posY = posCar.y();//
            m_modelCar->setModelPosition(posCar);//#
        }
    }


    /// ============= CHECKED Home ============= ///
    for(int h=0; h<listEntityHome->count(); ++h)
    {
        QVector3D posHome = listEntityHome->at(h)->getModelPosition();
        QCubeLimited limitHome = calcRotLimit(listEntityHome->at(h)->getModelLimited(),
                                              listEntityHome->at(h)->getModelRotate().y());
        if(checkedCollision(posCar, limitCar, posHome, limitHome))
        {
            /// Gravity Home
            if(posCar.y()+limitCar.limitedBottom.y() < posHome.y()+fabs(limitHome.limitedTop.y()+rngGrav) &&
               posCar.y()+limitCar.limitedBottom.y() > posHome.y()+fabs(limitHome.limitedTop.y()-rngGrav))
            {
                posCar = QVector3D(posCar.x(),
                                 listEntityHome->at(h)->getModelLimited()->limitedTop.y()
                                +fabs(limitCar.limitedBottom.y())+rngGrav,
                                 posCar.z());
                posY = posCar.y();//
                m_modelCar->setModelPosition(posCar);//#

            } else {

                for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
                {
                    QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                          rotCar.y());

                    QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                            rotCar.y()+
                                                            m_modelCar->getListEntity().at(eCar)->getRotation().y());
    // Check Limit Entity
    if(checkedCollision(posCar+posEntityCar, limitEntityCar, posHome, limitHome))
    {

        for(int pCar=0; pCar<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++pCar)
        {
            QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimitedParts()->at(pCar),
                                        m_modelCar->getModelRotate().y()+
                                        m_modelCar->getListEntity().at(eCar)->getRotation().y());
            if(checkedCollision(posCar+posEntityCar, limitPartCar, posHome, limitHome))
            {
                m_modelCar->sendCollisionCar(QVector3D(posX,posY,posZ), QVector3D(rotX,rotY,rotZ));
//                emit emtCollCarWithHome(m_modelCar, listEntityHome->at(h));
//                qDebug()<<"Col C+h : "<<eCar;
                return;
            }
        }
    }

                } // end Entity

            }
        }
    }

    /// ============= CHECKED Plane ============= ///
    QVector3D posPlane = m_modelPlane->getModelPosition();
    QCubeLimited limitPlane = calcRotLimit(m_modelPlane->getModelLimited(),
                                           m_modelPlane->getModelRotate().y());
    if(checkedCollision(posCar, limitCar, posPlane, limitPlane))
    {
        for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
        {
            QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                     m_modelCar->getModelRotate().y());
            QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                       m_modelCar->getModelRotate().y()+
                                                       m_modelCar->getListEntity().at(eCar)->getRotation().y());

            for(int ePlan=0; ePlan<m_modelPlane->getListEntity().count(); ++ePlan)
            {
                QVector3D posEntityPlan = calcRotPosition(m_modelPlane->getListEntity().at(ePlan)->getPosition(),
                                                          m_modelPlane->getModelRotate().y());
                QCubeLimited limitEntityPlan = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimited(),
                                                            m_modelPlane->getModelRotate().y()+
                                                            m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

if(checkedCollision(posCar+posEntityCar, limitEntityCar, posPlane+posEntityPlan, limitEntityPlan))
{
    for(int pCar=0; pCar<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++pCar)
    {
        QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimitedParts()->at(pCar),
                                    m_modelCar->getModelRotate().y()+
                                    m_modelCar->getListEntity().at(eCar)->getRotation().y());

        for(int pPlan=0; pPlan<m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->count(); ++pPlan)
        {
            QCubeLimited limiPartPlan = calcRotLimit(m_modelPlane->getListEntity().at(ePlan)->getLimitedParts()->at(pPlan),
                                                     m_modelPlane->getModelRotate().y()+
                                                     m_modelPlane->getListEntity().at(ePlan)->getRotation().y());

            if(checkedCollision(posCar+posEntityCar, limitPartCar, posPlane+posEntityPlan, limiPartPlan))
            {
                m_modelCar->sendCollisionCar(QVector3D(posX,posY,posZ), QVector3D(rotX,rotY,rotZ));
//                qDebug()<<"Parts: "<<pCar<<"="<<pPlan;
                return ;
            }
        }
    }
}
            }
        }
    }
    // end car plane

    /// ============= CHECKED Human ============= ///
    if(!m_modelRobot->checkOn())
    {
        QVector3D posHuman = m_modelRobot->getModelPosition();
        QCubeLimited limitHuman = calcRotLimit(m_modelRobot->getModelLimited(),
                                               m_modelRobot->getModelRotate().y());
        if(checkedCollision(posCar, limitCar, posHuman, limitHuman))
        {
            for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
            {
                QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                         m_modelCar->getModelRotate().y());
                QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                           m_modelCar->getModelRotate().y()+
                                                           m_modelCar->getListEntity().at(eCar)->getRotation().y());

                if(checkedCollision(posCar+posEntityCar, limitEntityCar, posHuman, limitHuman))
                {

                    for(int p=0; p<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++p)
                    {
                        QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimitedParts()->at(p),
                                                  m_modelCar->getModelRotate().y()+
                                                  m_modelCar->getListEntity().at(eCar)->getRotation().y());
                        if(checkedCollision(posCar+posEntityCar, limitPartCar, posHuman, limitHuman))
                        {
                            m_modelRobot->sendDropHuman(m_modelCar->getModelPosition(), m_modelCar->getDirection());

//                            qDebug()<<"Col C+h : "<<eCar<<"***";
                        }
                    }
                }
            }
        }
    }
}

void Collision::slotTimerHuman(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
//    QElapsedTimer timerElpsed;
//    timerElpsed.start();

    // سبب الانزالق هو أنه عند السقوط تكون قيمة
    // onErths = false
    // وبذلك فإن الحركة للأمام تكون مستمرة حتى تتغير قيمتها
    // ثم تستمر الحركة قليلاً إلى عودة اليدين للوضع الطبيعي
    // كما في دالة AnimitionMove()

    bool stateColHome=false;
    bool stateColPlane=false;
    bool stateColCar=false;
    bool onErths=false;

    QVector3D posHuman = m_modelRobot->getModelPosition();
    QCubeLimited limitHuman = *m_modelRobot->getModelLimited();

    /// ============= CHECKED Erth ============= ///

    /// Gravity Base Erth
//    if(stateGravy)
//    {
        onErths = funGravity(&posHuman, &limitHuman);
//    }

//    qDebug()<<"P "<<*entityErth->getInfo()->arry<<" "<<**entityErth->getInfo()->arry;
//    qDebug()<<"LL "<<(entityErth->getInfo()->arry+(4*entityErth->getInfo()->numColArray));
//    qDebug()<<"LL "<<*(entityErth->getInfo()->arry+entityErth->getInfo()->numColArray);
//    qDebug()<<"LL "<<**(entityErth->getInfo()->arry+entityErth->getInfo()->numColArray);

    /// ============= CHECKED Home ============= ///
    for(int h=0; h<listEntityHome->count(); ++h)
    {
        QVector3D posHome = listEntityHome->at(h)->getModelPosition();
        QCubeLimited limitHome = calcRotLimit(listEntityHome->at(h)->getModelLimited(),
                                              listEntityHome->at(h)->getModelRotate().y());
    if(checkedCollision(posHuman, limitHuman, posHome, limitHome))
    {
        /// Gravity Home
        float limitTopHome = posHome.y()+limitHome.limitedTop.y();
        if(posHuman.y()+limitHuman.limitedBottom.y() < limitTopHome+(rngGrav*1.5f) &&
           posHuman.y()+limitHuman.limitedBottom.y() > limitTopHome-(rngGrav*1.5f))
        {
            onErths=true;
            posHuman = QVector3D(posHuman.x(),
                                 limitTopHome + fabs(limitHuman.limitedBottom.y()),
                                 posHuman.z());

            if(timerGrav->isActive())
            {
                timerGrav->stop();
            }
        }
        else
        {
//    for(int pHome=0; pHome<listEntityHome->at(h)->getLimitedParts()->count(); ++pHome)
//    {
//        QCubeLimited limitPartHome = calcRotLimit(listEntityHome->at(h)->getLimitedParts()->at(pHome),
//                                                   listEntityHome->at(h)->getRotation().y());
//        qDebug()<<":::::::"<<pHome<<limitPartHome.limitedTop<<"|"<<limitPartHome.limitedBottom;

//        if(checkedCollision(posHuman, limitHuman, posHome, limitPartHome))
//        {
//        if(!timerGrav->isActive())
//        {
//            timerGrav->start(20);
//        }
            m_modelRobot->sendCollisionHuman(QVector3D(posX,posHuman.y(),posZ), QVector3D(rotX,rotY,rotZ));
            stateColHome=true;
//            qDebug()<<"Col Hu+h : "<<h;
//        }
//    }
        }
//        return;
    }
    }

    /// ============= CHECKED Plane ============= ///
    QVector3D posPlane = m_modelPlane->getModelPosition();
    QCubeLimited limitPlane = calcRotLimit(m_modelPlane->getModelLimited(),
                                           m_modelPlane->getModelRotate().y());
    if(checkedCollision(posHuman, limitHuman, posPlane, limitPlane))
    {

        for(int ePlane=0; ePlane<m_modelPlane->getListEntity().count(); ++ePlane)
        {
            QVector3D posEntityPlane = calcRotPosition(m_modelPlane->getListEntity().at(ePlane)->getPosition(),
                                                       m_modelPlane->getModelRotate().y());
            QCubeLimited limitEntityPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlane)->getLimited(),
                                                       m_modelPlane->getModelRotate().y()+
                                                       m_modelPlane->getListEntity().at(ePlane)->getRotation().y());

        if(checkedCollision(posHuman, limitHuman, posPlane+posEntityPlane, limitEntityPlane))
        {

            for(int pPlane=0; pPlane<m_modelPlane->getListEntity().at(ePlane)->getLimitedParts()->count(); ++pPlane)
            {
                QCubeLimited limitPartPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlane)->getLimitedParts()->at(pPlane),
                                                           m_modelPlane->getModelRotate().y()+
                                                           m_modelPlane->getListEntity().at(ePlane)->getRotation().y());

            if(checkedCollision(posHuman, limitHuman, posPlane+posEntityPlane, limitPartPlane))
            {
                /// Gravity Plane
                float limitTopPartPlane = posPlane.y()+posEntityPlane.y()+limitPartPlane.limitedTop.y();
                if(posHuman.y()+limitHuman.limitedBottom.y() <= limitTopPartPlane+(rngGrav*1.5f) &&
                   posHuman.y()+limitHuman.limitedBottom.y() >= limitTopPartPlane-(rngGrav*1.5f))
                {
//                    qDebug("On Pl");
                    onErths=true;
                    posHuman = QVector3D(posHuman.x(),
                                         limitTopPartPlane + fabs(limitHuman.limitedBottom.y()),
                                         posHuman.z());

                    if(timerGrav->isActive())
                    {
                        timerGrav->stop();
//                        qDebug()<<"Timr: "<<timerGrav->isActive();
                    }
                }
                else
                {
//                    qDebug("Coll Pl");
                    m_modelRobot->sendCollisionHuman(QVector3D(posX,posY,posZ), QVector3D(rotX,rotY,rotZ));
                    stateColPlane=true;
                }
//                return;
            }
            }
        }
        }
    }

    /// ============= CHECKED Car ============= ///
    QVector3D posCar = m_modelCar->getModelPosition();
    QCubeLimited limitCar = calcRotLimit(m_modelCar->getModelLimited(),
                                         m_modelCar->getModelRotate().y());
    if(checkedCollision(posHuman, limitHuman, posCar, limitCar))
    {

        for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
        {
            QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                     m_modelCar->getModelRotate().y());
            QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                       m_modelCar->getModelRotate().y()+
                                                       m_modelCar->getListEntity().at(eCar)->getRotation().y());

        if(checkedCollision(posHuman, limitHuman, posCar+posEntityCar, limitEntityCar))
        {

            for(int pCar=0; pCar<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++pCar)
            {
                QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                         m_modelCar->getModelRotate().y()+
                                                         m_modelCar->getListEntity().at(eCar)->getRotation().y());

            if(checkedCollision(posHuman, limitHuman, posCar+posEntityCar, limitPartCar))
            {
                /// Gravity Car
                float limitTopPartCar = posCar.y()+posEntityCar.y()+limitPartCar.limitedTop.y();
                if(posHuman.y()+limitHuman.limitedBottom.y() <= limitTopPartCar+(rngGrav*1.5f) &&
                        posHuman.y()+limitHuman.limitedBottom.y() >= limitTopPartCar-(rngGrav*1.5f))
                {
                    onErths=true;
                    posHuman = QVector3D(posHuman.x(),
                                         limitTopPartCar + fabs(limitHuman.limitedBottom.y()),
                                         posHuman.z());

                    if(timerGrav->isActive())
                    {
                        timerGrav->stop();
                    }
                }
                else
                {
                    m_modelRobot->sendCollisionHuman(QVector3D(posX,posHuman.y(),posZ), QVector3D(rotX,rotY,rotZ));
                    stateColCar=true;
                }
                //                return;
            }
            }
        }
        }

    }

    /// ============= UPDATE ============= ///
    if(stateColHome || stateColPlane || stateColCar)
    {
//        qDebug("1");
        m_modelRobot->sendOnErth(onErths);
    }
    else
    {
//        qDebug("2");
        m_modelRobot->setModelPosition(posHuman);
        m_modelRobot->sendOnErth(onErths);
//        qDebug()<<"GravPos: "<<posHuman;
    }

//    qDebug()<<"Elapsed: "<<timerElpsed.nsecsElapsed();
}

void Collision::slotTimerCollada(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
    qDebug("START");

//    QElapsedTimer timerElpsed;
//    timerElpsed.start();
//
        {
            // سبب الانزالق هو أنه عند السقوط تكون قيمة
            // onErths = false
            // وبذلك فإن الحركة للأمام تكون مستمرة حتى تتغير قيمتها
            // ثم تستمر الحركة قليلاً إلى عودة اليدين للوضع الطبيعي
            // كما في دالة AnimitionMove()
            //
            // في المقارنة يجب أن تكون زائد وليس ناقص
            // ويعتمد أن تكون المركز للجسم داخل حدود الكاركتر
            // وبذلك تكون قيمة الحد الأدنى للواي أصغر أو يساوي الموقع للواي
            //
            // يجب التنسيق بين قيمة نزول الجاذبية
            // وقيمة الفحص مثلاً عند حافة المنزل
            // قيمة النزول هي 0.05
            // قيمة الفحص للحافة يجب أن تكون في أكبر من 0.05
            // لأن الفحص لا يتم إلا عند حدوث التصادم بين الشخص والمنزل
            // وبذلك فقدنا القيمة الأكبر من حافة المنزل وغير مفيدة للفحص
        }

    bool onErths=false;
    bool stateColHome=false;
    bool stateColPlane=false;
    bool stateColCar=false;

    QVector3D posHuman(posX,posY,posZ); //= m_modelCollada->getModelPosition();
    QVector3D rotHuman(rotX,rotY,rotZ);
    QCubeLimited limitHuman = calcRotLimit(m_modelCollada->getModelLimited(), rotHuman.y());



    /// ============= CHECKED Erth ============= ///
    onErths = funGravity(&posHuman, &limitHuman);





    /// ============= CHECKED Home ============= ///
    for(int h=0; h<listEntityHome->count(); ++h)
    {
        QVector3D posHome = listEntityHome->at(h)->getModelPosition();
        QCubeLimited limitHome = calcRotLimit(listEntityHome->at(h)->getModelLimited(),
                                              listEntityHome->at(h)->getModelRotate().y());


    if(checkedCollision(posHuman, limitHuman, posHome, limitHome))
    {
        bool localOnErth = false;
        float localTopHuman=0.0f;
        bool localCollEntity = false;
        qDebug()<<"WWW: ";

    for(int pHome=0; pHome<listEntityHome->at(h)->getModelLimitParts()->count(); ++pHome)
    {
        QCubeLimited limitPartHome = calcRotLimit(listEntityHome->at(h)->getModelLimitParts()->at(pHome),
                                                   listEntityHome->at(h)->getModelRotate().y());
//        qDebug()<<":::::::"<<pHome<<limitPartHome.limitedTop<<"|"<<limitPartHome.limitedBottom;

//        qDebug()<<"check part "<<pHome<<" : "<<limitPartHome.limitedTop<<" | "<<limitPartHome.limitedBottom<<" ::: "<<posHome;
        if(checkedCollision(posHuman, limitHuman, posHome, limitPartHome))
        {
            qDebug()<<"COLLISION PART: "<<posHuman;
            /// Gravity Home
            float limitTopHomePart = posHome.y()+limitPartHome.limitedTop.y();
            if(posHuman.y()+limitHuman.limitedBottom.y() < limitTopHomePart+(rngGrav*3) &&
               posHuman.y()+limitHuman.limitedBottom.y() > limitTopHomePart-(rngGrav*3))
            {
                qDebug("@ On Home");
                localOnErth=true;
                localTopHuman=limitTopHomePart + fabs(limitHuman.limitedBottom.y());
                //0ld
//                onErths=true;
//                posHuman = QVector3D(posHuman.x(),
//                                     limitTopHomePart + fabs(limitHuman.limitedBottom.y()),
//                                     posHuman.z());
//                if(timerGrav->isActive())
//                {
//                    timerGrav->stop();
//                }
            }
            else
            {
                qDebug("@ NOT On Home");
                localCollEntity=true;
//                qDebug()<<"# No Home "<< posHuman.y()+limitHuman.limitedBottom.y() <<" | "
//                        <<limitTopHome+(rngGrav*1.5f)<<" | "<<limitTopHome-(rngGrav*1.5f);

                //0ld
//                m_modelCollada->sendCollisionCollada(QVector3D(posX,posHuman.y(),posZ), QVector3D(rotX,rotY,rotZ));
//                stateColHome=true;
//                break; // لابد من فحص جميع الأجزاء لأنه أحياناً يكون على جزء ويتصادم مع آخر


            }
//            qDebug()<<"AAAAAA: "<<posHuman;
//        if(!timerGrav->isActive())
//        {
//            timerGrav->start(20);
//        }
//            m_modelCollada->sendCollisionCollada(QVector3D(posX,posHuman.y(),posZ), QVector3D(rotX,rotY,rotZ));
//            qDebug()<<"OOO Home:"<<posHome << " \n"<<limitHome.limitedTop<<" | "<<limitHome.limitedBottom;
//            qDebug()<<"OOO Human:"<<posHuman << "\n"<<limitHuman.limitedTop<<" | "<<limitHuman.limitedBottom;
//            stateColHome=true;
//            break;
//            tests = false;
//            qDebug()<<"Col Hu+h : "<<h;

            //New Step
            if(localOnErth && localCollEntity)
            {
//                qDebug("BREAK");
                break;
            }

        }
    }




    //New Step for Collision Parts
    if(localOnErth && localCollEntity)
    {
        onErths=true;
        stateColHome=true;
        posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

        if(timerGrav->isActive())
        {
            timerGrav->stop();
        }

        break;
    }
    else if (localOnErth && (!localCollEntity))
    {
        onErths=true;
        posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

        if(timerGrav->isActive())
        {
            timerGrav->stop();
        }
    }
    else if (localCollEntity && (!localOnErth))
    {
        stateColHome=true;
    }

//        return;
    }





    }




    /// ============= CHECKED Plane ============= ///
    QVector3D posPlane = m_modelPlane->getModelPosition();
    QCubeLimited limitPlane = calcRotLimit(m_modelPlane->getModelLimited(),
                                           m_modelPlane->getModelRotate().y());
    if(checkedCollision(posHuman, limitHuman, posPlane, limitPlane))
    {

        for(int ePlane=0; ePlane<m_modelPlane->getListEntity().count(); ++ePlane)
        {
            bool localOnErth = false;
            float localTopHuman=0.0f;
            bool localCollEntity = false;

            QVector3D posEntityPlane = calcRotPosition(m_modelPlane->getListEntity().at(ePlane)->getPosition(),
                                                       m_modelPlane->getModelRotate().y());
            QCubeLimited limitEntityPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlane)->getLimited(),
                                                       m_modelPlane->getModelRotate().y()+
                                                       m_modelPlane->getListEntity().at(ePlane)->getRotation().y());

        if(checkedCollision(posHuman, limitHuman, posPlane+posEntityPlane, limitEntityPlane))
        {

            for(int pPlane=0; pPlane<m_modelPlane->getListEntity().at(ePlane)->getLimitedParts()->count(); ++pPlane)
            {
                QCubeLimited limitPartPlane = calcRotLimit(m_modelPlane->getListEntity().at(ePlane)->getLimitedParts()->at(pPlane),
                                                           m_modelPlane->getModelRotate().y()+
                                                           m_modelPlane->getListEntity().at(ePlane)->getRotation().y());

            if(checkedCollision(posHuman, limitHuman, posPlane+posEntityPlane, limitPartPlane))
            {
                qDebug()<<"=============== COL PART: "<<pPlane;
                /// Gravity Plane
                float limitTopPartPlane = posPlane.y()+posEntityPlane.y()+limitPartPlane.limitedTop.y();
                if(posHuman.y()+limitHuman.limitedBottom.y() <= limitTopPartPlane+(rngGrav*3.0f) &&
                   posHuman.y()+limitHuman.limitedBottom.y() >= limitTopPartPlane-(rngGrav*3.0f))
                {
                    qDebug("== Ons");
                    localOnErth = true;
                    localTopHuman = limitTopPartPlane + fabs(limitHuman.limitedBottom.y());
                }
                else
                {
                    qDebug("== Cols");
                    localCollEntity = true;
                }
//                return;

            }

            //New Step
            if(localOnErth && localCollEntity)
            {
                qDebug("BREAK");
                break;

            }

            }

        }


        //New Step for Collision Parts
        if(localOnErth && localCollEntity)
        {
            onErths=true;
            stateColPlane=true;
            posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

            if(timerGrav->isActive())
            {
                timerGrav->stop();
            }

        }
        else if (localOnErth && (!localCollEntity))
        {
            onErths=true;
            posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

            if(timerGrav->isActive())
            {
                timerGrav->stop();
            }
        }
        else if (localCollEntity && (!localOnErth))
        {
            stateColPlane=true;
        }

        }

    }





    /// ============= CHECKED Car ============= ///
    QVector3D posCar = m_modelCar->getModelPosition();
    QCubeLimited limitCar = calcRotLimit(m_modelCar->getModelLimited(),
                                         m_modelCar->getModelRotate().y());
    if(checkedCollision(posHuman, limitHuman, posCar, limitCar))
    {

        for(int eCar=0; eCar<m_modelCar->getListEntity().count(); ++eCar)
        {
            bool localOnErth = false;
            float localTopHuman=0.0f;
            bool localCollEntity = false;

            QVector3D posEntityCar = calcRotPosition(m_modelCar->getListEntity().at(eCar)->getPosition(),
                                                     m_modelCar->getModelRotate().y());
            QCubeLimited limitEntityCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                       m_modelCar->getModelRotate().y()+
                                                       m_modelCar->getListEntity().at(eCar)->getRotation().y());

        if(checkedCollision(posHuman, limitHuman, posCar+posEntityCar, limitEntityCar))
        {

            for(int pCar=0; pCar<m_modelCar->getListEntity().at(eCar)->getLimitedParts()->count(); ++pCar)
            {
                QCubeLimited limitPartCar = calcRotLimit(m_modelCar->getListEntity().at(eCar)->getLimited(),
                                                         m_modelCar->getModelRotate().y()+
                                                         m_modelCar->getListEntity().at(eCar)->getRotation().y());

            if(checkedCollision(posHuman, limitHuman, posCar+posEntityCar, limitPartCar))
            {
                /// Gravity Car
                float limitTopPartCar = posCar.y()+posEntityCar.y()+limitPartCar.limitedTop.y();
                if(posHuman.y()+limitHuman.limitedBottom.y() <= limitTopPartCar+(rngGrav*3.0f) &&
                        posHuman.y()+limitHuman.limitedBottom.y() >= limitTopPartCar-(rngGrav*3.0f))
                {
                    qDebug("== Ons");
                    localOnErth = true;
                    localTopHuman = limitTopPartCar + fabs(limitHuman.limitedBottom.y());
                }
                else
                {
                    qDebug("== Cols");
                    localCollEntity = true;
                }
//                return;
            }

            //New Step
            if(localOnErth && localCollEntity)
            {
                qDebug("BREAK");
                break;

            }

            }
        }

        //New Step for Collision Parts
        if(localOnErth && localCollEntity)
        {
            onErths=true;
            stateColCar=true;
            posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

            if(timerGrav->isActive())
            {
                timerGrav->stop();
            }

        }
        else if (localOnErth && (!localCollEntity))
        {
            onErths=true;
            posHuman = QVector3D(posHuman.x(), localTopHuman,posHuman.z());

            if(timerGrav->isActive())
            {
                timerGrav->stop();
            }
        }
        else if (localCollEntity && (!localOnErth))
        {
            stateColCar=true;
        }

        }
        }






    //    /// ============= UPDATE ============= ///
        qDebug()<<"~~~~~~ CHecked: "<<posHuman;
        if(stateColHome || stateColPlane || stateColCar)
        {
            emit emtCollisionCollada(posHuman.x(), posHuman.y(), posHuman.z(),
                                     rotHuman.x(), rotHuman.y(), rotHuman.z(), onErths);
        }
        else
        {
            emit emtNotCollisionCollada(posHuman.x(), posHuman.y(), posHuman.z(),
                                     rotHuman.x(), rotHuman.y(), rotHuman.z(), onErths);
        }

}


bool Collision::funGravity(QVector3D *posHuman, QCubeLimited *limitHuman)
{
    bool onErths = false;
    float spaceRng = entityErth->getInfo()->numSpaceRng;
    int numMaxX = entityErth->getInfo()->countMaxX;
    int numMaxZ = entityErth->getInfo()->countMaxZ;
    float posHX = posHuman->x();
    float posHZ = posHuman->z();
    int IDx=0;
    int IDz=0;

    if(posHX > (numMaxX*spaceRng) || posHX < -(numMaxX*spaceRng))
    {
        return true;
    }
    else if(posHX >= 0)
    {
        int px = floor(fabs(posHX)/spaceRng);
        IDx = numMaxX-(px+1);
    }
    else if (posHX < 0)
    {
        int px = ceil(fabs(posHX) / spaceRng);
        IDx = numMaxX+(px-1);
    }


    if(posHZ > (numMaxZ*spaceRng) || posHZ < -(numMaxZ*spaceRng))
    {
        return true;
    }
    else if(posHZ >= 0.0f)
    {
        int pz = floor(fabs(posHZ)/spaceRng);
        IDz = numMaxZ-(pz+1);
    }
    else if(posHZ < 0.0f)
    {
        int pz = ceil(fabs(posHZ) / spaceRng);
        IDz = numMaxZ+(pz-1);
    }

    float Ups = 0;
    if(posHX > 100 || posHX < -100 ||
       posHZ > 100 || posHZ < -100)
    {
        Ups = 0.0f;
    }
    else
    {
//        qDebug()<<"indexs: "<<IDx<<" "<<IDz;
        Ups = entityErth->getInfo()->arry[IDx][IDz];
//        qDebug()<<" = "<<Ups;
    }


    //===================//

    float limitTopErth = Ups;
    if(posHuman->y()+limitHuman->limitedBottom.y() <= limitTopErth+rngGrav)
    {
        onErths=true;

        *posHuman = QVector3D(posHuman->x(),
                             limitTopErth + fabs(limitHuman->limitedBottom.y())+rngGrav,
                             posHuman->z());

        if(timerGrav->isActive())
        {
            timerGrav->stop();
        }
    }
    else if(posHuman->y()+limitHuman->limitedBottom.y() > limitTopErth+rngGrav)
    {
        if(stateGravy)
        {
            onErths=false;
            *posHuman = QVector3D(posHuman->x(),
                                 posHuman->y()-(rngGrav+0.03), // down (0.02+0.03) 0.05
                                 posHuman->z());
            qDebug("DOWN////");

            if(posHuman->y()+limitHuman->limitedBottom.y() <= limitTopErth+rngGrav)
            {
                onErths=true;
                *posHuman = QVector3D(posHuman->x(),
                                     limitTopErth + fabs(limitHuman->limitedBottom.y())+rngGrav,
                                     posHuman->z());

                if(timerGrav->isActive())
                {
                    timerGrav->stop();
                }

            }
            else
            {
                if(!timerGrav->isActive())
                {
                    timerGrav->start(20);
                }
            }
        }
    }
//    qDebug()<<"POSs: "<<*posHuman;
    return onErths;
}

void Collision::setStateGravy(bool state)
{
    stateGravy = state;
}
QVector3D Collision::calcRotPosition(QVector3D pos, int rots)
{
    float lenthPos = sqrt( pow(pos.x(),2) + pow(pos.z(),2) );

    float angelPos = qRadiansToDegrees( atan2(pos.x(), pos.z()) ) ;

    float doubleRotatePos = angelPos + rots;

    QVector3D newPoss = QVector3D(lenthPos* sin(qDegreesToRadians(doubleRotatePos)),
                                  pos.y(),
                                  lenthPos* cos(qDegreesToRadians(doubleRotatePos)));

    return newPoss;
}
QCubeLimited Collision::calcRotLimit(QCubeLimited *limit, int rots)
{
    QVector2D topRA = QVector2D(limit->limitedTop.x(), limit->limitedBottom.z());
    QVector2D topRB = QVector2D(limit->limitedTop.x(), limit->limitedTop.z());
    QVector2D topLA = QVector2D(limit->limitedBottom.x(), limit->limitedBottom.z());
    QVector2D topLB = QVector2D(limit->limitedBottom.x(), limit->limitedTop.z());
    QVector2D heights = QVector2D(limit->limitedTop.y(), limit->limitedBottom.y());

    float lenthRA = sqrt( pow(topRA.x(),2) + pow(topRA.y(),2) );
    float lenthRB = sqrt( pow(topRB.x(),2) + pow(topRB.y(),2) );
    float lenthLA = sqrt( pow(topLA.x(),2) + pow(topLA.y(),2) );
    float lenthLB = sqrt( pow(topLB.x(),2) + pow(topLB.y(),2) );

    float angelRA = qRadiansToDegrees( atan2(topRA.x(), topRA.y()) ) ;
    float angelRB = qRadiansToDegrees( atan2(topRB.x(), topRB.y()) ) ;
    float angelLA = qRadiansToDegrees( atan2(topLA.x(), topLA.y()) ) ;
    float angelLB = qRadiansToDegrees( atan2(topLB.x(), topLB.y()) ) ;

    float doubleRotateRA = angelRA + rots;
    float doubleRotateRB = angelRB + rots;
    float doubleRotateLA = angelLA + rots;
    float doubleRotateLB = angelLB + rots;

    QVector2D newRA = QVector2D(lenthRA* sin(qDegreesToRadians(doubleRotateRA)),
                                lenthRA* cos(qDegreesToRadians(doubleRotateRA)));
    QVector2D newRB = QVector2D(lenthRB* sin(qDegreesToRadians(doubleRotateRB)),
                                lenthRB* cos(qDegreesToRadians(doubleRotateRB)));
    QVector2D newLA = QVector2D(lenthLA* sin(qDegreesToRadians(doubleRotateLA)),
                                lenthLA* cos(qDegreesToRadians(doubleRotateLA)));
    QVector2D newLB = QVector2D(lenthLB* sin(qDegreesToRadians(doubleRotateLB)),
                                lenthLB* cos(qDegreesToRadians(doubleRotateLB)));

    QCubeLimited limited;
    limited.limitedTop.setX( fmax( fmax(newLA.x(),newLB.x()) , fmax(newRA.x(),newRB.x()) ) );
    limited.limitedTop.setY(heights.x());
    limited.limitedTop.setZ( fmax( fmax(newLA.y(),newLB.y()) , fmax(newRA.y(),newRB.y()) ) );

    limited.limitedBottom.setX( fmin( fmin(newLA.x(),newLB.x()) , fmin(newRA.x(),newRB.x()) ) );
    limited.limitedBottom.setY(heights.y());
    limited.limitedBottom.setZ( fmin( fmin(newLA.y(),newLB.y()) , fmin(newRA.y(),newRB.y()) ) );

    return limited;
}

QVector3D Collision::calcRotPosition2(QVector3D pos, QVector3D rots)
{
    QVector3D posPoint = pos; //QVector3D(2.0f, 2.0f, 2.0f);
    QVector3D rotEntity = rots; //QVector3D(90.0f,90.0f,90.0f);
//    QVector3D posParent = QVector3D(0,0,0);

    /// ============== STEP 1 ============= ///
    /// X
    float lenthVec = sqrt( pow(posPoint.y(),2) + pow(posPoint.z(),2) );
    float angelVec = qRadiansToDegrees( atan2(posPoint.z(), posPoint.y()) ) ;
    float doubleRotVec = angelVec+rotEntity.x();
    QVector3D posEntityNew = QVector3D(posPoint.x(),
                                       lenthVec* cos(qDegreesToRadians(doubleRotVec)),
                                       lenthVec* sin(qDegreesToRadians(doubleRotVec)));

    /// Z
    float lenthVec3 = sqrt( pow(posEntityNew.x(),2) + pow(posEntityNew.y(),2) );
    float angelVec3 = qRadiansToDegrees( atan2(posEntityNew.y(), posEntityNew.x()) ) ; // True y->x :: Erro x->y
    float doubleRotVec3 = angelVec3+rotEntity.z();
    QVector3D posEntityNew3 = QVector3D(lenthVec3* cos(qDegreesToRadians(doubleRotVec3)),
                                        lenthVec3* sin(qDegreesToRadians(doubleRotVec3)),
                                        posEntityNew.z());

    /// y
    float lenthVec2 = sqrt( pow(posEntityNew3.x(),2) + pow(posEntityNew3.z(),2) );
    float angelVec2 = qRadiansToDegrees( atan2(posEntityNew3.x(), posEntityNew3.z()) ) ;
    float doubleRotVec2 = angelVec2+rotEntity.y();
    QVector3D posEntityNew2 = QVector3D(lenthVec2* sin(qDegreesToRadians(doubleRotVec2)),
                                        posEntityNew3.y(),
                                        lenthVec2* cos(qDegreesToRadians(doubleRotVec2)));

    return posEntityNew2;
}
QCubeLimited Collision::calcRotLimit2(QCubeLimited *limit, QVector3D rots)
{
    QVector3D topRA = QVector3D(limit->limitedTop.x(), limit->limitedTop.y(), limit->limitedTop.z());
    QVector3D topRB = QVector3D(limit->limitedTop.x(), limit->limitedTop.y(), limit->limitedBottom.z());
    QVector3D topLA = QVector3D(limit->limitedBottom.x(), limit->limitedTop.y(), limit->limitedTop.z());
    QVector3D topLB = QVector3D(limit->limitedBottom.x(), limit->limitedTop.y(), limit->limitedBottom.z());

    QVector3D bottomRA = QVector3D(limit->limitedTop.x(), limit->limitedBottom.y(), limit->limitedTop.z());
    QVector3D bottomRB = QVector3D(limit->limitedTop.x(), limit->limitedBottom.y(), limit->limitedBottom.z());
    QVector3D bottomLA = QVector3D(limit->limitedBottom.x(), limit->limitedBottom.y(), limit->limitedTop.z());
    QVector3D bottomLB = QVector3D(limit->limitedBottom.x(), limit->limitedBottom.y(), limit->limitedBottom.z());

    /// ================ ///

    QVector3D topRANew = calcRotPosition2(topRA, rots);
    QVector3D topRBNew = calcRotPosition2(topRB, rots);
    QVector3D topLANew = calcRotPosition2(topLA, rots);
    QVector3D topLBNew = calcRotPosition2(topLB, rots);

    QVector3D bottomRANew = calcRotPosition2(bottomRA, rots);
    QVector3D bottomRBNew = calcRotPosition2(bottomRB, rots);;
    QVector3D bottomLANew = calcRotPosition2(bottomLA, rots);;
    QVector3D bottomLBNew = calcRotPosition2(bottomLB, rots);;


//    qDebug()<<"HIEGHT: \n"<<topRA<<" | "<<topRB<<" | "<<topLA<<" | "<<topLB<<" \n"
//                          <<bottomRA<<" | "<<bottomRB<<" | "<<bottomLA<<" | "<<bottomLB<<"END Beffore\n++++++++++\n AFFTER:\n"

//                          <<topRANew<<" | "<<topRBNew<<" | "<<topLANew<<" | "<<topLBNew<<" \n"
//                          <<bottomRANew<<" | "<<bottomRBNew<<" | "<<bottomLANew<<" | "<<bottomLBNew<<"END ALL ==================================== \n \n";


    {
        //    QVector3D heights = QVector2D(limit->limitedTop.y(), limit->limitedBottom.y());

        //    float lenthRA = sqrt( pow(topRA.x(),2) + pow(topRA.y(),2) );
        //    float lenthRB = sqrt( pow(topRB.x(),2) + pow(topRB.y(),2) );
        //    float lenthLA = sqrt( pow(topLA.x(),2) + pow(topLA.y(),2) );
        //    float lenthLB = sqrt( pow(topLB.x(),2) + pow(topLB.y(),2) );

        //    float angelRA = qRadiansToDegrees( atan2(topRA.x(), topRA.y()) ) ;
        //    float angelRB = qRadiansToDegrees( atan2(topRB.x(), topRB.y()) ) ;
        //    float angelLA = qRadiansToDegrees( atan2(topLA.x(), topLA.y()) ) ;
        //    float angelLB = qRadiansToDegrees( atan2(topLB.x(), topLB.y()) ) ;

        //    float doubleRotateRA = angelRA + rots;
        //    float doubleRotateRB = angelRB + rots;
        //    float doubleRotateLA = angelLA + rots;
        //    float doubleRotateLB = angelLB + rots;

        //    QVector2D newRA = QVector2D(lenthRA* sin(qDegreesToRadians(doubleRotateRA)),
        //                                lenthRA* cos(qDegreesToRadians(doubleRotateRA)));
        //    QVector2D newRB = QVector2D(lenthRB* sin(qDegreesToRadians(doubleRotateRB)),
        //                                lenthRB* cos(qDegreesToRadians(doubleRotateRB)));
        //    QVector2D newLA = QVector2D(lenthLA* sin(qDegreesToRadians(doubleRotateLA)),
        //                                lenthLA* cos(qDegreesToRadians(doubleRotateLA)));
        //    QVector2D newLB = QVector2D(lenthLB* sin(qDegreesToRadians(doubleRotateLB)),
        //                                lenthLB* cos(qDegreesToRadians(doubleRotateLB)));
    }

    QCubeLimited limited;
    limited.limitedTop.setX( fmax( fmax( fmax(topRANew.x(),topRBNew.x()) ,       fmax(topLANew.x(),topLBNew.x()) ),
                                   fmax( fmax(bottomRANew.x(),bottomRBNew.x()) , fmax(bottomLANew.x(),bottomLBNew.x()) ) ));

    limited.limitedTop.setY( fmax( fmax( fmax(topRANew.y(),topRBNew.y()) ,       fmax(topLANew.y(),topLBNew.y()) ),
                                   fmax( fmax(bottomRANew.y(),bottomRBNew.y()) , fmax(bottomLANew.y(),bottomLBNew.y()) ) ));

    limited.limitedTop.setZ( fmax( fmax( fmax(topRANew.z(),topRBNew.z()) ,       fmax(topLANew.z(),topLBNew.z()) ),
                                   fmax( fmax(bottomRANew.z(),bottomRBNew.z()) , fmax(bottomLANew.z(),bottomLBNew.z()) ) ));


    limited.limitedBottom.setX( fmin( fmin( fmin(topRANew.x(),topRBNew.x()) ,       fmin(topLANew.x(),topLBNew.x()) ),
                                   fmin( fmin(bottomRANew.x(),bottomRBNew.x()) , fmin(bottomLANew.x(),bottomLBNew.x()) ) ));

    limited.limitedBottom.setY( fmin( fmin( fmin(topRANew.y(),topRBNew.y()) ,       fmin(topLANew.y(),topLBNew.y()) ),
                                   fmin( fmin(bottomRANew.y(),bottomRBNew.y()) , fmin(bottomLANew.y(),bottomLBNew.y()) ) ));

    limited.limitedBottom.setZ( fmin( fmin( fmin(topRANew.z(),topRBNew.z()) ,       fmin(topLANew.z(),topLBNew.z()) ),
                                   fmin( fmin(bottomRANew.z(),bottomRBNew.z()) , fmin(bottomLANew.z(),bottomLBNew.z()) ) ));


//    limited.limitedBottom.setX( fmin( fmin(newLA.x(),newLB.x()) , fmin(newRA.x(),newRB.x()) ) );
//    limited.limitedBottom.setY(heights.y());
//    limited.limitedBottom.setZ( fmin( fmin(newLA.y(),newLB.y()) , fmin(newRA.y(),newRB.y()) ) );

//    qDebug()<<"LIMITFull: "<<limited.limitedTop<<" ~ \n"<<limited.limitedBottom;
    return limited;
}

QCubeLimited Collision::calcRotTriangle(QTriangle3D trng, int rots)
{
    QVector3D vecP1 = trng.p1;
    QVector3D vecP2 = trng.p2;
    QVector3D vecP3 = trng.p3;

    float lenthP1 = sqrt( pow(vecP1.x(),2) + pow(vecP1.z(),2) );
    float lenthP2 = sqrt( pow(vecP2.x(),2) + pow(vecP2.z(),2) );
    float lenthP3 = sqrt( pow(vecP3.x(),2) + pow(vecP3.z(),2) );

    float angelP1 = qRadiansToDegrees( atan2(vecP1.x(), vecP1.z()) ) ;
    float angelP2 = qRadiansToDegrees( atan2(vecP2.x(), vecP2.z()) ) ;
    float angelP3 = qRadiansToDegrees( atan2(vecP3.x(), vecP3.z()) ) ;

    float doubleRotateP1 = angelP1 + rots;
    float doubleRotateP2 = angelP2 + rots;
    float doubleRotateP3 = angelP3 + rots;

    QTriangle3D tringle;
    tringle.p1 = QVector3D(lenthP1* sin(qDegreesToRadians(doubleRotateP1)), vecP1.y(), lenthP1* cos(qDegreesToRadians(doubleRotateP1)));
    tringle.p2 = QVector3D(lenthP2* sin(qDegreesToRadians(doubleRotateP2)), vecP2.y(), lenthP2* cos(qDegreesToRadians(doubleRotateP2)));
    tringle.p3 = QVector3D(lenthP3* sin(qDegreesToRadians(doubleRotateP3)), vecP3.y(), lenthP3* cos(qDegreesToRadians(doubleRotateP3)));

    QCubeLimited limited;
    limited.limitedTop.setX( fmax( tringle.p1.x(), fmax(tringle.p2.x(), tringle.p3.x()) ) );
    limited.limitedTop.setY( fmax( tringle.p1.y(), fmax(tringle.p2.y(), tringle.p3.y()) ) );
    limited.limitedTop.setZ( fmax( tringle.p1.z(), fmax(tringle.p2.z(), tringle.p3.z()) ) );
    limited.limitedBottom.setX( fmin( tringle.p1.x(), fmin(tringle.p2.x(), tringle.p3.x()) ) );
    limited.limitedBottom.setY( fmin( tringle.p1.y(), fmin(tringle.p2.y(), tringle.p3.y()) ) );
    limited.limitedBottom.setZ( fmin( tringle.p1.z(), fmin(tringle.p2.z(), tringle.p3.z()) ) );

    return limited;
}

bool Collision::checkedCollision(QVector3D pos1, QCubeLimited limit1, QVector3D pos2, QCubeLimited limit2)
{
    if(pos1.x()+limit1.limitedTop.x()+0 > pos2.x()+limit2.limitedBottom.x() &&
       pos1.x()+limit1.limitedBottom.x() < pos2.x()+limit2.limitedTop.x()+0
       &&
       pos1.y()+limit1.limitedTop.y()+0 > pos2.y()+limit2.limitedBottom.y() &&
       pos1.y()+limit1.limitedBottom.y() < pos2.y()+limit2.limitedTop.y()+0
       &&
       pos1.z()+limit1.limitedTop.z()+0 > pos2.z()+limit2.limitedBottom.z() &&
       pos1.z()+limit1.limitedBottom.z() < pos2.z()+limit2.limitedTop.z()+0)

    {
        return true;
    }
    else
    {
        return false;
    }
}



