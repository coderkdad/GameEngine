#ifndef MODELCAR_H
#define MODELCAR_H

#include <QObject>
#include <QVector3D>
#include <QtGui/qquaternion.h>
#include "basemodel.h"

class QOpenGLShaderProgram;
class Entity;
class VAOEntity;
class EntityInfo;
class QCubeLimited;
class QTimer;
class ControllerCollision;
class QQuaternion;

class ModelCar : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelCar(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelCar();
    void createEntity();

    QList<Entity *> getListEntity();

    void setDirection(QVector3D dir);
    QVector3D getDirection();

    void draw(QMatrix4x4 matrixProCam);
    void setAxisRightX(float v);
    void speedUp();
    void speedDown();


    void sendCollisionCar(QVector3D posA, QVector3D rotA);
    void setControl(ControllerCollision *controlCol);

signals:
    void emtUpdate();
    void emtStateCar(bool);
    void emtStopTimer();

private slots:
    void moveCar();
    void slotStopTimer();

private:
    QOpenGLShaderProgram *m_shader = nullptr;
    VAOEntity *m_vao = nullptr;
    // Entity Car
    Entity *entityCarBudy = nullptr;
    Entity *entityCarWheel1 = nullptr;
    Entity *entityCarWheel2 = nullptr;
    Entity *entityCarWheel3 = nullptr;
    Entity *entityCarWheel4 = nullptr;
    QList<Entity*> listEntity;
    QList<EntityInfo*> *listEntityInfo = nullptr;

    //
    QVector3D m_dir;
    QQuaternion quatAll;

    QTimer *timerCar = nullptr;
    float angelCar_Y=0.0f;
    float speedCar = 0.0;
    float m_AxisRightX=0.0;

    ControllerCollision *m_controlCol = nullptr;
    bool checking = false;

    // Test Convert
    void convertEluerTOQuat(float roll, float pitch, float yaw);
    void convertQuatTOEuler(QQuaternion quat);
};

#endif // MODELCAR_H
