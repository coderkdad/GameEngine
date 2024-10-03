#ifndef CONTROLLERTHROWER_H
#define CONTROLLERTHROWER_H

#include <QObject>
#include <QThread>

class Entity;
class EntityWedge;
class ModelRobot;
class ModelCar;
class ModelPlane;
class ModelErth;
class ModelHome;
class ModelHumanH;

class ControllerThrower : public QObject
{
    Q_OBJECT
public:
    explicit ControllerThrower(QObject *parent = nullptr);
    ~ControllerThrower();

    EntityWedge *getEntityWedge();
    void allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                         ModelRobot *modelHuman, ModelCar *modelCar, ModelPlane *modelPlane, ModelHumanH *modelCollada=nullptr);
    void runWedge();

signals:
    void settingWork();
    void goWork();
    void emtUpdate();
    void emtDeleteWedge(ControllerThrower *thisWedge);
    void emtCollisionThat(QString name, QVector3D *posColl);


private slots:
    void slotUpdate();
    void slotDelete(EntityWedge *thisWedge);
    void slotCollisionThat(QString name, QVector3D *posColl);


private:
    QThread thread;
    EntityWedge *wedge;

};

#endif // CONTROLLERTHROWER_H
