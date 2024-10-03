#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QTimer>
#include "entityrocket.h"
#include "entityemitterfire.h"
#include <GL/glu.h>
#include <QKeyEvent>
// Roll Pitch Yaw

class VAOEntity;
class Entity;
class QGamepad;
class QImage;
class ControllerFire;
class Collision;
class ModelCar;
class ModelPlane;
class ModelRobot;
class QThread;
class ControllerCollision;
class ControllerRocket;
class ModelGun;
class ModelGunThrower;
class ModelWater;
class QOpenGLFramebufferObject;
class ModelHome;
class ModelHumanH;

////
class Perlin2D;
class PerlinNoise;

class ModelErth;


class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

    enum OptionPlayer {
        Human = 0x0,
        Car = 0x1,
        Plane = 0x2
    };
    Q_DECLARE_FLAGS(Players, OptionPlayer)


protected:
    QSize sizeHint() const override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    virtual void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    // Plane
    void slotCollPlaneErth(ModelPlane *p, Entity *e, ControllerCollision *c);
    void slotCollPlanetHome(ModelPlane *p, Entity *h, ControllerCollision *c);
    void slotCollPlaneCar(ModelPlane *p, ModelCar *car, ControllerCollision *c);
    void slotCollPlaneHuman(ModelPlane *p, ModelRobot *human, ControllerCollision *c);
    // Car
    void slotCollCarErth(ModelCar *c, Entity *e, ControllerCollision *co);
    void slotCollCarHome(ModelCar *c, Entity *h, ControllerCollision *co);
    void slotCollCarPlane(ModelCar *c, ModelPlane *plane, ControllerCollision *co);
    void slotCollCarHuman(ModelCar *c, ModelRobot *human, ControllerCollision *co);
    // Human
    void slotCollHumanErth(ModelRobot *h, Entity *e, ControllerCollision *c);
    void slotCollHumanHome(ModelRobot *h, Entity *home, ControllerCollision *c);
    void slotCollHumanPlane(ModelRobot *h, ModelPlane *plane, ControllerCollision *c);
    void slotCollHumanCar(ModelRobot *h, ModelCar *car, ControllerCollision *c);
    
private slots:
    void slotUpdate();
    void slotUpdateHuman();
    void slotDeleteRocket(ControllerRocket *thisRockets);
    void slotDeleteBullet(ControllerRocket *thisBullet);
    void slotCreateFire(EntityEmitterFire *thisFire);
    void slotDeleteFire(EntityEmitterFire *thisFire, ControllerFire *thi);
    /// GamePad
    void emtAxisRightX(double v);
    void emtAxisRightY(double v);
    void emtAxisLeftX(double v);
    void emtAxisLeftY(double v);
    void emt_A(bool v);
    void emt_B(bool v);
    void emt_X(bool v);
    void emt_Y(bool v);
    void emt_Up(bool v);
    void emt_Down(bool v);
    void emt_L1(bool v);
    void emt_L2(bool v);
    void emt_R1(bool v);
    void emt_R2(bool v);
    void emt_R3(bool v);
    void emt_Right(bool v);
    void emt_Left(bool v);

    void rotationCamera();
    void rotationZoom();
    void slotCreateBullet();

    void setStatePlayers(MainWidget::Players p);
    void cleanup();

private:
//    QOpenGLContext *m_context;

    QMatrix4x4 matrixProj;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram shaderCollada;

    /// VAO
    bool buildVBOEntity();
    VAOEntity *vaoErth;
    VAOEntity *vaoHomeGood;
    VAOEntity *vaoHomeBad;
    VAOEntity *vaoTree;
    VAOEntity *vaoHuman;
    VAOEntity *vaoCar;
    VAOEntity *vaoPlane;
    VAOEntity *vaoGun;
    VAOEntity *vaoBullet;
    VAOEntity *vaoSmoke;
    VAOEntity *vaoRocket;
    VAOEntity *vaoFire;
    VAOEntity *vaoSkyBox;
    VAOEntity *vaoGunThrower;
    VAOEntity *vaoPoint;
    /// Entity
    Entity *entityGun;
    Entity *entityPoint;
    QList<ModelHome*> *listEntityHomeGood;
    QList<ModelHome*> *listEntityHomeBad;
    QList<Entity*> *listEntityTree;
    QList<ControllerRocket*> *listControllerBullet;
    QList<ControllerRocket*> *listControllerRocket;
    QList<EntityEmitterFire*> *listEntityFireEmitter;
    QList<ControllerFire*> *listControllerFire;
    QList<ControllerCollision*> listConCollision;
    QList<ModelHome *> *getListHome() { return listEntityHomeGood; }
    /// Create Entity
    void createHome(); float getUPs(QVector3D posHome);
    void createRocket(QVector3D posPlane);
    void createFire(QVector3D posPlane);
    void createBullet(QVector3D posGun);
    void createPoint();

    // Model
    ModelErth *modelErth;
    ModelRobot *modelRobot;
    ModelCar *modelCar;
    ModelPlane *modelPlane;
    ModelGunThrower *modelGunThrower;
    ModelGun *modelGun;
    ModelWater *modelWater;
    // Create Model
    void createErth();
    void createHuman();
    void createCar();
    void createPlane();
    void createThrower();
    void createGun();
    void createWater();
    QString checkingModelNow = "non";
    QString checkingGunNow = "Gun";

    /// Draw
    void cullDrawEntity(VAOEntity *entity, QMatrix4x4 mvMatrix, QMatrix4x4 pMatrix, float transparent=1.0f);
    void cullDrawSkyBox(QMatrix4x4 mvMatrix, QMatrix4x4 pMatrix);

    /// GamePad
    QGamepad *m_gamePS;
    bool controlsPlane=false;
    void setupGamePad();
    /// Camera
    QVector3D camPosTran = QVector3D(0.0f, -1.5f, -6.0f); // In Matrix & Far center camPos
    QVector3D camPos = QVector3D(0,0,0); // pos Look & center Matrix
    QVector3D camRot = QVector3D(0,0,0);
    QMatrix4x4 camMatrix;
    void getValModel();
    /// Camer Rot Free
    bool showBack=false;
    bool showBackLink=false;
    bool camStateZoom = false;
    float camRotFreeX = 0.0;
    float camRotFreeY = 0.0;
    float m_axisRightX= 0.0f;
    float m_axisLeftX = 0.0f;
    float m_axisLeftY = 0.0f;
    /// Timers
    QTimer *timerRotCamer;
    QTimer *timerRotZoom;
    QTimer *timerCreateBullet;

    void settingCollision();

    /// SkyBox Test
    QOpenGLVertexArrayObject *vaoSky;
    QOpenGLTexture *mTexture;
//    QOpenGLBuffer *mVertexBuf;
    void loadImageSkyBox();
    void createSkyBox();

    QVector3D calcRotatePosition(QVector3D pos, QVector3D rots);

    QOpenGLFramebufferObject *m_renderFbo = nullptr;
    QImage ImgBuffers;
    void setTexErth();

    // COLLADA
    ModelHumanH *modelCollada;
    void createCollada();

};
Q_DECLARE_OPERATORS_FOR_FLAGS(MainWidget::Players)

#endif // MAINWIDGET_H
