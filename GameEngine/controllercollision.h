#ifndef CONTROLLERCOLLISION_H
#define CONTROLLERCOLLISION_H

#include <QObject>
#include <QThread>

class QTimer;
class EntityRocket;
class Entity;
class ModelCar;
class ModelPlane;
class ModelRobot;
class ModelErth;
class Collision;
class ModelHome;
class ModelHumanH;

class ControllerCollision : public QObject
{
    Q_OBJECT
public:
    explicit ControllerCollision();
    ~ControllerCollision();

    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome, ModelCar *modelCar, ModelPlane *modelPlane,
                         ModelRobot *modelRobot, ModelHumanH *modelCollada=nullptr);

    void startColPlane(int strPriority);
    void startColCar(int strPriority);
    void startColHuman(int strPriority);
    void startColCollada(int strPriority);
    void startControl();
    void stopControl();
    void setStateGrv(bool state);

signals:
    void goWork(bool);
    void goWork2(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
    // Plane
    void emtConCollPlaneErth(ModelPlane *p, Entity *e, ControllerCollision *c);
    void emtConCollPlaneHome(ModelPlane *p, Entity *h, ControllerCollision *c);
    void emtConCollPlaneCar(ModelPlane *p, ModelCar *car, ControllerCollision *c);
    void emtConCollPlaneHuman(ModelPlane *p, ModelRobot *human, ControllerCollision *c);
    // Car
    void emtConCollCarErth(ModelCar *c, Entity *e, ControllerCollision *co);
    void emtConCollCarHome(ModelCar *c, Entity *h, ControllerCollision *co);
    void emtConCollCarPlane(ModelCar *c, ModelPlane *plane, ControllerCollision *co);
    void emtConCollCarHuman(ModelCar *c, ModelRobot *human, ControllerCollision *co);
    // Human
    void emtConCollHumanErth(ModelRobot *h, Entity *e, ControllerCollision *c);
    void emtConCollHumanHome(ModelRobot *h, Entity *home, ControllerCollision *c);
    void emtConCollHumanPlane(ModelRobot *h, ModelPlane *plane, ControllerCollision *c);
    void emtConCollHumanCar(ModelRobot *h, ModelCar *car, ControllerCollision *c);
    //Collada
    void emtCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);
    void emtNotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);

public slots:
    void stateControl(bool state);
    void stateControl2(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

private slots:
    // Plane
    void slotCollPlaneWithErth(ModelPlane *p, Entity *e);
    void slotCollPlaneWithHome(ModelPlane *p, Entity *h);
    void slotCollPlaneWithCar(ModelPlane *p, ModelCar *car);
    void slotCollPlaneWithHuman(ModelPlane *p, ModelRobot *human);
    // Car
    void slotCollCarWithErth(ModelCar *c, Entity *e);
    void slotCollCarWithHome(ModelCar *c, Entity *h);
    void slotCollCarWithPlane(ModelCar *c, ModelPlane *plane);
    void slotCollCarWithHuman(ModelCar *c, ModelRobot *human);
    // Plane
    void slotCollHumanWithErth(ModelRobot *h, Entity *e);
    void slotCollHumanWithHome(ModelRobot *h, Entity *home);
    void slotCollHumanWithPlane(ModelRobot *h, ModelPlane *plane);
    void slotCollHumanWithCar(ModelRobot *h, ModelCar *car);
    //Collada
    void slotColCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);
    void slotNotCollCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);
private:
    QThread thread ;
    Collision *col;
    //list Entity
    ModelErth *m_erth;
    QList<ModelHome*> *m_listHome;
    ModelCar *m_modelCar;
    ModelPlane *m_modelPlane;
    ModelRobot *m_modelRobot;
    ModelHumanH *m_modelCollada;

    bool stateGrav = true;



};

#endif // CONTROLLERCOLLISION_H
