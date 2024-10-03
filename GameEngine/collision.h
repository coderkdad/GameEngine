#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>

class QTimer;
class Entity;
class EntityRocket;
class ModelCar;
class ModelRobot;
class ModelHumanH;
class ModelPlane;
class ModelErth;
class QTriangle3D;
class QCubeLimited;
class ModelHome;


class Collision : public QObject
{
    Q_OBJECT
public:
    explicit Collision(QObject *parent = nullptr);
    ~Collision();
    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome,
                            ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada=nullptr);
    void setStateGravy(bool state);

signals:
    void emtFinished();
    //Plane
    void emtCollPlaneWithErth(ModelPlane *p, Entity *e);
    void emtCollPlaneWithHome(ModelPlane *p, Entity *h);
    void emtCollPlaneWithCar(ModelPlane *p, ModelCar *car);
    void emtCollPlaneWithHuman(ModelPlane *p, ModelRobot *human);
    //Car
    void emtCollCarWithErth(ModelCar *c, Entity *e);
    void emtCollCarWithHome(ModelCar *c, Entity *h);
    void emtCollCarWithPlane(ModelCar *c, ModelPlane *plane);
    void emtCollCarWithHuman(ModelCar *c, ModelRobot *human);
    //Human
    void emtCollHumanWithErth(ModelRobot *h, Entity *e);
    void emtCollHumanWithHome(ModelRobot *h, Entity *home);
    void emtCollHumanWithPlane(ModelRobot *h, ModelPlane *plane);
    void emtCollHumanWithCar(ModelRobot *h, ModelCar *car);
    //Collada
    void emtCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);
    void emtNotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);

public slots:
    void workCollisionPlane(bool state);
    void workCollisionCar(bool state);
    void workCollisionHuman(bool state);
    void slotTimerHuman(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    void slotTimerCollada(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    void slotTimerCar(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    void slotTimerPlane(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    void slotTimerGrav();

private slots:

private:
    bool funGravity(QVector3D *posHuman, QCubeLimited *limitHuman);
    QTimer *timerR;
    QTimer *timerP;
    QTimer *timerC;
    QTimer *timerH;
    QTimer *timerGrav;
    float rngGrav=0.02f;
    bool stateGravy=true;

    ModelErth *entityErth;
    QList<ModelHome*> *listEntityHome;
    ModelRobot *m_modelRobot;
    ModelHumanH *m_modelCollada;
    ModelCar *m_modelCar;
    ModelPlane *m_modelPlane;

    QVector3D calcRotPosition(QVector3D pos, int rots);
    QVector3D calcRotPosition2(QVector3D pos, QVector3D rots);
    QCubeLimited calcRotLimit(QCubeLimited *limit, int rots);
    QCubeLimited calcRotLimit2(QCubeLimited *limit, QVector3D rots);

    QCubeLimited calcRotTriangle(QTriangle3D trng, int rots);
    bool checkedCollision(QVector3D pos1, QCubeLimited limit1,QVector3D pos2, QCubeLimited limit2);

};

#endif // COLLISION_H
