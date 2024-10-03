#ifndef ENTITYROCKET_H
#define ENTITYROCKET_H

#include <QObject>
#include "entity.h"

class QTimer;
class ControllerCollision;
class ModelRobot;
class ModelCar;
class ModelPlane;
class QCubeLimited;
class ModelErth;
class TestWedget;
class ModelHome;
class ModelHumanH;

class EntityRocket :  public QObject, public Entity
{
    Q_OBJECT
public:
    explicit EntityRocket(QObject *parent = nullptr);
    ~EntityRocket();

    void setDirection(QVector3D dir);
    QVector3D getDirection();

    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome,
                            ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada = nullptr);
    void setStartPos(QVector3D startPos, float speedsRocket=1.0f, float rngRocket=40.0f, float speedTimer=10.0f);
    Entity *getEntitySmoke();
    void setEnableSmoke(bool state);
    bool isEnableSmoke();


signals:
    void emtUpdate();
    void emtDeleteRocket(EntityRocket *thisRocket);

public slots:
    void goBullet();


private slots:
    void timerMoveRocket();

private:
    QVector3D m_dir;
    QVector3D m_startPos;
    float m_speedRocket=0.0f;
    float m_rngRocket=0.0f;
    float m_speedTimer=0.0f;

    QTimer *timerRocket;
    Entity *entitySmoke;

    // Entitys
    ModelErth *m_erth;
    QList<ModelHome*> *m_listHome;
    ModelCar *m_modelCar;
    ModelPlane *m_modelPlane;
    ModelRobot *m_modelRobot;
    ModelHumanH *m_modelCollada;

    bool m_enableSmoke=false;

    bool checkArea();
    bool isCollisionEntity();
    bool checkedCollision(QVector3D pos1, QCubeLimited limit1, QVector3D pos2, QCubeLimited limit2);
    QCubeLimited calcRotLimit2(QCubeLimited *limit, QVector3D rots);
    QVector3D calcRotPosition2(QVector3D pos, QVector3D rots);
};

#endif // ENTITYROCKET_H
