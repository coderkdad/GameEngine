#ifndef MODELGUNTHROWER_H
#define MODELGUNTHROWER_H

#include <QObject>
#include <QVector3D>
#include "basemodel.h"

class Entity;
class QOpenGLShaderProgram;
class VAOEntity;
class QMatrix4x4;
class QTimer;
class ControllerThrower;
class EntityWedge;
class ModelCar;
class ModelPlane;
class ModelRobot;
class ModelErth;
class ModelHome;
class ModelHumanH;

class ModelGunThrower : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelGunThrower(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelGunThrower();
    void createEntity();
    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome,
                            ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelHuman, ModelHumanH *modelCollada=nullptr);

//    void setModelPosition(QVector3D pos);
//    void setModelRotate(QVector3D rot);
//    void setModelScale(QVector3D scal);
    void setDirection(QVector3D dir);
//    Q_INVOKABLE QVector3D getModelPosition();
//    Q_INVOKABLE QVector3D getModelRotate();
//    Q_INVOKABLE QVector3D getModelScale();
    Q_INVOKABLE QVector3D getDirection();
    Q_INVOKABLE QList<Entity *> getListEntity();

    void draw(QMatrix4x4 matrixProCam);
    void shutThrower(QVector3D posWedge, QVector3D rotWedge, QVector3D dirWedge);
    void setRotUp(float rotUp);

signals:

private slots:
    void slotUpdateWedge();
    void slotDeleteWedge();
    void slotCollisionWedge(QString nameEntity, QVector3D *posColl);
    void slotUpdateHuman();
    void slotUpdatePlane();
    void slotTimerMoveUpHuman();

private:
    QOpenGLShaderProgram *m_shader = nullptr;
    VAOEntity *m_vao = nullptr;
    //
//    QVector3D modelPos = QVector3D(0,0,0);
//    QVector3D modelRot = QVector3D(0,0,0);
//    QVector3D modelScal = QVector3D(1,1,1);
    QVector3D m_dir;
    //
    Entity *entityThrower = nullptr;
    Entity *entityRope = nullptr;
    Entity *entityWedge = nullptr;
    QList<Entity*> listEntity;
    QList<Entity*> listRope;
    QList<ControllerThrower*> listControllerWedge;
    float m_hieghtRope = 0.0f;

    void createConWedge();
    void createRope();
    void clearThrower();

    // Entitys
    ModelErth *m_erth = nullptr;
    QList<ModelHome*> *m_listHome = nullptr;
    ModelCar *m_modelCar = nullptr;
    ModelPlane *m_modelPlane = nullptr;
    ModelRobot *m_modelHuman = nullptr;
    ModelHumanH *m_modelCollada= nullptr;

    QTimer *timerMoveUpWedge;
    bool m_WedgeGo = false;
    bool m_WedgeDelete = false;
    bool m_WedgeLink = false;
    float m_rotUp = 0.0f;
    QVector3D m_vecLinkHuman;
    QVector3D m_vecLinkWedge;
    QVector3D m_vecLinkRope;
    QVector3D m_rotPlane;
    QVector3D m_rotHuman;

    void setVecLink();
    void setPosAllMove();

    QVector3D calcRotPosition2(QVector3D pos, QVector3D rots);
    QVector3D getPosFromEntity2(QVector3D posParent, QVector3D rotParent, QVector3D posChild);

};

#endif // MODELGUNTHROWER_H
