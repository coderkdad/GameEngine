#include "modelrobot.h"
#include "entity.h"
#include "math.h"
#include "qmath.h"
#include <QTimer>
#include <QDebug>
#include "vaoentity.h"
#include "modelcar.h"
#include "modelplane.h"
#include <QOpenGLShaderProgram>
#include <controllercollision.h>
#include <QThread>
#include <QElapsedTimer>
#include <testwedget.h>

#define SPEED_T_HUMAN 30.0f
#define SPEED_T_ANIMI1 20.0f
#define SPEED_T_ANIMI2 40.0f


ModelRobot::ModelRobot(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) :
    QObject(parent),
    m_shader(shader),
    m_vao(vao),
    listEntityInfo(vao->getListInfo()),
    modelLimit(vao->getLimited())
{
    timerHumanGo = new QTimer(this);
    connect(timerHumanGo, &QTimer::timeout, this, &ModelRobot::moveHuman);

    timerAnimitionMove= new QTimer(this);
    connect(timerAnimitionMove, &QTimer::timeout, this, &ModelRobot::animitionMove);

    timerHumanJump = new QTimer(this);
    connect(timerHumanJump, &QTimer::timeout, this, &ModelRobot::slotTimerJump);

    timerAnimitionDrop = new QTimer(this);
    connect(timerAnimitionDrop, &QTimer::timeout, this, &ModelRobot::slotTimerDrop);
    connect(this, &ModelRobot::emtStartTimerDrop, this, &ModelRobot::slotStartTimerDrop);
    connect(this, &ModelRobot::emtStopTimerMove, this, &ModelRobot::slotStopTimerMove);
    connect(this, &ModelRobot::emtStopJump, this, &ModelRobot::slotStopJump);

    timerRotLink = new QTimer(this);
    connect(timerRotLink, &QTimer::timeout, this, &ModelRobot::slotRotLink);

}

ModelRobot::~ModelRobot()
{
    qDebug("DeHum");
//    c_test->deleteLater();
//    c_test = nullptr;

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
    modelLimit = nullptr;
    // Control
    m_controlCol = nullptr;
    // Entitys
    m_erth = nullptr;
    m_listHome = nullptr;
    m_modelCar = nullptr;
    m_modelPlane = nullptr;
    ///

    // Timer
    if(timerHumanGo->isActive())
        timerHumanGo->stop();
    timerHumanGo->deleteLater();

    if(timerRotLink->isActive())
        timerRotLink->stop();
    timerRotLink->deleteLater();

    if(timerHumanJump->isActive())
        timerHumanJump->stop();
    timerHumanJump->deleteLater();

    if(timerAnimitionDrop->isActive())
        timerAnimitionDrop->stop();
    timerAnimitionDrop->deleteLater();

    if(timerAnimitionMove->isActive())
        timerAnimitionMove->stop();
    timerAnimitionMove->deleteLater();

}

void ModelRobot::createEntity()
{
    //============= BUDY =============//

    entityHumanBelly = new Entity;
    entityHumanBelly->setPosition(QVector3D(0,0.0f,0)); // 0.61= 286+0.321 [limitBottom_FootTop]+[limitBottom_FootBottom]
    entityHumanBelly->setInfo(listEntityInfo->at(0), true);
    listEntity.append(entityHumanBelly);

    entityHumanChest = new Entity;
    entityHumanChest->setPosition(QVector3D(0, entityHumanBelly->getLimited()->limitedTop.y()+0.01f, 0));
    entityHumanChest->setInfo(listEntityInfo->at(1), true);
    listEntity.append(entityHumanChest);


    entityHumanNeck = new Entity;
    entityHumanNeck->setPosition(QVector3D(0, entityHumanChest->getLimited()->limitedTop.y(), 0));
    entityHumanNeck->setInfo(listEntityInfo->at(2), true);
    listEntity.append(entityHumanNeck);

    entityHumanHead = new Entity;
    entityHumanHead->setPosition(QVector3D(0, entityHumanNeck->getLimited()->limitedTop.y()+0.01f, 0));
    entityHumanHead->setInfo(listEntityInfo->at(3), true);
    listEntity.append(entityHumanHead);


    //============= HAND =============//
    /// Hand R
    entityHumanHandRT = new Entity;
    entityHumanHandRT->setPosition(QVector3D(entityHumanChest->getLimited()->limitedTop.x()+0.03f, entityHumanChest->getLimited()->limitedTop.y()-0.07f, 0.0f));
    entityHumanHandRT->setRotation(QVector3D(-15.0f, 0.0f, 8.0f));
    entityHumanHandRT->setInfo(listEntityInfo->at(4), true);
    listEntity.append(entityHumanHandRT);

    entityHumanHandRB = new Entity;
    entityHumanHandRB->setPosition(QVector3D(0,entityHumanHandRT->getLimited()->limitedBottom.y()-0.03f,0)); /////////////
    entityHumanHandRB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
    entityHumanHandRB->setInfo(listEntityInfo->at(5), true);
    listEntity.append(entityHumanHandRB);

    /// Hand L
    entityHumanHandLT = new Entity;
    entityHumanHandLT->setPosition(QVector3D(entityHumanChest->getLimited()->limitedBottom.x()-0.03f, entityHumanChest->getLimited()->limitedTop.y()-0.07f, 0.0f));
    entityHumanHandLT->setRotation(QVector3D(0.0f, 0.0f, -30.0f));
    entityHumanHandLT->setInfo(listEntityInfo->at(6), true);
    listEntity.append(entityHumanHandLT);

    entityHumanHandLB = new Entity;
    entityHumanHandLB->setPosition(QVector3D(0,entityHumanHandLT->getLimited()->limitedBottom.y()-0.03f,0));
    entityHumanHandLB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
    entityHumanHandLB->setInfo(listEntityInfo->at(7), true);
    listEntity.append(entityHumanHandLB);


    //============= FOOT =============//
    /// Foot R
    entityHumanFootRT = new Entity;
    entityHumanFootRT->setPosition(QVector3D(0.13f, 0.0f, 0.0f));
    entityHumanFootRT->setInfo(listEntityInfo->at(8), true);
    listEntity.append(entityHumanFootRT);

    entityHumanFootRB = new Entity;
    entityHumanFootRB->setPosition(QVector3D(0.,entityHumanFootRT->getLimited()->limitedBottom.y(),0));
    entityHumanFootRB->setInfo(listEntityInfo->at(9), true);
    listEntity.append(entityHumanFootRB);

    /// Foot L
    entityHumanFootLT = new Entity;
    entityHumanFootLT->setPosition(QVector3D(-0.13, 0.0f, 0.0f));
    entityHumanFootLT->setInfo(listEntityInfo->at(10), true);
    listEntity.append(entityHumanFootLT);

    entityHumanFootLB = new Entity;
    entityHumanFootLB->setPosition(QVector3D(0,entityHumanFootLT->getLimited()->limitedBottom.y(),0));
    entityHumanFootLB->setInfo(listEntityInfo->at(11), true);
    listEntity.append(entityHumanFootLB);


    entityHumanBelly->setPosition(QVector3D(0,
                                            fabs(entityHumanFootRT->getLimited()->limitedBottom.y())+
                                            fabs(entityHumanFootRB->getLimited()->limitedBottom.y()),
                                            0));
    setModelLimited();
}
void ModelRobot::setModelPosition(QVector3D pos)
{
    modelPos = pos;
    emit emtUpdate(); // Solution Grav & posGun
//    emit modelPosChanged();
}
void ModelRobot::setModelRotate(QVector3D rot)
{
    modelRot = rot;
    emit emtUpdate();
//    emit modelRotChanged();
}
void ModelRobot::setModelScale(QVector3D scal)
{
    modelScal = scal;
}
void ModelRobot::setDirection(QVector3D dir)
{
    modelDirect = dir;
}
void ModelRobot::setModelLimited()
{
    for(int i=0; i<listEntity.count(); ++i)
    {
        QVector3D posE = listEntity.at(i)->getPosition();
        QCubeLimited limitE = calcRotLimit2(listEntity.at(i)->getLimited(), listEntity.at(i)->getRotation());
        if(i==0)
        {

            modelLimit->limitedTop = QVector3D(posE.x() + limitE.limitedTop.x(),
                                               posE.y() + limitE.limitedTop.y(),
                                               posE.z() + limitE.limitedTop.z());
            modelLimit->limitedBottom = QVector3D(posE.x() + limitE.limitedBottom.x(),
                                                  posE.y() + limitE.limitedBottom.y(),
                                                  posE.z() + limitE.limitedBottom.z());

        } else {

            QVector3D posEParent = QVector3D(0,0,0);
            QVector3D rotEParent = QVector3D(0,0,0);
            if(i==1 || i==8 || i==10){ // Chest+FootTop[ R&L ]
                posEParent = entityHumanBelly->getPosition();
                rotEParent = entityHumanBelly->getRotation();
            }else if(i==2 || i==4 || i==6){ // Neck + HandTop[ R&L]
                posEParent = entityHumanBelly->getPosition() + entityHumanChest->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanChest->getRotation();
            }else if(i==3 ){ // Head
                posEParent = entityHumanBelly->getPosition() + entityHumanChest->getPosition() + entityHumanNeck->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanChest->getRotation() + entityHumanNeck->getRotation();
            }else if(i==5 ){ // HandBottom R
                posEParent = entityHumanBelly->getPosition() + entityHumanChest->getPosition() + entityHumanHandRT->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanChest->getRotation() + entityHumanHandRT->getRotation();
            }else if(i==7 ){ // HandBottom L
                posEParent = entityHumanBelly->getPosition() + entityHumanChest->getPosition() + entityHumanHandLT->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanChest->getRotation() + entityHumanHandLT->getRotation();
            } else if(i==9){ // FootBottom R
                posEParent = entityHumanBelly->getPosition() + entityHumanFootRT->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanFootRT->getRotation();
            } else if(i==11){ // FootBottom L
                posEParent = entityHumanBelly->getPosition() + entityHumanFootLT->getPosition();
                rotEParent = entityHumanBelly->getRotation() + entityHumanFootLT->getRotation();
            }

    modelLimit->limitedTop = QVector3D(fmax((posE.x()+posEParent.x()) + limitE.limitedTop.x(), modelLimit->limitedTop.x()),
                                       fmax((posE.y()+posEParent.y()) + limitE.limitedTop.y(), modelLimit->limitedTop.y()),
                                       fmax((posE.z()+posEParent.z()) + limitE.limitedTop.z(), modelLimit->limitedTop.z()));

    modelLimit->limitedBottom = QVector3D(fmin((posE.x()+posEParent.x()) + limitE.limitedBottom.x(), modelLimit->limitedBottom.x()),
                                          fmin((posE.y()+posEParent.y()) + limitE.limitedBottom.y(), modelLimit->limitedBottom.y()),
                                          fmin((posE.z()+posEParent.z()) + limitE.limitedBottom.z(), modelLimit->limitedBottom.z()));
        }
    }
//    qDebug()<<"Top:    "<<modelLimit->limitedTop;
//    qDebug()<<"BOTTOM: "<<modelLimit->limitedBottom;

}
QVector3D ModelRobot::getModelPosition()
{
    return modelPos;
}
QVector3D ModelRobot::getModelRotate()
{
    return modelRot;
}
QVector3D ModelRobot::getModelScale()
{
    return modelScal;
}
QVector3D ModelRobot::getDirection()
{
    return modelDirect;
}
QCubeLimited *ModelRobot::getModelLimited()
{
    return modelLimit;
}
QList<Entity *> ModelRobot::getListEntity()
{
    return listEntity;
}

void ModelRobot::draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam)
{
    QMatrix4x4 matrixModelHuman;
    matrixModelHuman.setToIdentity();
    matrixModelHuman.translate(getModelPosition());
    matrixModelHuman.rotate(getModelRotate().y(), 0, 1, 0);
    matrixModelHuman.rotate(getModelRotate().z(), 0, 0, 1);
    matrixModelHuman.rotate(getModelRotate().x(), 1, 0, 0);


    for(int i=0; i<getListEntity().count(); ++i)
    {

        QMatrix4x4 mBaseBelly;
        mBaseBelly.setToIdentity();
        mBaseBelly.translate(getListEntity().at(0)->getPosition());
        mBaseBelly.rotate(getListEntity().at(0)->getRotation().y(), 0,1,0);
        mBaseBelly.rotate(getListEntity().at(0)->getRotation().z(), 0,0,1);
        mBaseBelly.rotate(getListEntity().at(0)->getRotation().x(), 1,0,0);

        QMatrix4x4 m;
        m.setToIdentity();

        if(i<8)
        {

            QMatrix4x4 mBaseChest;
            mBaseChest.setToIdentity();
            mBaseChest.translate(getListEntity().at(1)->getPosition());
            mBaseChest.rotate(getListEntity().at(1)->getRotation().y(), 0,1,0);
            mBaseChest.rotate(getListEntity().at(1)->getRotation().z(), 0,0,1);
            mBaseChest.rotate(getListEntity().at(1)->getRotation().x(), 1,0,0);

            if(i == 0)
            {
                m = matrixModelHuman *mBaseBelly;
            }
            else if(i == 1)
            {
                m = matrixModelHuman *mBaseBelly*mBaseChest;
            }
            else if(i == 3) // Head
            {
                QMatrix4x4 matrixParent;
                matrixParent.setToIdentity();
                matrixParent.translate(getListEntity().at(2)->getPosition());
                matrixParent.rotate(getListEntity().at(2)->getRotation().y(),0,1,0);
                matrixParent.rotate(getListEntity().at(2)->getRotation().z(),0,0,1);
                matrixParent.rotate(getListEntity().at(2)->getRotation().x(),1,0,0);


                QMatrix4x4 mtrixChild;
                mtrixChild.setToIdentity();
                mtrixChild.translate(getListEntity().at(3)->getPosition());
                mtrixChild.rotate(getListEntity().at(3)->getRotation().y(), 0,1,0);
                mtrixChild.rotate(getListEntity().at(3)->getRotation().z(), 0,0,1);
                mtrixChild.rotate(getListEntity().at(3)->getRotation().x(), 1,0,0);

                m = matrixModelHuman *mBaseBelly*mBaseChest* matrixParent * mtrixChild;
            }
            else if(i == 5) // Hand RB
            {
                QMatrix4x4 matrixParent;
                matrixParent.setToIdentity();
                matrixParent.translate(getListEntity().at(4)->getPosition());
                matrixParent.rotate(getListEntity().at(4)->getRotation().y(),0,1,0);
                matrixParent.rotate(getListEntity().at(4)->getRotation().z(),0,0,1);
                matrixParent.rotate(getListEntity().at(4)->getRotation().x(),1,0,0);

                QMatrix4x4 mtrixChild;
                mtrixChild.setToIdentity();
                mtrixChild.translate(getListEntity().at(5)->getPosition());
                mtrixChild.rotate(getListEntity().at(5)->getRotation().y(), 0,1,0);
                mtrixChild.rotate(getListEntity().at(5)->getRotation().z(), 0,0,1);
                mtrixChild.rotate(getListEntity().at(5)->getRotation().x(), 1,0,0);

                m = matrixModelHuman *mBaseBelly*mBaseChest* matrixParent * mtrixChild;
            }
            else if (i == 7) // Hand LB
            {
                QMatrix4x4 matrixParent;
                matrixParent.setToIdentity();
                matrixParent.translate(getListEntity().at(6)->getPosition());
                matrixParent.rotate(getListEntity().at(6)->getRotation().y(),0,1,0);
                matrixParent.rotate(getListEntity().at(6)->getRotation().z(),0,0,1);
                matrixParent.rotate(getListEntity().at(6)->getRotation().x(),1,0,0);

                QMatrix4x4 mtrixChild;
                mtrixChild.setToIdentity();
                mtrixChild.translate(getListEntity().at(7)->getPosition());
                mtrixChild.rotate(getListEntity().at(7)->getRotation().y(), 0,1,0);
                mtrixChild.rotate(getListEntity().at(7)->getRotation().z(), 0,0,1);
                mtrixChild.rotate(getListEntity().at(7)->getRotation().x(), 1,0,0);

                m = matrixModelHuman *mBaseBelly*mBaseChest* matrixParent * mtrixChild;
            } else { // HandTop[ R & L ] & Neck

                QMatrix4x4 matrixEntitylHuman;
                matrixEntitylHuman.setToIdentity();
                matrixEntitylHuman.translate(getListEntity().at(i)->getPosition());
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().y() , 0, 1, 0);
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().z() , 0, 0, 1);
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().x() , 1, 0, 0);

                m = matrixModelHuman *mBaseBelly*mBaseChest* matrixEntitylHuman;
            }

        } else {

            if(i == 9) // Foot RB
            {

                QMatrix4x4 matrixParent;
                matrixParent.setToIdentity();
                matrixParent.translate(getListEntity().at(8)->getPosition());
                matrixParent.rotate(getListEntity().at(8)->getRotation().y(),0,1,0);
                matrixParent.rotate(getListEntity().at(8)->getRotation().z(),0,0,1);
                matrixParent.rotate(getListEntity().at(8)->getRotation().x(),1,0,0);

                QMatrix4x4 mtrixChild;
                mtrixChild.setToIdentity();
                mtrixChild.translate(getListEntity().at(9)->getPosition());
                mtrixChild.rotate(getListEntity().at(9)->getRotation().y(), 0,1,0);
                mtrixChild.rotate(getListEntity().at(9)->getRotation().z(), 0,0,1);
                mtrixChild.rotate(getListEntity().at(9)->getRotation().x(), 1,0,0);

                m = matrixModelHuman *mBaseBelly* matrixParent * mtrixChild;
            }
            else if (i == 11) // Foot LB
            {

                QMatrix4x4 matrixParent;
                matrixParent.setToIdentity();
                matrixParent.translate(getListEntity().at(10)->getPosition());
                matrixParent.rotate(getListEntity().at(10)->getRotation().y(),0,1,0);
                matrixParent.rotate(getListEntity().at(10)->getRotation().z(),0,0,1);
                matrixParent.rotate(getListEntity().at(10)->getRotation().x(),1,0,0);

                QMatrix4x4 mtrixChild;
                mtrixChild.setToIdentity();
                mtrixChild.translate(getListEntity().at(11)->getPosition());
                mtrixChild.rotate(getListEntity().at(11)->getRotation().y(), 0,1,0);
                mtrixChild.rotate(getListEntity().at(11)->getRotation().z(), 0,0,1);
                mtrixChild.rotate(getListEntity().at(11)->getRotation().x(), 1,0,0);

                m = matrixModelHuman *mBaseBelly* matrixParent * mtrixChild;
            } else {

                QMatrix4x4 matrixEntitylHuman;
                matrixEntitylHuman.setToIdentity();
                matrixEntitylHuman.translate(getListEntity().at(i)->getPosition());
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().y() , 0, 1, 0);
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().z() , 0, 0, 1);
                matrixEntitylHuman.rotate(getListEntity().at(i)->getRotation().x() , 1, 0, 0);

                m = matrixModelHuman *mBaseBelly* matrixEntitylHuman;
            }

        }

        /// ================= /// ================= ///

        m_shader->bind();
        m_shader->setUniformValue("mvMatrix", (m)); /// LIGHTING
        m_shader->setUniformValue("pMatrix", matrixPro*matrixCam);
        m_shader->setUniformValue("u_lightPosition", QVector4D(0.0, 1.0, 1.0, 1.0));
        m_shader->setUniformValue("u_lightPower", 1.0f);
        m_shader->setUniformValue("fragFlag", 1.0f);
        m_shader->setUniformValue("texturTransparency", 1.0f);

        m_vao->getListInfo()->at(i)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(i)->numVertex); // create List
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(i)->vao->release();

        m_shader->release();
    }

}

void ModelRobot::moveHuman()
{
    QElapsedTimer el;
    el.start();

    if(m_stateLink)
    {
        qInfo()<<"Ela Move: "<<el.nsecsElapsed();
        return;
    }


    if(!isFoucsGun)
    {
        float rotHumanY = getModelRotate().y() - m_AxisRightXEdit;

        if(rotHumanY >= 360.0f)
        {
            rotHumanY = fmod(rotHumanY,360.0f);
        }
        else if(rotHumanY < 0.0f)
        {
            rotHumanY = 360.0f + rotHumanY;
        }

        float newX = sin(qDegreesToRadians(rotHumanY));
        float newZ = cos(qDegreesToRadians(rotHumanY));

        setModelPosition(QVector3D(getModelPosition().x()+(newX*speedHuman),
                                   getModelPosition().y(),
                                   getModelPosition().z()+(newZ*speedHuman) ));
        setModelRotate(QVector3D(getModelRotate().x(), rotHumanY, getModelRotate().z()));
        setDirection(QVector3D(newX, getDirection().y(), newZ));
//        qInfo()<<"Ang No: "<<rotHumanY;
    }
    else
    {
//        float rotHumanY = getModelRotate().y();

//        float newX = sin(qDegreesToRadians(0.0f));
//        float newZ = cos(qDegreesToRadians(0.0f));

//        QVector3D posBase = QVector3D(newX*speedHuman, 0, newZ*speedHuman);

//        float ll = sqrt(pow(-m_AxisRightX,2) + pow(-m_AxisRightY,2));


        float rotHumanY = getModelRotate().y() + m_AxisRightXEdit;
        if(rotHumanY >= 360.0f)
        {
            rotHumanY = fmod(rotHumanY,360.0f);
        }
        else if(rotHumanY < 0.0f)
        {
            rotHumanY = 360.0f + rotHumanY;
        }

        float newX = sin(qDegreesToRadians(rotHumanY));
        float newZ = cos(qDegreesToRadians(rotHumanY));

        // Setting
        float angelAxis = qRadiansToDegrees(atan2(-m_AxisRightX, -m_AxisRightY));
        if(angelAxis < 0.0f) { angelAxis = 360.0f + angelAxis; }
        QVector3D rotAxis = QVector3D(0.0f, angelAxis, 0.0f);
        QVector3D posBefore = QVector3D(newX, 0.0f, newZ);
        QVector3D posAfter = calcRotPosition2(posBefore, rotAxis);

        setModelPosition(QVector3D(getModelPosition().x()+(posAfter.x()*-fabs(speedHuman)),
                                   getModelPosition().y(),
                                   getModelPosition().z()+(posAfter.z()*-fabs(speedHuman)) ));
        setDirection(QVector3D(posAfter.x(), getDirection().y(), posAfter.z()));

    }

    if(/*stateCollision*/false)
    {
//        qDebug("Checking Cools");
        m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
                                getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
    }

//    emit emtUpdate();
//    qInfo()<<"Ela Move: "<<el.nsecsElapsed();
}

void ModelRobot::setOnTransport(bool check)
{
    m_stateOn = check;
}
bool ModelRobot::checkOn()
{
    return m_stateOn;
}
void ModelRobot::setLink(bool stateLink)
{
    m_stateLink = stateLink;
}
bool ModelRobot::checkLink()
{
    return m_stateLink;
}
void ModelRobot::setControl(ControllerCollision *controlCol)
{
    m_controlCol=controlCol;
}
void ModelRobot::sendOnErth(bool on)
{
    if(!m_OnErth && on)
    {
        animitionStandUp(); // Not Save
    }
//    if(!on)
//    {
//        if(speedHuman != 0.00f)
//        {
//            if(speedHuman>0)
//            {
//                speedHuman-=0.01;
//            }
//            else if(speedHuman<0)
//            {
//                speedHuman+=0.01;
//            }
//        }
//    }
    m_OnErth = on;
}

void ModelRobot::setAxisRightX(double axis)
{

    if(checkLink())
    {
        setAxisRotLink(axis);
    }
    else
    {
        m_AxisRightX = axis;

        if(axis > 0.4f || axis < -0.4f)
        {
            m_AxisRightXEdit = axis*2.5f;
        }
        else if(axis > 0.1f || axis < -0.1f)
        {
            m_AxisRightXEdit = axis*1.5f;
        }
        else
        {
            m_AxisRightXEdit = 0.0f;
        }

        axisAll();
    }

}
void ModelRobot::setAxisRightY(double axis)
{
    m_AxisRightY = axis;

    axisAll();

}
void ModelRobot::setAxisLeftX(double axis)
{
    //rotCam
    float m_axisEdit;
    if(axis > 0.4f || axis < -0.4f)
    {
        m_axisEdit = axis*2.5f;
    }
    else if(axis > 0.1f || axis < -0.1f)
    {
        m_axisEdit = axis*1.5f;
    }
    else
    {
        m_axisEdit = 0.0f;
    }

    float rotHumanY = getModelRotate().y() - m_axisEdit;

    if(rotHumanY >= 360.0f)
    {
        rotHumanY = fmod(rotHumanY,360.0f);
    }
    else if(rotHumanY < 0.0f)
    {
        rotHumanY = 360.0f + rotHumanY;
    }

    setModelRotate(QVector3D(getModelRotate().x(), rotHumanY, getModelRotate().z()));
}
void ModelRobot::setAxisLeftY(double axis)
{

}

void ModelRobot::axisAll()
{
    float lenF = sqrt( pow(m_AxisRightY, 2) + pow(m_AxisRightX, 2)) ;

    if(m_AxisRightY<0.0f) // Problem 1
    {
        if(lenF>0.2f && lenF<=0.6f)
        {
            stateCollision = true;
            speedHuman = -0.1f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }

            if(!timerAnimitionMove->isActive())
            {
                timerAnimitionMove->start(SPEED_T_ANIMI2);
            }
            else
            {
                if(timerAnimitionMove->interval() != SPEED_T_ANIMI2)
                {
                    timerAnimitionMove->setInterval(SPEED_T_ANIMI2);
                }
            }
        }
        else if(lenF>0.6f)
        {
            stateCollision = true;
            speedHuman = -0.2f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }

            if(!timerAnimitionMove->isActive())
            {
                timerAnimitionMove->start(SPEED_T_ANIMI1);
            }
            else
            {
                if(timerAnimitionMove->interval() != SPEED_T_ANIMI1)
                {
                    timerAnimitionMove->setInterval(SPEED_T_ANIMI1);
                }
            }
        }
    }
    else if(m_AxisRightY>0.0f)
    {
        if(lenF>0.2f)
        {
            stateCollision = true;
            speedHuman = 0.1f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }

            if(!timerAnimitionMove->isActive())
            {
                timerAnimitionMove->start(SPEED_T_ANIMI2);
            }
            else
            {
                if(timerAnimitionMove->interval() != SPEED_T_ANIMI2)
                {
                    timerAnimitionMove->setInterval(SPEED_T_ANIMI2);
                }
            }
        }
    }

}

void ModelRobot::sendCollisionHuman(QVector3D posA, QVector3D rotA)
{
//    qDebug("Coll");
    if(stateCollision)
    {
        stateCollision = false;
        emit emtStopTimerMove();
        setModelPosition(posA);
//        setModelRotate(rotA);

        speedHuman = -speedHuman;
        moveHuman();
        speedHuman = 0.0f;
    }
}
void ModelRobot::slotStopTimerMove()
{
    timerHumanGo->stop();
//    timerHumanJump->stop();
//    m_controlCol->setStateGrv(true);
//    m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
//                                getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
}

void ModelRobot::sendStopJump()
{
    emit emtStopJump();
    animitionStandUp();
}
void ModelRobot::slotStopJump()
{
    if(timerHumanJump->isActive())
    {
        timerHumanJump->stop();
    }
}

void ModelRobot::sendDropHuman(QVector3D posCar, QVector3D direcCar)
{
//    setModelRotate(QVector3D(90,0,0));
    dropDirectCar=direcCar;
    dropPosCar=posCar;
    if(!timerAnimitionDrop->isActive())
    {
        emit emtStartTimerDrop();
    }

}
void ModelRobot::slotStartTimerDrop()
{
    timerAnimitionDrop->start(20);
}
void ModelRobot::slotTimerDrop()
{
    //    setModelRotate(QVector3D(90,0,0));

    ////////////

    setModelPosition(QVector3D(getModelPosition().x()+dropDirectCar.x(),
                               getModelPosition().y(),
                               getModelPosition().z()+dropDirectCar.z()));
    if(dropPosCar.x()>getModelPosition().x()+2 || dropPosCar.x()<getModelPosition().x()-2)
    {
        timerAnimitionDrop->stop();
    }
//    if(getModelRotate().z() < 90.0f || getModelRotate().z() == 0.0f)
//    {
//        setModelRotate(QVector3D(getModelRotate().x(),getModelRotate().y()+1.0f,getModelRotate().z()+1.0f));
//    } else {
//        timerHumanDrop->stop();
//    }
//    qDebug()<<"RotZ:"<<getModelRotate();

}

void ModelRobot::jumpHuman()
{
    if(m_OnErth)
    {
        oldJump_Y = getModelPosition().y();
//        setModelPosition(QVector3D(getModelPosition().x(),
//                                   getModelPosition().y()+0.05f,
//                                   getModelPosition().z()));
        m_controlCol->setStateGrv(false);
        animitionJump();
        timerHumanJump->start(20);
        m_OnErth = false;
    }
}
void ModelRobot::slotTimerJump()
{
//    timerAnimitionMove->stop();

    float newJump_Y = getModelPosition().y();
    if(newJump_Y < (oldJump_Y+1.5f))
    {
        setModelPosition(QVector3D(getModelPosition().x(),
                                   getModelPosition().y()+0.05f,
                                   getModelPosition().z()));
        qDebug()<<"+++ "<<getModelPosition().y();
    }
    else
    {
        timerHumanJump->stop();
        m_controlCol->setStateGrv(true);
//        m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
//                                    getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
    }
    emit emtUpdate();
}

void ModelRobot::setState_StandSid()
{
    if(stateHuman() == "SitDown")
    {
        animitionStandUp();
    }
    else if (stateHuman() == "StandUp")
    {
        animitionSitDown();
    }
    emit emtUpdate();
}

void ModelRobot::animitionMove()
{
    //    QElapsedTimer el;
    //    el.start();

    float lenF = sqrt( pow(m_AxisRightY, 2) + pow(m_AxisRightX, 2)) ;
//    qDebug()<<"Ani: "<<lenF<<" | "<<m_AxisRightX<<" | "<<m_AxisRightY ;

    //    if((m_AxisRightY > -0.3 && m_AxisRightY < 0.3) && (m_AxisRightX > -0.3 && m_AxisRightX < 0.3))
    if(lenF <= 0.2f)
    {
//        if(counterRotGo == 0)
        {
            timerHumanGo->stop();
            timerAnimitionMove->stop();

            entityHumanChest->setRotation(QVector3D(0.0f, 0.0f, 0.0f));
            entityHumanFootLB->setRotation(QVector3D(0.0f, 0.0f, 0.0f));
            entityHumanFootRB->setRotation(QVector3D(0.0f, 0.0f, 0.0f));

            emit emtUpdate();
            return;
        }
//        else
        {
            speedHuman -= 0.02; // Slow Speed With Back valRotate to 0
        }
    }


    if(!m_OnErth || m_stateLink)
    {
        if(counterRotGo > 0)
        {
            counterRotGo-=4;
        }
        else if( counterRotGo < 0)
        {
            counterRotGo+=4;
        }
        return;
    }

    entityHumanChest->setRotation(QVector3D(-15.0f,0,0));

    /// =============== ///

    //    float addX = 0.0f;
    float hightBelly = fabs(entityHumanFootRT->getLimited()->limitedBottom.y())+
            fabs(entityHumanFootRB->getLimited()->limitedBottom.y());

    if(counterRotGo >= 20)
    { addX = -4.0; }
    if(counterRotGo <= -20)
    { addX = 4.0; }

    if(addX < 0)
    {
        counterRotGo+=addX;
        float valCount = 0.0f;
        if(speedHuman == -0.1f || speedHuman == 0.1f)
        {
            valCount = counterRotGo / 1.5f;
        } else if(speedHuman == -0.2f)
        {
            valCount = counterRotGo;
        }

        if(!isFoucsGun)
        {
            entityHumanBelly->setPosition(QVector3D(entityHumanBelly->getPosition().x(),
                                                    hightBelly + fabs(valCount) * 0.005f,
                                                    entityHumanBelly->getPosition().z()));

            entityHumanChest->setRotation(QVector3D(entityHumanChest->getRotation().x(),
                                                    -valCount*0.7f,
                                                    entityHumanChest->getRotation().z()));

            entityHumanHandRT->setRotation(QVector3D(-valCount*1.5f, 0.0f, 8.0f));
            entityHumanHandRB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));

            entityHumanHandLT->setRotation(QVector3D(valCount*1.5f, 0.0f, -30.0f));
            entityHumanHandLB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
        }
        /// ============== FOOT ================ ///

        entityHumanFootRT->setRotation(QVector3D(valCount*4.0f, 0.0f, 0.0f));

        entityHumanFootRB->setRotation(QVector3D(-fabs(entityHumanFootRT->getRotation().x())*1.5, 0.0f, 0.0f));

        entityHumanFootLT->setRotation(QVector3D(-valCount*4.0f, 0.0f, 0.0f));

//        entityHumanFootLB->setRotation(QVector3D(-fabs(entityHumanFootLT->getRotation().x()), 0.0f, 0.0f));
    }
    if(addX > 0)
    {
        counterRotGo+=addX;
        float valCount = 0.0f;
        if(speedHuman == -0.1f || speedHuman == 0.1f)
        {
            valCount = counterRotGo / 1.5f;
        } else if(speedHuman == -0.2f)
        {
            valCount = counterRotGo;
        }

        if(!isFoucsGun)
        {
            entityHumanBelly->setPosition(QVector3D(entityHumanBelly->getPosition().x(),
                                                    hightBelly + fabs(valCount) * 0.005f,
                                                    entityHumanBelly->getPosition().z()));

            entityHumanChest->setRotation(QVector3D(entityHumanChest->getRotation().x(),
                                                    -valCount*0.7f,
                                                    entityHumanChest->getRotation().z()));

            entityHumanHandRT->setRotation(QVector3D(-valCount*1.5f, 0.0f, 8.0f));
            entityHumanHandRB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));

            entityHumanHandLT->setRotation(QVector3D(valCount*1.5f, 0.0f, -30.0f));
            entityHumanHandLB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
        }
        /// ============== FOOT ================ ///

        entityHumanFootRT->setRotation(QVector3D(valCount*4.0f, 0.0f, 0.0f));

//        entityHumanFootRB->setRotation(QVector3D(-fabs(entityHumanFootRT->getRotation().x()), 0.0f, 0.0f));

        entityHumanFootLT->setRotation(QVector3D(-valCount*4.0f, 0.0f, 0.0f));

        entityHumanFootLB->setRotation(QVector3D(-fabs(entityHumanFootLT->getRotation().x())*1.5, 0.0f, 0.0f));
    }
    //    qDebug()<<"ela nimi: "<<el.nsecsElapsed();

    emit emtUpdate();

}
void ModelRobot::animitionJump()
{

    float hieghtBelly = fabs(entityHumanFootRT->getLimited()->limitedBottom.y())+
                       fabs(entityHumanFootRB->getLimited()->limitedBottom.y());

    entityHumanBelly->setPosition(QVector3D(entityHumanBelly->getPosition().x(),
                                            hieghtBelly,
                                            entityHumanBelly->getPosition().z()));

    entityHumanChest->setRotation(QVector3D(320.0f, 0.0f, 0.0f));

    entityHumanHandRT->setRotation(QVector3D(15.0f, 0.0f, 45.0f));
    entityHumanHandLT->setRotation(QVector3D(45.0f, 0.0f, 315.0f));

    entityHumanHandRB->setRotation(QVector3D(75.0f, 0.0f, 0.0f));
    entityHumanHandLB->setRotation(QVector3D(45.0f, 0.0f, 0.0f));

    entityHumanFootRT->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
    entityHumanFootLT->setRotation(QVector3D(60.0f, 0.0f, 0.0f));

    entityHumanFootRB->setRotation(QVector3D(270.0f, 0.0f, 0.0f));
    entityHumanFootLB->setRotation(QVector3D(300.0f, 0.0f, 0.0f));
}
void ModelRobot::animitionGunZoom(bool boolGun)
{
    isFoucsGun=boolGun;

    if(boolGun)
    {
        entityHumanHead->setRotation(QVector3D(0.0f, 0.0f, -20.0f));
        entityHumanHandRT->setRotation(QVector3D(20.0f, 0.0f, 0.0f));
        entityHumanHandRB->setRotation(QVector3D(120.0f, 0.0f, 0.0f));

        entityHumanHandLT->setRotation(QVector3D(50.0f, -45.0f, -45.0f));
        entityHumanHandLB->setRotation(QVector3D(60.0f, 0.0f, 0.0f));

    } else {

        entityHumanHead->setRotation(QVector3D(0.0f, 0.0f, 0.0f));
        entityHumanHandRT->setRotation(QVector3D(0.0f, 0.0f, 8.0f));
        entityHumanHandRB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));

        entityHumanHandLT->setRotation(QVector3D(0.0f, 0.0f, -30.0f));
        entityHumanHandLB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
    }
    emit emtUpdate();
}
void ModelRobot::animitionSitDown()
{
    QVector3D rotsTop(70,0,0);
    QVector3D rotsBottom(-140,0,0);

    entityHumanChest->setRotation(QVector3D(-15.0f, 0.0f, 0.0f));
    entityHumanFootRT->setRotation(rotsTop);
    entityHumanFootLT->setRotation(rotsTop);
    entityHumanFootRB->setRotation(rotsBottom);
    entityHumanFootLB->setRotation(rotsBottom);

    QVector3D vecFootT = calcRotLimit2(entityHumanFootRT->getLimited(),entityHumanFootRT->getRotation()).limitedBottom;
    QVector3D vecFootB = calcRotLimit2(entityHumanFootRB->getLimited(),entityHumanFootRB->getRotation()).limitedBottom;

    entityHumanBelly->setPosition(QVector3D(0.0f, fabs(vecFootT.y())+fabs(vecFootB.y()), 0.0f));

    m_stateHuman="SitDown";
    setModelLimited();
}
void ModelRobot::animitionStandUp()
{
    QVector3D rotsTop(0,0,0);
    QVector3D rotsBottom(0,0,0);

    entityHumanChest->setRotation(QVector3D(0.0f, 0.0f, 0.0f));
    entityHumanFootRT->setRotation(rotsTop);
    entityHumanFootLT->setRotation(rotsTop);
    entityHumanFootRB->setRotation(rotsBottom);
    entityHumanFootLB->setRotation(rotsBottom);

    entityHumanHandRT->setRotation(QVector3D(0.0f, 0.0f, 8.0f));
    entityHumanHandRB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));
    entityHumanHandLT->setRotation(QVector3D(0.0f, 0.0f, -30.0f));
    entityHumanHandLB->setRotation(QVector3D(90.0f, 0.0f, 0.0f));

    QVector3D vecFootT = calcRotLimit2(entityHumanFootRT->getLimited(),entityHumanFootRT->getRotation()).limitedBottom;
    QVector3D vecFootB = calcRotLimit2(entityHumanFootRB->getLimited(),entityHumanFootRB->getRotation()).limitedBottom;

    entityHumanBelly->setPosition(QVector3D(0.0f, fabs(vecFootT.y())+fabs(vecFootB.y()), 0.0f));

    m_stateHuman="StandUp";
    setModelLimited();
}

QString ModelRobot::stateHuman()
{
    return m_stateHuman;
}


void ModelRobot::setAxisRotLink(double axis)
{
        if(axis > 0.4f || axis < -0.4f)
        {
            m_AxisRotLink = -axis*2.5f;

            if(!timerRotLink->isActive())
            { timerRotLink->start(SPEED_T_HUMAN); }
        }
        else if(axis > 0.1f || axis < -0.1f)
        {
            m_AxisRotLink = -axis*1.5f;

            if(!timerRotLink->isActive())
            { timerRotLink->start(SPEED_T_HUMAN); }
        }
        else
        {
            m_AxisRotLink = 0.0f;

            if(timerRotLink->isActive())
            { timerRotLink->stop(); }
        }
}
void ModelRobot::slotRotLink()
{
    setModelRotate(getModelRotate()+QVector3D(0,m_AxisRotLink,0));
}



QMatrix4x4 ModelRobot::getMatrixGun()
{

    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(getModelPosition());
    matrixModel.rotate(getModelRotate().y(), 0, 1, 0);
//    matrixModel.rotate(getModelRotate().z(), 0, 0, 1);
//    matrixModel.rotate(getModelRotate().x(), 1, 0, 0);

    QMatrix4x4 matrixBelly;
    matrixBelly.translate(entityHumanBelly->getPosition());
    matrixBelly.rotate(entityHumanBelly->getRotation().y(), 0,1,0);
//    matrixBelly.rotate(entityHumanBelly->getRotation().z(), 0,0,1);
//    matrixBelly.rotate(entityHumanBelly->getRotation().x(), 1,0,0);

    QMatrix4x4 matrixChest;
    matrixChest.translate(entityHumanChest->getPosition());
    matrixChest.rotate(entityHumanChest->getRotation().y(), 0,1,0);
//    matrixChest.rotate(entityHumanChest->getRotation().z(), 0,0,1);
//    matrixChest.rotate(entityHumanChest->getRotation().x(), 1,0,0);

    QMatrix4x4 matrixHandT;
    matrixHandT.translate(entityHumanHandRT->getPosition());
    matrixHandT.rotate(entityHumanHandRT->getRotation().y(), 0,1,0);
//    matrixHandT.rotate(entityHumanHandRT->getRotation().z(), 0,0,1);
//    matrixHandT.rotate(entityHumanHandRT->getRotation().x(), 1,0,0);

    QMatrix4x4 matrixHandB;
    matrixHandB.translate(entityHumanHandRB->getPosition());
    matrixHandB.rotate(entityHumanHandRB->getRotation().y(), 0,1,0);
//    matrixHandB.rotate(entityHumanHandRB->getRotation().z(), 0,0,1);
//    matrixHandB.rotate(entityHumanHandRB->getRotation().x(), 1,0,0);

    QMatrix4x4 matrixEndHand;
    matrixEndHand.translate( QVector3D(0, entityHumanHandRB->getLimited()->limitedBottom.y(), 0));
    matrixEndHand.rotate( entityHumanHandRT->getRotation().x()+entityHumanHandRB->getRotation().x() ,1,0,0);

    return matrixModel*matrixBelly*matrixChest*matrixHandT*matrixHandB /*matrixEndHand */;

}
QVector3D ModelRobot::getPosGun()
{
    // نجمع مواقع سلسلة الكائنات
    // ثم نجمع دوران سلسلة الكائنات على محور واي
    //
    //
    QVector3D vecGuns = entityHumanBelly->getPosition() +
                        entityHumanChest->getPosition() +
                        entityHumanHandRT->getPosition() +
                        entityHumanHandRB->getPosition();
    return vecGuns;
}
QVector3D ModelRobot::getPosBullet()
{

    QVector3D posChest = getPosFromEntity(entityHumanBelly->getPosition(),
                                          entityHumanChest->getPosition(),
                                          entityHumanChest->getRotation());

    QVector3D posHandRT = getPosFromEntity(posChest,
                                          entityHumanHandRT->getPosition(),
                                          entityHumanHandRT->getRotation());

    QVector3D posGlobal = getPosFromEntity(posHandRT,
                                      getModelPosition(),
                                      getModelRotate());

    return posGlobal;
}
QVector3D ModelRobot::getPosThrower()
{
    QVector3D posBelly = entityHumanBelly->getPosition(); //getPosFromEntity2(getModelPosition(), getModelRotate(), entityHumanBelly->getPosition());

    QVector3D posChest = getPosFromEntity2(posBelly, entityHumanBelly->getRotation(), entityHumanChest->getPosition());
//    qDebug()<<"CC: "<<entityHumanChest->getRotation();
    QVector3D posHandRT = getPosFromEntity2(posChest, entityHumanChest->getRotation(), entityHumanHandRT->getPosition());

    QVector3D posHandRB = getPosFromEntity2(posHandRT, entityHumanHandRT->getRotation(), entityHumanHandRB->getPosition());


    QVector3D endHand = QVector3D(0.0f,entityHumanHandRB->getLimited()->limitedBottom.y()/*-(0.06f)*/,0.0f);
    QVector3D posEndHandRB = getPosFromEntity2(posHandRB,
                                               entityHumanHandRB->getRotation()+entityHumanHandRT->getRotation(),
                                               endHand);

    QVector3D posThrowers = getPosFromEntity2( getModelPosition(), getModelRotate(), posEndHandRB);

    return posThrowers;

}

QVector3D ModelRobot::getPosCamZoom()
{

    QVector3D posBelly = entityHumanBelly->getPosition(); //getPosFromEntity2(getModelPosition(), getModelRotate(), entityHumanBelly->getPosition());

    QVector3D posChest = getPosFromEntity2(posBelly, entityHumanBelly->getRotation(), entityHumanChest->getPosition());

    QVector3D posHandRT = getPosFromEntity2(posChest, entityHumanChest->getRotation(), entityHumanHandRT->getPosition());

    QVector3D posHandRB = getPosFromEntity2(posHandRT, entityHumanHandRT->getRotation(), entityHumanHandRB->getPosition());


    QVector3D endHand = QVector3D(0.0f,entityHumanHandRB->getLimited()->limitedBottom.y(),0.0f);
    QVector3D posEndHandRB = getPosFromEntity2(posHandRB,
                                               entityHumanHandRB->getRotation()+entityHumanHandRT->getRotation(),
                                               endHand);

    return posEndHandRB;
}

QVector3D ModelRobot::getPosFromEntity(QVector3D posEntity, QVector3D posParent, QVector3D rotParent)
{
    float lenthVec = sqrt( pow(posEntity.x(),2) + pow(posEntity.z(),2) );
    float angelVec = qRadiansToDegrees( atan2(posEntity.x(), posEntity.z()) ) ;
    float doubleRotVec = angelVec+rotParent.y();

    QVector3D posEntityNew = QVector3D(lenthVec* sin(qDegreesToRadians(doubleRotVec)),
                                       posEntity.y(),
                                       lenthVec* cos(qDegreesToRadians(doubleRotVec)));

    QVector3D returnPos = posParent + posEntityNew;
    return returnPos;

}
QVector3D ModelRobot::getPosFromEntity2(QVector3D posParent, QVector3D rotParent, QVector3D posChild)
{
     QVector3D newPosChilds = calcRotPosition2(posChild, rotParent);
     return posParent + newPosChilds;
}
QVector3D ModelRobot::calcRotPosition2(QVector3D pos, QVector3D rots)
{

    /// X
    float lenVec1 = sqrt( pow(pos.y(),2) + pow(pos.z(),2) );
    float angelVec1 = qRadiansToDegrees( atan2(pos.z(), pos.y()) ) ;
    float rotVec1 = angelVec1+rots.x();
    QVector3D newPos1 = QVector3D(pos.x(),
                                       lenVec1* cos(qDegreesToRadians(rotVec1)),
                                       lenVec1* sin(qDegreesToRadians(rotVec1)));

    /// Z
    float lenVec3 = sqrt( pow(newPos1.x(),2) + pow(newPos1.y(),2) );
    float angelVec3 = qRadiansToDegrees( atan2(newPos1.y(), newPos1.x()) ) ; // True y->x :: Erro x->y
    float rotVec3 = angelVec3+rots.z();
    QVector3D newPos3 = QVector3D(lenVec3* cos(qDegreesToRadians(rotVec3)),
                                        lenVec3* sin(qDegreesToRadians(rotVec3)),
                                        newPos1.z());

    /// y
    float lenVec2 = sqrt( pow(newPos3.x(),2) + pow(newPos3.z(),2) );
    float angelVec2 = qRadiansToDegrees( atan2(newPos3.x(), newPos3.z()) ) ;
    float rotVec2 = angelVec2+rots.y();
    QVector3D newPos2 = QVector3D(lenVec2* sin(qDegreesToRadians(rotVec2)),
                                        newPos3.y(),
                                        lenVec2* cos(qDegreesToRadians(rotVec2)));

    return newPos2;
}

void ModelRobot::setEnableZoom(bool state)
{
    m_stateZoom = state;
}
QCubeLimited ModelRobot::calcRotLimit2(QCubeLimited *limit, QVector3D rots)
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

//    qDebug()<<"LIMITFull: "<<limited.limitedTop<<" ~ \n"<<limited.limitedBottom;
    return limited;
}

///=============== Wedge =================///




