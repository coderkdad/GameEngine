#ifndef ENTITYWEDGE_H
#define ENTITYWEDGE_H

#include <QObject>
#include "entity.h"

class ModelRobot;
class ModelCar;
class ModelPlane;
class QTimer;
class ModelErth;
//class QCubeLimited;
class ModelHome;
class ModelHumanH;

class EntityWedge : public QObject, public Entity
{
    Q_OBJECT
public:
    explicit EntityWedge(QObject *parent = nullptr);
    ~EntityWedge();

    void setDirection(QVector3D dir);
    QVector3D getDirection();

    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome,
                            ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelHuman, ModelHumanH *modelCollada = nullptr);
    void setStartPos(QVector3D startPos, float speedWedge=1.0f, float rngSpace=30.0f, float speedTimer=20.0f);


signals:
    void emtUpdate();
    void emtDeleteWedge(EntityWedge *thisWedge);
    void emtCollisionThat(QString naeEntity, QVector3D *posColl);

public slots:
    void settingWedge();
    void goWedge();

private slots:
    void timerMoveWedge();


private:
    QVector3D m_dir;
    QVector3D m_startPos;
    float m_speedWedge=0.0f;
    float m_rngSpace=0.0f;
    float m_speedTimer=0.0f;

    QTimer *timerWedge;

    // Entitys
    ModelErth *m_erth;
    QList<ModelHome*> *m_listHome;
    ModelCar *m_modelCar;
    ModelPlane *m_modelPlane;
    ModelRobot *m_modelHuman;
    ModelHumanH *m_modelCollada;

    bool checkArea();
    bool isCollisionEntity();
    bool checkedCollision(QVector3D pos1, QCubeLimited limit1, QVector3D pos2, QCubeLimited limit2);
    QCubeLimited calcRotLimit2(QCubeLimited *limit, QVector3D rots);
    QVector3D calcRotPosition2(QVector3D pos, QVector3D rots);

};

#endif // ENTITYWEDGE_H
