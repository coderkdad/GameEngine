#include "modelplane.h"
#include "entity.h"
#include "math.h"
#include "qmath.h"
#include <QTimer>
#include <QDebug>
#include "vaoentity.h"
#include <QOpenGLShaderProgram>
#include <controllercollision.h>
#include <controllerrocket.h>
#include <modelcar.h>
#include <modelrobot.h>
#include <entityrocket.h>

ModelPlane::ModelPlane(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) :
    QObject(parent),
    m_shader(shader),
    m_vao(vao),
    listEntityInfo(vao->getListInfo())
{


//    float lenthss = 3.0f;
//    float modsX = 90;
//    float newssY = lenthss* sin(qDegreesToRadians(modsX));
//    float newssZ = lenthss*-cos(qDegreesToRadians(modsX));
//    qDebug()<<"MODES: "<<newssY<<" "<<newssZ;

//    float modsY = 45;
//    float newsssX = fabs(newssZ)* cos(qDegreesToRadians(modsY));
//    float newsssZ = fabs(newssZ)*-sin(qDegreesToRadians(modsY));
//    qDebug()<<"MODES 2 : "<<newsssX<<" "<<newsssZ;
//    qDebug()<<"Final: "<<QVector3D(newsssX,newssY,newsssZ) ;

    //Move Plane
    timerPlane = new QTimer(this);
    connect(timerPlane, SIGNAL(timeout()), this, SLOT(movePlane()));

    //Rotate Plane
    timerRotatePlane = new QTimer(this);
    connect(timerRotatePlane, SIGNAL(timeout()), this, SLOT(rotateZPlane()));

    //Close Wheels Plane
    timerClose = new QTimer(this);
    connect(timerClose, SIGNAL(timeout()), this, SLOT(moveCloseWheels()));

    connect(this, &ModelPlane::emtStopTimer, this, &ModelPlane::slotStopTimer);
}

ModelPlane::~ModelPlane()
{
    for(int l=0; l<listEntity.count(); ++l)
    {
        Entity *e = listEntity.takeAt(l);
        delete e;
        e = nullptr;
    }

    /// Un Link Another Pointer
    m_shader = nullptr;
    m_vao = nullptr;
    listEntityInfo = nullptr;
    // Control
    m_controlCol = nullptr;

    timerClose->deleteLater();
    timerPlane->deleteLater();
    timerRotatePlane->deleteLater();

}

void ModelPlane::createEntity()
{
    entityPlaneBudy = new Entity;
    entityPlaneBudy->setPosition(QVector3D(0,0,0));
//    entityPlaneBudy->setRotation(modelRot);
    entityPlaneBudy->setInfo(listEntityInfo->at(0), true);
    listEntity.append(entityPlaneBudy);

    entityPlaneWheel1 = new Entity; // Left
    entityPlaneWheel1->setPosition(QVector3D(-0.3,-0.25,0));
//    entityPlaneWheel1->setRotation(modelRot);
    entityPlaneWheel1->setInfo(listEntityInfo->at(1), true);
    listEntity.append(entityPlaneWheel1);

    entityPlaneWheel2 = new Entity; // Right
    entityPlaneWheel2->setPosition(QVector3D(0.3,-0.25,0));
//    entityPlaneWheel2->setRotation(modelRot);
    entityPlaneWheel2->setInfo(listEntityInfo->at(2), true);
    listEntity.append(entityPlaneWheel2);

    entityPlaneGlass = new Entity;
    entityPlaneGlass->setPosition(QVector3D(0,0.26,-1.08));
//    entityPlaneGlass->setRotation(modelRot);
    entityPlaneGlass->setInfo(listEntityInfo->at(3), true);
    listEntity.append(entityPlaneGlass);
}

QList<Entity *> ModelPlane::getListEntity()
{
    return listEntity;
}

void ModelPlane::setDirection(QVector3D dir)
{
    m_dir = dir;
}
QVector3D ModelPlane::getDirection()
{
    return m_dir;
}


void ModelPlane::setStatePlane(bool state)
{
    if(state)
    {
        timerPlane->start(100);
    }
    else
    {
        timerPlane->stop();
    }
}

void ModelPlane::setAxisRightX(double axis)
{

    ///Rotate Plane Timer
    if(axis >0.1f || axis < -0.1f)
    {
        m_AxisRightX=axis;
        float sumtMode = axis*200;
        resultAngelZ = -(0.25*sumtMode);
    }
    else
    {
//        qDebug("ZEROOO");
        m_AxisRightX=0.0f;
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
void ModelPlane::setAxisLeft(float axis)
{
    if(axis > 0.1f || axis < -0.1f)
    {
        m_AxisLeftY=axis;
    }
    else
    {
        m_AxisLeftY=0.0f;
    }
}
void ModelPlane::rotateZPlane()
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

void ModelPlane::speedUP()
{
    checking = true;
    speedPlane +=  1;

    if(speedPlane == 0)
    {
        timerPlane->stop();
    }
    else
    {
        if(speedPlane > 10)
        {
            speedPlane = 10;
        }

        if(!timerPlane->isActive())
        {
            timerPlane->start(40);
        }
    }
}
void ModelPlane::speedDown()
{
    checking = true;
    speedPlane -= 1;

    if(speedPlane == 0)
    {
        timerPlane->stop();
    }
    else
    {
        if(speedPlane < -10)
        {
            speedPlane = -10;
        }

        if(!timerPlane->isActive())
        {
            timerPlane->start(100);
        }
    }
}

void ModelPlane::closeWheels()
{
    if(!timerClose->isActive())
    {
        timerClose->start(200);
    }
}
void ModelPlane::moveCloseWheels()
{
    float mada=0.01f;
    if(stateOpenWheel)
    {
        entityPlaneWheel1->setPosition(QVector3D(entityPlaneWheel1->getPosition().x()+mada,
                                                 entityPlaneWheel1->getPosition().y()+mada,
                                                 entityPlaneWheel1->getPosition().z() ));
        entityPlaneWheel2->setPosition(QVector3D(entityPlaneWheel2->getPosition().x()-mada,
                                                 entityPlaneWheel2->getPosition().y()+mada,
                                                 entityPlaneWheel2->getPosition().z() ));
        if(entityPlaneWheel1->getPosition().y() > 0.1f)
        { timerClose->stop(); stateOpenWheel = false; }
    }
    else {
        entityPlaneWheel1->setPosition(QVector3D(entityPlaneWheel1->getPosition().x()-mada,
                                                 entityPlaneWheel1->getPosition().y()-mada,
                                                 entityPlaneWheel1->getPosition().z() ));
        entityPlaneWheel2->setPosition(QVector3D(entityPlaneWheel2->getPosition().x()+mada,
                                                 entityPlaneWheel2->getPosition().y()-mada,
                                                 entityPlaneWheel2->getPosition().z() ));
        if(entityPlaneWheel1->getPosition().y() <= -0.25f)
        { timerClose->stop(); stateOpenWheel = true; }
    }

}

void ModelPlane::slotStopTimer()
{
    timerPlane->stop();
}

void ModelPlane::movePlane()
{
//    if(speedPlaneStart<0.1f && speedPlaneStart >= 0.0f)
//    {
//        speedPlaneStart+=0.001f;
//    }
//    if(speedPlaneStart>-0.1f && speedPlaneStart <= 0.0f)
//    {
//        speedPlaneStart-=0.001f;
//    }

    movePlaneRotation();
}
void ModelPlane::movePlaneRotation()
{
//    QVector3D posOldP = getModelPosition();
    /// Angel
    angelPlane_Y-=m_AxisRightX;

    if(angelPlane_Y<0.00f)
    { angelPlane_Y = 360.0f+angelPlane_Y; }
    else if (angelPlane_Y>=360.00f)
    { angelPlane_Y = angelPlane_Y-360.0f; }

    float testAngelX = angelPlane_X-m_AxisLeftY;
    if(testAngelX > -90 && testAngelX < 90)
    {
        angelPlane_X-=m_AxisLeftY;
    }

    float rotX = fmod(angelPlane_X,360.0f);
    float rotY = fmod(angelPlane_Y,360.0f);
    float rotZ = fmod(angelPlane_Z,360.0f);

    /// Move
    QVector3D vecOnes = calcRotatePosition(QVector3D(0,0,-1), QVector3D(rotX,rotY,rotZ));

    setModelPosition(QVector3D(getModelPosition().x()+( (speedPlane/10.0f) *vecOnes.x()),
                               getModelPosition().y()+( (speedPlane/10.0f) *vecOnes.y()),
                               getModelPosition().z()+( (speedPlane/10.0f) *vecOnes.z())));
    setModelRotate(QVector3D(rotX,rotY,rotZ));
    setDirection(QVector3D(vecOnes.x(),vecOnes.y(),vecOnes.z()));

    if(checking)
    {
//        m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
//                                getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
    }

    emit emtUpdate();
}

void ModelPlane::stopPlane()
{
    timerPlane->stop();
    speedPlane = 0;
}

void ModelPlane::sendCollisionPlane(QVector3D posA, QVector3D rotA)
{
    if(checking)
    {
        checking = false;
        emit emtStopTimer();
//        qDebug("UU: Plane");
        setModelPosition(posA);
        setModelRotate(rotA);

        speedPlane = -speedPlane;
        movePlane();
        speedPlane = 0;
    }

}

void ModelPlane::setControl(ControllerCollision *controlCol)
{
    m_controlCol=controlCol;
}

void ModelPlane::draw(QMatrix4x4 matrixProCam)
{
    QMatrix4x4 matrixModelCar;
    matrixModelCar.setToIdentity();
    matrixModelCar.translate(getModelPosition());
    matrixModelCar.rotate(getModelRotate().y(),0,1,0);
    matrixModelCar.rotate(getModelRotate().z(),0,0,1);
    matrixModelCar.rotate(getModelRotate().x(),1,0,0);

    for(int i=0; i<getListEntity().count(); ++i)
    {
        QMatrix4x4 matrixEntityCar;
        matrixEntityCar.setToIdentity();
        matrixEntityCar.translate(getListEntity().at(i)->getPosition());
        matrixEntityCar.rotate(getListEntity().at(i)->getRotation().y(), 0, 1, 0);
        matrixEntityCar.rotate(getListEntity().at(i)->getRotation().z(), 0, 0, 1);
        matrixEntityCar.rotate(getListEntity().at(i)->getRotation().x(), 1, 0, 0);

        m_shader->bind();
        m_shader->setUniformValue("mvMatrix", (matrixModelCar*matrixEntityCar)); /// LIGHTING
        m_shader->setUniformValue("pMatrix", matrixProCam);
        m_shader->setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
        m_shader->setUniformValue("u_lightPower", 1.0f);
        m_shader->setUniformValue("fragFlag", 1.0f);
        m_shader->setUniformValue("texturTransparency", 1.0f);
        m_vao->getListInfo()->at(i)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(i)->numVertex);
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(i)->vao->release();
        m_shader->release();
    }
}

QVector3D ModelPlane::calcRotatePosition(QVector3D pos, QVector3D rots)
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
    QVector3D posEntityNew2 = QVector3D(lenthVec3* cos(qDegreesToRadians(doubleRotVec3)),
                                        lenthVec3* sin(qDegreesToRadians(doubleRotVec3)),
                                        posEntityNew.z());

    /// y
    float lenthVec2 = sqrt( pow(posEntityNew2.x(),2) + pow(posEntityNew2.z(),2) );
    float angelVec2 = qRadiansToDegrees( atan2(posEntityNew2.x(), posEntityNew2.z()) ) ;
    float doubleRotVec2 = angelVec2+rotEntity.y();
    QVector3D posEntityNew3 = QVector3D(lenthVec2* sin(qDegreesToRadians(doubleRotVec2)),
                                        posEntityNew2.y(),
                                        lenthVec2* cos(qDegreesToRadians(doubleRotVec2)));

    return posEntityNew3;
}





















