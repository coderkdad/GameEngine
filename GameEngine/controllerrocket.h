#ifndef CONTROLLERROCKET_H
#define CONTROLLERROCKET_H

#include <QObject>
#include <QThread>

class Entity;
class EntityRocket;
class ModelRobot;
class ModelCar;
class ModelPlane;
class ModelErth;
class ModelHome;
class ModelHumanH;

class ControllerRocket : public QObject
{
    Q_OBJECT
public:
    explicit ControllerRocket(QObject *parent = nullptr);
    ~ControllerRocket();


    EntityRocket *getEntityRocket();
    void allEntityCollision(ModelErth *erth, QList<ModelHome*> *listHome,
                         ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelRobot, ModelHumanH *modelCollada=nullptr);

signals:
    void goWork();
    void emtUpdate();
    void emtDeleteRocket(ControllerRocket *thisRocket);


private slots:
    void slotUpdate();
    void slotDelete(EntityRocket *thisRocket);


private:
    QThread thread;
    EntityRocket *rocket;


};

#endif // CONTROLLERROCKET_H
