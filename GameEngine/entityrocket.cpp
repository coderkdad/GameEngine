#include "entityrocket.h"
#include <QDebug>
#include <QTimer>
#include "controllercollision.h"
#include <modelcar.h>
#include <modelrobot.h>
#include <modelplane.h>
#include <qmath.h>
#include "math.h"
#include "vaoentity.h"
#include <modelerth.h>
#include "testwedget.h"
#include "modelhome.h"
#include "modelhumanh.h"

EntityRocket::EntityRocket(QObject *parent) : QObject(parent)
{
}
EntityRocket::~EntityRocket()
{
    qDebug("S1");
//    delete entitySmoke;
//    entitySmoke = nullptr;
    qDebug("SS");
//    timerRocket->destroyed();
    qDebug("S");
}
void EntityRocket::setDirection(QVector3D dir)
{
    m_dir = dir;
}
QVector3D EntityRocket::getDirection()
{
    return m_dir;
}

void EntityRocket::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                                      ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada)
{
    m_erth = erth;
    m_listHome = listHome;
    m_modelRobot = modelRobot;
    m_modelCar = modelCar;
    m_modelPlane = modelPlane;
    m_modelCollada = modelCollada;
}

void EntityRocket::goBullet()
{
    timerRocket = new QTimer(this);
    connect(timerRocket, SIGNAL(timeout()), this, SLOT(timerMoveRocket()));
    timerRocket->start(m_speedTimer);
}

void EntityRocket::setStartPos(QVector3D startPos, float speedsRocket, float rngRocket, float speedTimer)
{
    m_startPos=startPos;
    m_speedRocket=speedsRocket;
    m_rngRocket=rngRocket;
    m_speedTimer=speedTimer;
}

void EntityRocket::timerMoveRocket()
{
    setPosition(QVector3D(getPosition().x() + getDirection().x()*m_speedRocket,
                          getPosition().y() + getDirection().y()*m_speedRocket,
                          getPosition().z() + getDirection().z()*m_speedRocket));

    if(isEnableSmoke())
    {
        entitySmoke->setScale(QVector3D(entitySmoke->getScale().x(),
                                        entitySmoke->getScale().y(),
                                        entitySmoke->getScale().z()+3/*getDirection().z()*(m_speedRocket)*/));
    }

    if(checkArea())
    {

        if(isCollisionEntity())
        {
            timerRocket->stop();
            emit emtDeleteRocket(this);
        }

    } else {
        timerRocket->stop();
        emit emtDeleteRocket(this);
    }


    qDebug("emt Rocket OO");
    emit emtUpdate();
}

Entity *EntityRocket::getEntitySmoke()
{
    return entitySmoke;
}

void EntityRocket::setEnableSmoke(bool state)
{
    m_enableSmoke=state;

    if(state)
    {
        entitySmoke = new Entity;
        entitySmoke->setPosition(m_startPos);
        entitySmoke->setRotation(getRotation());
        entitySmoke->setScale(QVector3D(0.2f, 0.2f, 1.0f));
    }
    else
    {

    }
}

bool EntityRocket::isEnableSmoke()
{
    return m_enableSmoke;
}

bool EntityRocket::checkArea()
{
    if(getPosition().x() < m_startPos.x()+m_rngRocket && getPosition().x() > m_startPos.x()-m_rngRocket &&
       getPosition().z() < m_startPos.z()+m_rngRocket && getPosition().z() > m_startPos.z()-m_rngRocket)
    {
        return true;
    }
    else
    {
        return  false;
    }
}

bool EntityRocket::isCollisionEntity()
{
        qDebug("DRAW is Coll");
        QVector3D possR = this->getPosition();
        QCubeLimited limittR = calcRotLimit2(this->getLimited(), this->getRotation());

        /// CHECKED Erth
        if(possR.y() < m_erth->getModelPosition().y()+m_erth->getModelLimited()->limitedTop.y() )
        {
//            return true;
        }

        /// CHECKED Home
        for(int h=0; h<m_listHome->count(); ++h)
        {
            QVector3D possHome = m_listHome->at(h)->getModelPosition();
            QCubeLimited limittHome = calcRotLimit2(m_listHome->at(h)->getModelLimited(), m_listHome->at(h)->getModelRotate());

            // Step 1: Limited
            if(checkedCollision(possR, limittR, possHome, limittHome))
            {
//                for(int rL=0; rL<entityRocket->getVecTring()->count(); ++rL)
//                {
//                    QVector<QTriangle3D> *vecR =
//                    new QVector<QTriangle3D>(*entityRocket->getVecTring());

//                    for(int rV=0; rV<vecR->count(); ++rV)
//                    {
//                        QTriangle3D triangelR = vecR->at(rV);
////                        QCubeLimited lim = calcRotTriangle(vecR->at(rV), entityRocket->getRotation().y());

//                        float topRX = fmax( triangelR.p1.x(), fmax(triangelR.p2.x(), triangelR.p3.x()) );
//                        float topRY = fmax( triangelR.p1.y(), fmax(triangelR.p2.y(), triangelR.p3.y()) );
//                        float topRZ = fmax( triangelR.p1.z(), fmax(triangelR.p2.z(), triangelR.p3.z()) );
//                        float bottomRX = fmin( triangelR.p1.x(), fmin(triangelR.p2.x(), triangelR.p3.x()) );
//                        float bottomRY = fmin( triangelR.p1.y(), fmin(triangelR.p2.y(), triangelR.p3.y()) );
//                        float bottomRZ = fmin( triangelR.p1.z(), fmin(triangelR.p2.z(), triangelR.p3.z()) );

//                        for(int hL=0; hL<listEntityHome->at(h)->getVecTring()->count(); ++hL)
//                        {
//                            QVector<QTriangle3D> *vecH =
//                            new QVector<QTriangle3D>(*listEntityHome->at(h)->getVecTring());

//                            for(int hV=0; hV<vecH->count(); ++hV)
//                            {
//    QCubeLimited test = calcRotTriangle(vecH->at(hV), listEntityHome->at(h)->getRotation().y());
////    QVector3D newP1 = test.p1;
////    QVector3D newP2 = test.p2;
////    QVector3D newP3 = test.p3;

//    float topHX = test.limitedTop.x();
//    float topHY = test.limitedTop.y();
//    float topHZ = test.limitedTop.z();
//    float bottomHX = test.limitedBottom.x();
//    float bottomHY = test.limitedBottom.y();
//    float bottomHZ = test.limitedBottom.z();

//    // Step 2: Tringles
//    if((((vecPosR.x() + bottomRX < vecPosH.x() +topHX) && (vecPosR.x() + topRX > vecPosH.x() + bottomHX))
//    || ((vecPosR.x() + bottomRX > vecPosH.x() + bottomHX) && (vecPosR.x() + topRX < vecPosH.x() + topHX)))

//    && (((vecPosR.y() + bottomRY < vecPosH.y() +topHY) && (vecPosR.y() + topRY > vecPosH.y() + bottomHY))
//    || ((vecPosR.y() + bottomRY > vecPosH.y() + bottomHY) && (vecPosR.y() + topRY < vecPosH.y() + topHY)))

//    && (((vecPosR.z() + bottomRZ < vecPosH.z() +topHZ) && (vecPosR.z() + topRZ > vecPosH.z() + bottomHZ))
//    || ((vecPosR.z() + bottomRZ > vecPosH.z() + bottomHZ) && (vecPosR.z() + topRZ < vecPosH.z() + topHZ))))
//    {
//        qDebug("Coll Homes");
//        timerR->stop();
//            emit emtCollRocketWithHome(entityRocket, listEntityHome->at(h)); // Here Action: setState Home form Good to Bad
//        return;

//    }
//                            }
//                        }
//                    }
//                }
                return true;

            }//End Limit
        }

        /// CHECKED Car
        QVector3D possCar = m_modelCar->getModelPosition();
        QCubeLimited limittCar = calcRotLimit2(m_modelCar->getModelLimited(), m_modelCar->getModelRotate());
        if(checkedCollision(possR, limittR, possCar, limittCar))
        {
//                timerR->stop();
//                emit emtCollRocketWithCar(entityRocket, m_modelCar);
            return  true;
        }

        /// CHECKED Human
//        if(!m_modelRobot->checkOn())
//        {
//            QVector3D possHuman = m_modelRobot->getModelPosition();
//            QCubeLimited limittHuman = calcRotLimit2(m_modelRobot->getModelLimited(), m_modelRobot->getModelRotate());

//            if(checkedCollision(possR, limittR, possHuman, limittHuman))
//            {
////                timerR->stop();
////                emit emtCollRocketWithHuman(entityRocket, m_modelHuman);
////                return true;
//            }
//        }

        /// CHECKED Plane
        QVector3D possPlaner = m_modelPlane->getModelPosition();
        QCubeLimited limittPlane = calcRotLimit2(m_modelPlane->getModelLimited(), m_modelPlane->getModelRotate());
        if(checkedCollision(possR, limittR, possPlaner, limittPlane))
        {
//                timerR->stop();
//                emit emtCollRocketWithCar(entityRocket, m_modelCar);
            return  true;
        }

        return false;
}

bool EntityRocket::checkedCollision(QVector3D pos1, QCubeLimited limit1, QVector3D pos2, QCubeLimited limit2)
{
    if(pos1.x()+limit1.limitedTop.x() > pos2.x()+limit2.limitedBottom.x() &&
       pos1.x()+limit1.limitedBottom.x() < pos2.x()+limit2.limitedTop.x()
       &&
       pos1.y()+limit1.limitedTop.y() > pos2.y()+limit2.limitedBottom.y() &&
       pos1.y()+limit1.limitedBottom.y() < pos2.y()+limit2.limitedTop.y()
       &&
       pos1.z()+limit1.limitedTop.z() > pos2.z()+limit2.limitedBottom.z() &&
       pos1.z()+limit1.limitedBottom.z() < pos2.z()+limit2.limitedTop.z())
    {
        qDebug("Bullete Shutters >>>");
        return true;
    } else {
        return false;
    }
}

QVector3D EntityRocket::calcRotPosition2(QVector3D pos, QVector3D rots)
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
QCubeLimited EntityRocket::calcRotLimit2(QCubeLimited *limit, QVector3D rots)
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



