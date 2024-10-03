#ifndef MODELPLANE_H
#define MODELPLANE_H

#include <QObject>
#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QMatrix4x4>
#include "basemodel.h"

class QOpenGLShaderProgram;
class Entity;
class VAOEntity;
class EntityInfo;
class QCubeLimited;
class QTimer;
class ControllerCollision;

class ModelPlane : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelPlane(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelPlane();
    void createEntity();
    QList<Entity *> getListEntity();

    void setDirection(QVector3D dir);
    QVector3D getDirection();

    void draw(QMatrix4x4 matrixProCam);
    void setStatePlane(bool state);
    void setAxisRightX(double axis);
    void setAxisLeft(float axis);
    void speedUP();
    void speedDown();
    void closeWheels();
    void stopPlane();

    void sendCollisionPlane(QVector3D posA, QVector3D rotA);
    void setControl(ControllerCollision *controlCol);

signals:
    void emtUpdate();
    void emtStatePlane(bool);
    void emtPStop();
    void emtStopTimer();

private slots:
    void movePlane();
    void rotateZPlane();
    void moveCloseWheels();
    void slotStopTimer();

private:
    QOpenGLShaderProgram *m_shader = nullptr;
    VAOEntity *m_vao = nullptr;
    // Entity Plane
    Entity *entityPlaneBudy = nullptr;
    Entity *entityPlaneGlass = nullptr;
    Entity *entityPlaneWheel1 = nullptr;
    Entity *entityPlaneWheel2 = nullptr;
    QList<Entity*> listEntity;
    QList<EntityInfo*> *listEntityInfo = nullptr;

    //
    QVector3D m_dir = QVector3D(0,0,0);

    QTimer *timerPlane;
    QTimer *timerRotatePlane;
    QTimer *timerClose;
    bool stateOpenWheel=false;
    int speedPlane=0;
    float speedPlaneStart=0.0;

    /// Rotation Plane
    float angelPlane_X=0.0;
    float angelPlane_Y=0.0;
    float angelPlane_Z=0.0;
    float resultAngelZ=0.0;
    float m_AxisRightX=0.0;
    float m_AxisLeftY =0.0;

    void movePlaneRotation();

    ControllerCollision *m_controlCol = nullptr;
    bool checking = false;
    QVector3D calcRotatePosition(QVector3D pos, QVector3D rots);
};

#endif // MODELPLANE_H

/// This is Best : smae Car And Wheel Car
//QMatrix4x4 matrixModelCar;
//matrixModelCar.setToIdentity();
//matrixModelCar.translate(modelPlane->getModelPosition());
//matrixModelCar.rotate(modelPlane->getModelRotate().y(),0,1,0);
//matrixModelCar.rotate(modelPlane->getModelRotate().z(),0,0,1);
//matrixModelCar.rotate(modelPlane->getModelRotate().x(),1,0,0);

//for(int i=0; i<modelPlane->getListEntity().count(); ++i)
//{
//    QMatrix4x4 matrixEntityCar;
//    matrixEntityCar.setToIdentity();
//    matrixEntityCar.translate(modelPlane->getListEntity().at(i)->getPosition());
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().y(), 0, 1, 0);
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().z(), 0, 0, 1);
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().x(), 1, 0, 0);
//    shaderProgram.bind();
//    shaderProgram.setUniformValue("mvpMatrix", matrixProj*matrixCamer*(matrixModelCar*matrixEntityCar));
//    shaderProgram.setUniformValue("fragFlag", 1.0f);
//    shaderProgram.setUniformValue("texturTransparency", 1.0f);
//    vboPlane->getListVAO()->at(i)->bind();
//    vboPlane->getTextureImg()->bind();
//    glDrawArrays(GL_TRIANGLES, 0, vboPlane->getListNumVertex().at(i));
//    vboPlane->getTextureImg()->release();
//    vboPlane->getListVAO()->at(i)->release();
//    shaderProgram.release();
//}


/// This is Best : if want rotate Around Another Budy no Around self
//for(int i=0; i<modelPlane->getListEntity().count(); ++i)
//{
//    QMatrix4x4 matrixEntityCar;
//    matrixEntityCar.setToIdentity();
//    matrixEntityCar.translate(modelPlane->getListEntity().at(i)->getPosition()
//                             +modelPlane->getModelPosition());
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().y()
//                          +modelPlane->getModelRotate().y(), 0, 1, 0);
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().z()
//                          +modelPlane->getModelRotate().z(), 0, 0, 1);
//    matrixEntityCar.rotate(modelPlane->getListEntity().at(i)->getRotation().x()
//                          +modelPlane->getModelRotate().x(), 1, 0, 0);
//    QMatrix4x4 matrixAll = matrixEntityCar;
///   this is change : value in matrix not in Object
//    QMatrix4x4 m;
//    m.setToIdentity();
//    if(i==1)
//    {
//        m.translate(-2,0,0);
//        m.rotate(modelPlane->getListEntity().at(i)->getRotation().y(),0,1,0);
//        matrixAll = matrixAll * m;
//    } else if(i==2)
//    {
//        m.translate(2,0,0);
//        matrixAll = matrixAll * m;
//    }
//    shaderProgram.bind();
//    shaderProgram.setUniformValue("mvpMatrix", matrixProj*matrixCamer*matrixAll);
//    shaderProgram.setUniformValue("fragFlag", 1.0f);
//    shaderProgram.setUniformValue("texturTransparency", 1.0f);
//    vboPlane->getListVAO()->at(i)->bind();
//    vboPlane->getTextureImg()->bind();
//    glDrawArrays(GL_TRIANGLES, 0, vboPlane->getListNumVertex().at(i));
//    vboPlane->getTextureImg()->release();
//    vboPlane->getListVAO()->at(i)->release();

//    shaderProgram.release();
//}
