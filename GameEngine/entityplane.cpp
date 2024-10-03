#include "entityplane.h"
#include "math.h"
#include "qmath.h"
#include <QTimer>
#include <QDebug>

EntityPlane::EntityPlane(QObject *parent) : QObject(parent)
{
    //Move Plane
    timerPlane = new QTimer(this);
    connect(timerPlane, SIGNAL(timeout()), this, SLOT(movePlane()));

    //Rotate Plane
    timerRotatePlane = new QTimer(this);
    connect(timerRotatePlane, SIGNAL(timeout()), this, SLOT(rotatePlane()));

}
EntityPlane::~EntityPlane()
{
}

void EntityPlane::setDirection(QVector3D dir)
{
    m_dir = dir;
}
QVector3D EntityPlane::getDirection()
{
    return m_dir;
}

void EntityPlane::setStatePlane(bool state)
{
    if(state)
    {
        timerPlane->start(50);
    }
    else
    {
        timerPlane->stop();
    }
}

void EntityPlane::setAxisRight(double axis)
{
//    qDebug()<<"AXIS: "<<axis;
    m_AxisRightX=axis;

    ///Rotate Plane Timer
    if(axis >0.1f || axis < -0.1f)
    {
        float sumtMode = axis*100;
        resultAngelZ = -(0.25f*sumtMode);
    }
    else
    {
        resultAngelZ=0;
    }

    if(resultAngelZ!= 0.0f ||  angelPlane_Z!= 0.0f)
    {
        if(!timerRotatePlane->isActive())
        {
            timerRotatePlane->start(10);
        }
    }
    else
    {
        if(timerRotatePlane->isActive())
        {
            timerRotatePlane->stop();
        }
    }

}
void EntityPlane::setAxisLeft(float axis)
{
    m_AxisLeftY=axis;
}
void EntityPlane::rotatePlane()
{
    if (angelPlane_Z > resultAngelZ)
    {
        angelPlane_Z-=0.5f;
    }
    else if (angelPlane_Z < resultAngelZ)
    {
        angelPlane_Z+=0.5f;
    }
    else
    {
        timerRotatePlane->stop();
//        angelPlane_Z=0;
//        resultAngelZ=0;
    }
}

void EntityPlane::speedUP()
{
    if(!timerPlane->isActive())
    {
        timerPlane->start(20);
        emit emtStatePlane(timerPlane->isActive());
    }

    if(speedPlane<1.0)
    {
        if(speedPlane>0.1f)
        {
            speedPlane+=0.1;
        }
    }
    else
    {
        speedPlane=1.0;
    }
}
void EntityPlane::speedDown()
{
    if(speedPlane>0)
    {
        speedPlane-=0.1;
        if(speedPlane<0.1)
        {
            timerPlane->stop();
            speedPlane=0;
            emit emtStatePlane(timerPlane->isActive());
        }
    }
    else
    {
        speedPlane=0;
        timerPlane->stop();
        if(timerPlane->isActive())
        {
            emit emtStatePlane(timerPlane->isActive());
        }
    }
}

void EntityPlane::testCalc(QVector3D posMe, QVector3D posEnemy)
{
    /// Step Calc Tan
    QVector2D vectorAC = QVector2D( posMe.x()-posEnemy.x(), posMe.z()-posEnemy.z() );
    double tans=0;
    if(posEnemy.z()<posMe.z())
    {
        tans = abs( qRadiansToDegrees( atan( vectorAC.y()/vectorAC.x() ) ) );
        if(posEnemy.x()<posMe.x())
        { tans=360-tans; }
        else
        { tans+=180; }
    }
    else
    {
        tans = abs( qRadiansToDegrees( atan( vectorAC.y()/vectorAC.x() ) ) );
        if(posEnemy.x()<posMe.x())
        { }
        else
        { tans=180-tans; }
    }
    tans-=90;
    angelPlane_Y=tans;

    /// Check Value Angel
    if(angelPlane_Y>=360)
    {
        angelPlane_Y = fmod(angelPlane_Y,360);
    }
    else if(angelPlane_Y<0)
    {
        angelPlane_Y = (360+angelPlane_Y);
    }
    setRotation(QVector3D(0,angelPlane_Y,0));
    checkMove=false;
    speedPlane=0.1;
    movePlaneRotation();
}

void EntityPlane::movePlane()
{
    if(speedPlane<0.1f)
    {
        speedPlane+=0.001f;
    }
    movePlaneRotation();
}
void EntityPlane::movePlaneRotation()
{
    /// Angelse
    if(checkMove)
    {
        angelPlane_Y-=m_AxisRightX;

        if(angelPlane_Y<0)
        { angelPlane_Y = 360-m_AxisRightX; }
        else if (angelPlane_Y>360)
        { angelPlane_Y = m_AxisRightX; }

        float testAngelX = angelPlane_X-m_AxisLeftY;
        if(testAngelX > -45 && testAngelX < 45)
        {
            angelPlane_X-=m_AxisLeftY;
        }
    }

    double mode_x = fmod(angelPlane_X,360.0f);
    double mode_y = fmod(angelPlane_Y,360.0f);
    float mode_z = fmod(angelPlane_Z,360.0f);

    /// Moved
    double newX = sin(mode_y*double(M_PI)/180) * cos(mode_x*double(M_PI)/180);
    double newY = sin(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180);

    if(qDegreesToRadians(mode_y)>=double(M_PI))
    {
        if(mode_y >= 315 || mode_y < 45 || (mode_y > 135 && mode_y <= 225))
        {
            if(mode_y>315)
            {
                newY = -cos(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180); //##
            }
            else
            {
                newY = cos(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180);
            }
        }
        else
        {
            newY = sin(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180); //##
        }
    }
    else if (qDegreesToRadians(mode_y)<double(M_PI))
    {
        if(mode_y > 315 || mode_y < 45 || (mode_y > 135 && mode_y <225))
        {
            if(mode_y>=135 && mode_y<180)
            {
                newY = cos(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180);
            }
            else
            {
                newY = -cos(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180); //##
            }
        }
        else
        {
            newY = -sin(mode_y*double(M_PI)/180) * sin(mode_x*double(M_PI)/180); //##
        }
    }
    else
    {
        newY = 0;
    }

    double newZ = cos(mode_y*double(M_PI)/180);

    /// FILL
    setPosition(QVector3D(getPosition().x()-(speedPlane*newX),
                          getPosition().y()-(speedPlane*newY),
                          getPosition().z()-(speedPlane*newZ)));
    setDirection(QVector3D(newX,newY,newZ));

    if(checkMove)
    {
        setRotation (QVector3D(mode_x, mode_y, mode_z));
    }

    emit emtUpdate();
}
