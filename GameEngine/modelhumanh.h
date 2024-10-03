#ifndef MODELHUMANH_H
#define MODELHUMANH_H

#include <QObject>
#include "basemodel.h"
#include <QVector3D>
#include <QVector2D>

class QOpenGLShaderProgram;
class VAOColladaAssimp;
class QCubeLimited;
class QTimer;
class ControllerCollision;

class ModelHumanH : public QObject, public BaseModel
{
    Q_OBJECT

public:
    explicit ModelHumanH(QOpenGLShaderProgram *shader, QObject *parent = nullptr);
    bool createEntity();
    void setModelPosition(QVector3D pos);
    void setModelRotate(QVector3D rot);
    void setDirection(QVector3D dir);
    void setControl(ControllerCollision *controlCol);
    Q_INVOKABLE QVector3D getDirection();

    void draw(QMatrix4x4 matrixProCam, QMatrix4x4 matrixCam);

    void setAxisRightX(double axis);
    void setAxisRightY(double axis);
    void setAxisLeftX(double axis);
    void setAxisLeftY(double axis);
    void setHumanDown();
    void jump();

    void setOnTransport(bool check);
    bool checkOn();

    void setLink(bool stateLink);
    bool checkLink();
    void setAxisRotLink(double axis);

    void sendCollisionCollada(QVector3D posA, QVector3D rotA, bool onErth);
    void sendNotCollisionCollada(QVector3D posA, QVector3D rotA, bool onErth);
    void sendOnErth(bool on);
    QMatrix4x4 getPosGun(int x);

signals:
    void emtUpdate();
//    void emtStopTimerMove(QVector3D pos, QVector3D rot, bool onErth);
//    void emtCollision(QVector3D pos, QVector3D rot, bool onErth);
//    void emtNotCollision(QVector3D pos, QVector3D rot, bool onErth);

public slots:
    void slotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);
    void slotNotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth);

private slots:
    void moveHuman();
    void slotTimerJump();
//    void slotStopTimerMove(QVector3D pos, QVector3D rot, bool onErth);
//    void slotCollision(QVector3D pos, QVector3D rot, bool onErth);
//    void slotNotCollision(QVector3D pos, QVector3D rot, bool onErth);

    void slotRotLink();

private:
    QOpenGLShaderProgram *m_shader;
    VAOColladaAssimp *m_vaoAssimp;
    
    QVector3D modelPos = QVector3D(0,0,0);
    QVector3D modelRot = QVector3D(0,0,0);
    QVector3D modelScal = QVector3D(1,1,1);
    QVector3D m_dir;

    float speedHuman = 0;
    float speedHumanOld = 0;
    float m_AxisRightX = 0.0;
    float m_AxisRightXEdit = 0.0;
    float m_AxisRightXEditOld = 0.0;
    float m_AxisRightY = 0.0;
    void axisAll();

    //Move
    QTimer *timerHumanGo;
    bool mStateHumanGo=true;
    bool mStateHumanDown=false;
    ControllerCollision *m_controlCol;
    void setStateHumanGo(bool state, int stateNow=-1);

    // limited
    QCubeLimited *limFull;
    void settingLimited();

    // Jump
    QTimer *timerJump;
    float oldJump_Y = 0.0f;
    bool m_OnErth = false;

    //Link
    bool m_stateLink = false;
    bool m_stateOn = false;

};

#endif // MODELHUMANH_H
