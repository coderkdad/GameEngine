#ifndef MODELROBOT_H
#define MODELROBOT_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include "basemodel.h"

class QOpenGLShaderProgram;
class Entity;
class VAOEntity;
class EntityInfo;
class QCubeLimited;
class QTimer;
class ControllerCollision;
class QOpenGLVertexArrayObject;
class ModelCar;
class ModelPlane;
class TestWedget;

class ModelRobot : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector3D modelPos READ getModelPosition WRITE setModelPosition NOTIFY modelPosChanged)
    Q_PROPERTY(QVector3D modelRot READ getModelRotate WRITE setModelRotate NOTIFY modelRotChanged)

public:
    explicit ModelRobot(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelRobot();
    void createEntity();
    void setModelPosition(QVector3D pos);
    void setModelRotate(QVector3D rot);
    void setModelScale(QVector3D scal);
    void setDirection(QVector3D dir);
    void setModelLimited();
    QVector3D getModelPosition();
    QVector3D getModelRotate();
    QVector3D getModelScale();
    QVector3D getDirection();
    QCubeLimited *getModelLimited();
    QList<Entity *> getListEntity();

    void draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam);

    void setOnTransport(bool check);
    bool checkOn();
    void setLink(bool stateLink);
    bool checkLink();
//    bool checkOnErth();

    void setAxisRightX(double axis);
    void setAxisRotLink(double axis);
    void setAxisRightY(double axis);
    void setAxisLeftX(double axis);
    void setAxisLeftY(double axis);

    void axisAll();

    // Collision
    void setControl(ControllerCollision *controlCol);
    void sendCollisionHuman(QVector3D posA, QVector3D rotA);
    void sendDropHuman(QVector3D posCar, QVector3D direcCar);
    void sendOnErth(bool on);
    void sendStopJump();

    // Animition Gun
    void jumpHuman();
    void animitionGunZoom(bool boolGun);
    void setState_StandSid();
    QString stateHuman();

    QMatrix4x4 getMatrixGun();
    QVector3D getPosGun();
    QVector3D getPosBullet();
    QVector3D getPosThrower();
    QVector3D getPosCamZoom();
    void setEnableZoom(bool state);

signals:
   void emtUpdate();
   void emtStopTimerMove();
   void emtStartTimerDrop();
   void emtStopJump();

   void modelPosChanged();
   void modelRotChanged();
   void emtAnimition();

private slots:
   void moveHuman();
   void animitionMove();
   void slotStopTimerMove();
   void slotTimerDrop();
   void slotStartTimerDrop();
   void slotTimerJump();
   void slotRotLink();
   void slotStopJump();


private:
   QOpenGLShaderProgram *m_shader = nullptr;
   VAOEntity *m_vao = nullptr;
    // Entity Human
    Entity *entityHumanBelly = nullptr;
    Entity *entityHumanChest = nullptr;
    Entity *entityHumanNeck = nullptr;
    Entity *entityHumanHead = nullptr;
    Entity *entityHumanHandRT = nullptr;
    Entity *entityHumanHandRB = nullptr;
    Entity *entityHumanHandLT = nullptr;
    Entity *entityHumanHandLB = nullptr;
    Entity *entityHumanFootRT = nullptr;
    Entity *entityHumanFootRB = nullptr;
    Entity *entityHumanFootLT = nullptr;
    Entity *entityHumanFootLB = nullptr;
    QList<Entity*> listEntity;
    QList<EntityInfo*> *listEntityInfo = nullptr;

    //
    QVector3D modelPos = QVector3D(0,0,0);
    QVector3D modelRot = QVector3D(0,0,0);
    QVector3D modelScal = QVector3D(1,1,1);
    QVector3D modelDirect;
    QCubeLimited *modelLimit = nullptr;

    //Move
    QTimer *timerHumanGo;
    float speedHuman=0.0;
    float m_AxisRightX = 0.0;
    float m_AxisRightXEdit = 0.0;
    float m_AxisRightY = 0.0;
    float m_BaseAxisRightY = 0.0;


    // Collision
    ControllerCollision *m_controlCol = nullptr;
    bool stateCollision = false;

    // Entitys
    Entity *m_erth = nullptr;
    QList<Entity*> *m_listHome = nullptr;
    ModelCar *m_modelCar = nullptr;
    ModelPlane *m_modelPlane = nullptr;

    // Animition Go
    QTimer *timerAnimitionMove;
    float addX = 4.0f;
    float counterRotGo = 0;

    // Animition Jump
    QTimer *timerHumanJump;
    float oldJump_Y=0.0f;
    void animitionJump();

    // Animition Sit&Stand
    QString m_stateHuman="StandUp";
    void animitionSitDown();
    void animitionStandUp();

    // Animition Drop
    QTimer *timerAnimitionDrop;
    QVector3D dropDirectCar;
    QVector3D dropPosCar;

    // Link
    QTimer *timerRotLink;
    bool m_stateLink = false;
    float m_AxisRotLink = 0.0;



    QVector3D getPosFromEntity(QVector3D posEntity, QVector3D posParent, QVector3D rotParent);
    QVector3D getPosFromEntity2(QVector3D posParent, QVector3D rotParent, QVector3D posChild);
    QCubeLimited calcRotLimit2(QCubeLimited *limit, QVector3D rots);
    QVector3D calcRotPosition2(QVector3D pos, QVector3D rots);

    bool m_stateOn = false;
    bool isFoucsGun=false;
    bool m_OnErth = false;

    TestWedget *c_test = nullptr;
    bool m_stateZoom = false;
};

#endif // MODELROBOT_H
