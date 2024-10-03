#include "modelcar.h"
#include "entity.h"
#include "math.h"
#include "qmath.h"
#include <QTimer>
#include <QDebug>
#include "vaoentity.h"
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <controllercollision.h>


ModelCar::ModelCar(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) :
    QObject(parent),
    m_shader(shader),
    m_vao(vao),
    listEntityInfo(vao->getListInfo())
{
    quatAll = QQuaternion(1,0,0,0);
    timerCar = new QTimer;
    connect(timerCar, &QTimer::timeout, this, &ModelCar::moveCar);
    connect(this, &ModelCar::emtStopTimer, this, &ModelCar::slotStopTimer);
}

ModelCar::~ModelCar()
{
    for(int l=0; l<listEntity.count(); ++l)
    {
        Entity *e = listEntity.takeAt(l);
        delete e;
        e = nullptr;
    }

    /// Un Link Another Pointer
    m_shader = nullptr;
    delete m_vao;//m_vao = nullptr;
    listEntityInfo = nullptr;
    // Control
    m_controlCol = nullptr;

    timerCar->deleteLater();
}

void ModelCar::createEntity()
{
    entityCarBudy = new Entity;
    entityCarBudy->setPosition(QVector3D(0,-0.19,0));
    entityCarBudy->setInfo(listEntityInfo->at(0), true);
    listEntity.append(entityCarBudy);

    entityCarWheel1 = new Entity; // left-amam
    entityCarWheel1->setPosition(QVector3D(-0.83,0,-2.45));
    entityCarWheel1->setInfo(listEntityInfo->at(1), true);
    //    entityCarWheel1->setRotation(QVector3D(0, 30, 0));
    listEntity.append(entityCarWheel1);

    entityCarWheel2 = new Entity; //left-back
    entityCarWheel2->setPosition(QVector3D(-0.83,0,0));
    entityCarWheel2->setInfo(listEntityInfo->at(2), true);
    listEntity.append(entityCarWheel2);

    entityCarWheel3 = new Entity; // right-amam
    entityCarWheel3->setPosition(QVector3D(0.83,0,-2.45));
    entityCarWheel3->setInfo(listEntityInfo->at(3), true);
    //    entityCarWheel3->setRotation(QVector3D(0, 30, 0));
    listEntity.append(entityCarWheel3);

    entityCarWheel4 = new Entity; //right-back
    entityCarWheel4->setPosition(QVector3D(0.83,0,0));
    entityCarWheel4->setInfo(listEntityInfo->at(4), true);
    listEntity.append(entityCarWheel4);

    // Test Check
    //    for(int x=0; x<listEntityInfo->at(0)->partsLimited->count(); ++x)
    {
        //        qDebug()<<"Test "<<x<<" :"<<listEntityInfo->at(0)->partsLimited->at(x)->limitedTop<<" # "<<listEntityInfo->at(0)->partsLimited->at(x);
    }
    //    qDebug()<<"storg: "<<listEntityInfo->at(0)->partsLimited;
}
QList<Entity *> ModelCar::getListEntity()
{
    return listEntity;
}

void ModelCar::setDirection(QVector3D dir)
{
    m_dir = dir;
}
QVector3D ModelCar::getDirection()
{
    return m_dir;
}

void ModelCar::draw(QMatrix4x4 matrixProCam)
{
    /// =========== World Car =========== ///

    QMatrix4x4 matrixModelCar;
    matrixModelCar.setToIdentity();
    matrixModelCar.translate(getModelPosition());
    matrixModelCar.rotate(quatAll);

    for(int i=0; i<getListEntity().count(); ++i)
    {
        QMatrix4x4 matrixEntitylCar;
        matrixEntitylCar.setToIdentity();
        matrixEntitylCar.translate(getListEntity().at(i)->getPosition());
        //        matrixEntitylCar.rotate(getListEntity().at(i)->getRotation().x(), 1, 0, 0);
        //        matrixEntitylCar.rotate(getListEntity().at(i)->getRotation().y(), 0, 1, 0);
        //        matrixEntitylCar.rotate(getListEntity().at(i)->getRotation().z(), 0, 0, 1);
        QQuaternion quatAlls;
        QQuaternion quatX = QQuaternion::fromAxisAndAngle(quatAlls.rotatedVector(QVector3D(1,0,0)), getListEntity().at(i)->getRotation().x());
        quatAlls = quatX*quatAlls;
        QQuaternion quatY = QQuaternion::fromAxisAndAngle(quatAlls.rotatedVector(QVector3D(0,1,0)), getListEntity().at(i)->getRotation().y());
        quatAlls = quatY*quatAlls;
        QQuaternion quatZ = QQuaternion::fromAxisAndAngle(quatAlls.rotatedVector(QVector3D(0,0,1)), getListEntity().at(i)->getRotation().z());
        quatAlls = quatZ*quatAlls;
        matrixEntitylCar.rotate(quatAlls);


        m_shader->bind();

        m_shader->setUniformValue("mvMatrix", (matrixModelCar*matrixEntitylCar));
        m_shader->setUniformValue("pMatrix", matrixProCam);
        m_shader->setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
        m_shader->setUniformValue("u_lightPower", 1.0f);
        m_shader->setUniformValue("fragFlag", 0.1f);
        m_shader->setUniformValue("texturTransparency", 1.0f);

        m_vao->getListInfo()->at(i)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(i)->numVertex); // create List
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(i)->vao->release();
        m_shader->release();
    }
}
void ModelCar::moveCar()
{
    /// Angel
    QQuaternion quatY = QQuaternion::fromAxisAndAngle(quatAll.rotatedVector(QVector3D(0,1,0)), float(-m_AxisRightX));
    quatAll = quatY * quatAll;
    float toAxisY = (quatAll.toEulerAngles().y()<0.0f) ? 360.0f+quatAll.toEulerAngles().y() : quatAll.toEulerAngles().y();
    //    qDebug()<<"Angel_Car: "<<toAxisY<<" > "<<quatAll.toEulerAngles().y();

    /// Move
    float newX = -sin(qDegreesToRadians(toAxisY));
    float newZ = -cos(qDegreesToRadians(toAxisY));

    setModelPosition(QVector3D(getModelPosition().x()+(speedCar*newX),
                               getModelPosition().y(),
                               getModelPosition().z()+(speedCar*newZ) )) ;
    setDirection(QVector3D(newX,0,newZ));

    if(checking)
    {
        //        m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
        //                                getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
    }

    emit emtUpdate();
}

void ModelCar::speedUp()
{

    //    QQuaternion quatX = QQuaternion::fromAxisAndAngle(quatAll.rotatedVector(QVector3D(0,1,0)), 2.0f);
    //    quatAll = quatX * quatAll /*quatY * quatZ*/;
    ///====================
    checking = true;

    if(speedCar<0.2f)
    {
        speedCar+=0.02f;
    }
    else
    {
        speedCar=0.2f;
    }

    if(!timerCar->isActive())
    {
        timerCar->start(20);
        emit emtStateCar(true);
    }

    if(speedCar<0.02f && speedCar>-0.02f)
    {
        speedCar=0.0f;
        if(timerCar->isActive())
        {
            timerCar->stop();
            emit emtStateCar(false);
        }
    }

}
void ModelCar::speedDown()
{
    //    QQuaternion quatX = QQuaternion::fromAxisAndAngle(quatAll.rotatedVector(QVector3D(1,0,0)), 2.0f);
    //    QQuaternion quatY = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), rot.y());
    //    QQuaternion quatZ = QQuaternion::fromAxisAndAngle(QVector3D(0,0,1), rot.z());
    //    quatAll = quatX * quatAll /*quatY * quatZ*/;

    ///===========================
    checking = true;

    if(speedCar>=-0.18f)
    {
        speedCar-=0.02f;
    } else {
        speedCar=-0.2f;
    }

    if(!timerCar->isActive())
    {
        timerCar->start(20);
        emit emtStateCar(true);
    }

    if(speedCar<0.02f && speedCar>-0.02f)
    {
        speedCar=0.0f;
        if(timerCar->isActive())
        {
            timerCar->stop();
            emit emtStateCar(false);
        }
    }
}
void ModelCar::setAxisRightX(float v)
{
    m_AxisRightX=v;
}

void ModelCar::setControl(ControllerCollision *controlCol)
{
    m_controlCol = controlCol;
}

void ModelCar::slotStopTimer()
{
    timerCar->stop();
    emit emtStateCar(false);
}
void ModelCar::sendCollisionCar(QVector3D posA, QVector3D rotA)
{
    if(checking)
    {
        checking = false;
        emit emtStopTimer();
        setModelPosition(posA);
        setModelRotate(rotA);

        speedCar = -speedCar;
        moveCar();
        speedCar = 0.0f;
    }

}


void ModelCar::convertEluerTOQuat(float roll, float pitch, float yaw)
{
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    //    Quaternion q;
    float w = cr * cp * cy + sr * sp * sy;
    float x = sr * cp * cy - cr * sp * sy;
    float y = cr * sp * cy + sr * cp * sy;
    float z = cr * cp * sy - sr * sp * cy;

    //    return q;
}
void ModelCar::convertQuatTOEuler(QQuaternion quat)
{
    QVector3D vecRPY;
    QVector4D vecQuat(quat.toVector4D());
    //    QVector4D()
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (vecQuat.w() * vecQuat.x() + vecQuat.y() * vecQuat.z());
    double cosr_cosp = 1 - 2 * (vecQuat.w() * vecQuat.x() + vecQuat.y() * vecQuat.z());
    //        vecRPY.setX(atan2(sinr_cosp, cosr_cosp));
    vecRPY = QVector3D(std::atan2(sinr_cosp, cosr_cosp), vecRPY.y(), vecRPY.z());

    // pitch (y-axis rotation)
    double sinp = 2 * (vecQuat.w() * vecQuat.x() + vecQuat.y() * vecQuat.z());
    if (std::abs(sinp) >= 1)
        vecRPY = QVector3D(vecRPY.x(), std::copysign(M_PI / 2, sinp), vecRPY.z());
    //            angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        vecRPY = QVector3D(vecRPY.x(), std::asin(sinp), vecRPY.z());
    //            angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (vecQuat.w() * vecQuat.x() + vecQuat.y() * vecQuat.z());
    double cosy_cosp = 1 - 2 * (vecQuat.w() * vecQuat.x() + vecQuat.y() * vecQuat.z());
    //        angles.yaw = std::atan2(siny_cosp, cosy_cosp);
    vecRPY = QVector3D(vecRPY.x(), vecRPY.y(), std::atan2(siny_cosp, cosy_cosp));

    //        return angles;
}









