#include "entitywedge.h"
#include <QTimer>
#include "modelrobot.h"
#include "modelcar.h"
#include "modelplane.h"
#include <qmath.h>
#include "math.h"
#include "modelerth.h"

#include "vaoentity.h"
#include "modelhome.h"
#include "modelhumanh.h"

EntityWedge::EntityWedge(QObject *parent) : QObject(parent)
{
}

EntityWedge::~EntityWedge()
{

}

void EntityWedge::settingWedge()
{

    timerWedge = new QTimer;
    connect(timerWedge, SIGNAL(timeout()), this, SLOT(timerMoveWedge()));
//    connect(timerWedge, &QTimer::timeout, this, &EntityWedge::timerMoveWedge);
}

void EntityWedge::setDirection(QVector3D dir)
{
    m_dir = dir;
}

QVector3D EntityWedge::getDirection()
{
    return m_dir;
}

void EntityWedge::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                        ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelHuman, ModelHumanH *modelCollada)
{
    m_erth = erth;
    m_listHome = listHome;
    m_modelHuman = modelHuman;
    m_modelCar = modelCar;
    m_modelPlane = modelPlane;
    m_modelCollada = modelCollada;

}

void EntityWedge::setStartPos(QVector3D startPos, float speedWedge, float rngSpace, float speedTimer)
{
    m_startPos = startPos;
    m_speedWedge = speedWedge;
    m_rngSpace = rngSpace;
    m_speedTimer = speedTimer;
}

void EntityWedge::goWedge()
{
    if(!timerWedge->isActive())
    {
        qDebug("Cr No");
        timerWedge->start(m_speedTimer);
    }
    else
    {
        qDebug()<<"Cr Ys: "<<&timerWedge;
    }

}

void EntityWedge::timerMoveWedge()
{
    setPosition(QVector3D(getPosition().x() + getDirection().x()*m_speedWedge,
                          getPosition().y() + getDirection().y()*m_speedWedge,
                          getPosition().z() + getDirection().z()*m_speedWedge));

    if(checkArea())
    {
        if(isCollisionEntity())
        {
            qDebug("Ys Col We");
            timerWedge->stop();
//            emit emtDeleteWedge(this); 
        }

    } else {
        timerWedge->stop();
        emit emtDeleteWedge(this);
    }

    emit emtUpdate();
}

bool EntityWedge::checkArea()
{
    if(getPosition().x() < m_startPos.x()+m_rngSpace && getPosition().x() > m_startPos.x()-m_rngSpace &&
       getPosition().z() < m_startPos.z()+m_rngSpace && getPosition().z() > m_startPos.z()-m_rngSpace)
    {
        return true;
    }
    else
    {
        return  false;
    }
}

bool EntityWedge::isCollisionEntity()
{
        QVector3D possMe = this->getPosition();
        QCubeLimited limittMe = calcRotLimit2(this->getLimited(), this->getRotation());

        QVector3D *posssMe = new QVector3D(possMe);

        /// CHECKED Erth
        if(possMe.y() < m_erth->getModelPosition().y()+m_erth->getModelLimited()->limitedTop.y() )
        {
//            return true;
        }

        /// CHECKED Home
        for(int h=0; h<m_listHome->count(); ++h)
        {
            QVector3D possHome = m_listHome->at(h)->getModelPosition();
            QCubeLimited limittHome = calcRotLimit2(m_listHome->at(h)->getModelLimited(), m_listHome->at(h)->getModelRotate());

            if(checkedCollision(possMe, limittMe, possHome, limittHome))
            {
                emit emtCollisionThat("Home "+QString::number(h), posssMe);
                return true;
            }
        }

        /// CHECKED Car
        QVector3D possCar = m_modelCar->getModelPosition();
        QCubeLimited limittCar = calcRotLimit2(m_modelCar->getModelLimited(), m_modelCar->getModelRotate());
        if(checkedCollision(possMe, limittMe, possCar, limittCar))
        {
            emit emtCollisionThat("Car", posssMe);
            return  true;
        }

        /// CHECKED Human
        if(!m_modelHuman->checkOn())
        {
            QVector3D possHuman = m_modelHuman->getModelPosition();
            QCubeLimited limittHuman = calcRotLimit2(m_modelHuman->getModelLimited(), m_modelHuman->getModelRotate());

            if(checkedCollision(possMe, limittMe, possHuman, limittHuman))
            {
//                return true;
            }
        }

        /// CHECKED Plane
        QVector3D possPlaner = m_modelPlane->getModelPosition();
        QCubeLimited limittPlane = calcRotLimit2(m_modelPlane->getModelLimited(), m_modelPlane->getModelRotate());
        if(checkedCollision(possMe, limittMe, possPlaner, limittPlane))
        {
            emit emtCollisionThat("Plane", posssMe);
            return  true;
        }

        return false;
}

bool EntityWedge::checkedCollision(QVector3D pos1, QCubeLimited limit1, QVector3D pos2, QCubeLimited limit2)
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
        return true;
    } else {
        return false;
    }
}

QVector3D EntityWedge::calcRotPosition2(QVector3D pos, QVector3D rots)
{
    QVector3D posPoint = pos;
    QVector3D rotEntity = rots;

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
QCubeLimited EntityWedge::calcRotLimit2(QCubeLimited *limit, QVector3D rots)
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


    return limited;
}



