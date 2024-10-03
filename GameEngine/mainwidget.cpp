#include "mainwidget.h"
#include "math.h"
#include "qmath.h"
#include "entity.h"
#include "gamePS.h"
#include "controllerfire.h"
#include "controllercollision.h"
#include "collision.h"
#include "vaoentity.h"
#include <QtGamepad/QGamepad>
#include <QLoggingCategory>
#include <QOpenGLTexture>
#include <QDateTime>
#include <QElapsedTimer>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QDebug>
#include "modelcar.h"
#include "modelplane.h"
#include "modelrobot.h"
#include "modelgunthrower.h"
#include <QQuaternion>
#include <QMetaObject>
#include <QThread>
#include "threadfire.h"
#include <controllerrocket.h>
#include <QKeyEvent>

#include "PerlinNoise.h"
#include "modelwater.h"
#include "modelerth.h"
#include "modelgun.h"
#include "modelhome.h"

#include <QOpenGLContext>
#include <QSurface>
#include <QOpenGLFramebufferObjectFormat>

#include "modelhumanh.h"

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

//    m_context = new QOpenGLContext;
////    m_context->setFormat(formate);
//    m_context->create();
//    m_context->makeCurrent(this);

    setGeometry(700,10, 640, 480);
    //List
    listEntityHomeGood = new QList<ModelHome*>;
    listEntityHomeBad = new QList<ModelHome*>;
    listEntityTree = new QList<Entity*>;
    listControllerRocket= new QList<ControllerRocket*>;
    listControllerBullet = new QList<ControllerRocket*>;
    listEntityFireEmitter = new QList<EntityEmitterFire*>;
    listControllerFire = new QList<ControllerFire*>; // Online get Fire From thread :(

    //Timer
    timerRotCamer = new QTimer(this);
    connect(timerRotCamer, SIGNAL(timeout()), this, SLOT(rotationCamera()));
    timerRotZoom = new QTimer(this);
    connect(timerRotZoom, SIGNAL(timeout()), this, SLOT(rotationZoom()));
    timerCreateBullet = new QTimer(this);
    connect(timerCreateBullet, &QTimer::timeout, this, &MainWidget::slotCreateBullet);

    //GamePad
    setupGamePad();

//    QTimer::singleShot(10, this, SLOT(update()));
    QTimer *tUp = new QTimer;
    connect(tUp, SIGNAL(timeout()), this, SLOT(update()));
    tUp->start(20);
}
MainWidget::~MainWidget()
{
    makeCurrent();
    qDebug("Finish Destroy 1");
    // VAO
    delete vaoErth;
    vaoErth = 0;
    qDebug()<<"Finish Destroy 11 "<<vaoErth;

    delete vaoHomeGood;
    vaoHomeGood = nullptr;
    qDebug()<<"Finish Destroy 111 " << vaoHomeGood;
    delete vaoHomeBad;
    vaoHomeBad = NULL;
    qDebug()<<"Finish Destroy 1111 "<< vaoHomeBad;
    delete vaoTree;
    delete vaoHuman;
    delete vaoCar;
    delete vaoPlane;
    delete vaoGun;
    delete vaoBullet;
    delete vaoSmoke;
    delete vaoRocket;
    delete vaoFire;
    delete vaoSkyBox;
    delete vaoGunThrower;
    delete vaoPoint;

    // Entity
//    delete entityErth;
    qDebug("X");
//    delete entityGun;
    delete  modelGun;
    modelGun = nullptr;
    qDebug("W");
    delete entityPoint; //entityPoint->~Entity();
    qDebug("Z");

    // Model
//    qDebug()<<"Hu: "<<modelRobot;
//    delete modelRobot;
//    modelRobot = nullptr;
//    qDebug()<<"Hu 2: "<<modelRobot;
    delete modelCar;
    delete modelPlane;
    delete modelGunThrower;
    qDebug("Finish Destroy 2");
    delete modelWater;
    modelErth->deleteLater();
    qDebug("Finish Destroy 3");

    // List
//    QList<Entity*> *listEntityHomeGood;
//    QList<Entity*> *listEntityHomeBad;
//    QList<Entity*> *listEntityTree;
//    QList<ControllerRocket*> *listControllerBullet;
//    QList<ControllerRocket*> *listControllerRocket;
//    QList<EntityEmitterFire*> *listEntityFireEmitter;
//    QList<ControllerFire*> *listControllerFire;
//    QList<ControllerCollision*> listConCollision;

    ///============== FOR ==============///
    for(int i=0; i<listEntityHomeGood->count(); ++i)
    {
        ModelHome *e = listEntityHomeGood->at(i);
        listEntityHomeGood->removeAt(i);
        delete e;
        e = 0;
    }
    delete listEntityHomeGood;
    listEntityHomeGood = 0;

    for(int i=0; i<listEntityHomeBad->count(); ++i)
    {
        ModelHome *e = listEntityHomeBad->at(i);
        listEntityHomeBad->removeAt(i);
        delete e;
        e = 0;
    }
    delete listEntityHomeBad;
    listEntityHomeBad = 0;

    for(int i=0; i<listEntityTree->count(); ++i)
    {
        Entity *e = listEntityTree->at(i);
        listEntityTree->removeAt(i);
        delete e;
        e = 0;
    }
    delete listEntityTree;
    listEntityTree = 0;
    qDebug("FOR 1");
    ///============================///

    for(int i=0; i<listControllerRocket->count(); ++i)
    {
        ControllerRocket *e = listControllerRocket->at(i);
        listControllerRocket->removeAt(i);
        delete e;
        e = 0;
    }
    delete listControllerRocket;
    listControllerRocket = 0;

    for(int i=0; i<listControllerBullet->count(); ++i)
    {
        ControllerRocket *e = listControllerBullet->at(i);
        listControllerBullet->removeAt(i);
        delete e;
        e = 0;
    }
    delete listControllerBullet;
    listControllerBullet = 0;

    qDebug("FOR 2");
    ///============================///
    // Etc
//    delete m_gamePS;
    timerRotCamer->deleteLater();
//    delete vaoSky;
//    delete mTexture;
    doneCurrent();
    qDebug("Finish Destroy 9");
}
void MainWidget::cleanup()
{
//    if(&shaderProgram == nullptr)
//        return;

//    makeCurrent();
//    vaoErth->destroy();
//    delete shaderProgram;
//    shaderProgram = 0;
//    doneCurrent();
}

QSize MainWidget::sizeHint() const
{
    return QSize(640, 480);
}
void MainWidget::resizeGL(int w, int h)
{
    matrixProj.setToIdentity();
    matrixProj.perspective(45.0f, float(w) / float(h), 0.01f, 1000);
    glViewport(0, 0, w, h);
}
void MainWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MainWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClearColor(0.39f, 0.58f, 0.93f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //glCullFace(GL_BACK); glFrontFace(GL_CW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable ( GL_LIGHTING ) ;  /*glEnable(GL_LIGHT0);*/ ///LIGHTING
//    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glLoadIdentity();

    //Shaders
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.fsh");
    shaderProgram.link();

    shaderCollada.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh");
    shaderCollada.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh");
    shaderCollada.link();

    //Shader Water
//    shaderWater.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/aloool14/Documents/ExQt/water/w1/glWater/shader.vsh");
//    shaderWater.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/aloool14/Documents/ExQt/water/w1/glWater/shader.fsh");
//    shaderWater.link();

    //Create Entity And VBO
    if(buildVBOEntity())
    {
        /// After
        createErth();
        createHome();
        createPlane();
        createCar();
        createCollada();
//        createHuman();
        createGun();
//        createThrower();
//        createPoint();
//        createWater();
//        createBullet();
        settingCollision();
        // sky Box
    //    loadImageSkyBox();
//        createSkyBox();

    }
}
void MainWidget::paintGL()
{
//    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//    glDrawBuffer()
    /// ========= World Camera  ========== ///
    /// الكاميرا لها موقعين لتغيير التحكم
    /// لتغيير موقع الكاميرا بدل المنتصف نضيف القيم الى :

    /// matrixCamer.translate(0,-1.5f,-10)
    /// وهذا يجعل الكاميرا تدور حول مركز الرؤية
    /// أو
    /// cameraPosition (ex+0,ey+1.5f,ez+10)
    /// وهذا يجعل الكاميرا تدور حول نفسها
    /// ================================== ////

    getValModel();
    qDebug("RER");

    QMatrix4x4 matrixCamer;
    {
        matrixCamer.setToIdentity();
        matrixCamer.translate(camPosTran.x(), camPosTran.y(), camPosTran.z());

        if(/*showBack ||*/ showBackLink || camStateZoom)
        { matrixCamer.rotate((360-camRot.x())+(camRotFreeX-90), 1, 0, 0); }
        else
        { matrixCamer.rotate((360-camRot.x()), 1, 0, 0); }

        if(showBack && !modelCollada->checkLink())
        { matrixCamer.rotate((360-camRot.y())+camRotFreeY, 0, 1, 0); }
        else
        { matrixCamer.rotate((360-camRot.y()), 0, 1, 0); }

        matrixCamer.rotate(0, 0, 0, 1); // Problem rot Z

        QVector3D cameraPosition = QVector3D(camPos.x(),
                                             camPos.y(),
                                             camPos.z()); // مركز الكاميرا الذي تدور حوله
        QVector3D cameraUpDirection = QVector3D(0,1,0);
        QVector3D cameraLook = QVector3D(camPos.x(),
                                         camPos.y(),
                                         camPos.z()-0.001f); // النقطة التي تنظر لها الكاميرا
        matrixCamer.lookAt(cameraPosition, cameraLook, cameraUpDirection);


        QMatrix4x4 mtrxCam;
        mtrxCam.setToIdentity();
        mtrxCam.translate(camPos);

        if(showBack && !modelCollada->checkLink())
        { mtrxCam.rotate((camRot.y())-camRotFreeY, 0, 1, 0); }
        else
        { mtrxCam.rotate((camRot.y()), 0, 1, 0); }

        mtrxCam.rotate(camRot.z(), 0,0,1);

        if(showBack || showBackLink || camStateZoom)
        { mtrxCam.rotate((camRot.x())-camRotFreeX, 1, 0, 0);}
        else
        { mtrxCam.rotate((camRot.x()), 1, 0, 0); }

        /// ======== Point ========== ///

//        QMatrix4x4 matrixPoint;
//        matrixPoint.setToIdentity();
//        matrixPoint.translate(entityPoint->getPosition());
//        matrixPoint.rotate(entityPoint->getRotation().y(), 0,1,0);
//        matrixPoint.rotate(entityPoint->getRotation().z(), 0,0,1);
//        matrixPoint.rotate(entityPoint->getRotation().x(), 1,0,0);
//        cullDrawEntity(vaoPoint, matrixCamer*mtrxCam*matrixPoint, matrixProj);
    }


    ///============== Collada ===========///
    modelCollada->draw(matrixProj*matrixCamer, matrixCamer);

    /// =============== SKYBOX ==================///
//    QMatrix4x4 matrixSkyBox;
//    matrixSkyBox.setToIdentity();
//    cullDrawSkyBox(matrixCamer*matrixSkyBox, matrixProj);

    ///============ Model Human ============///
//    if(checkingModelNow == "non")
//    modelRobot->draw(matrixProj, matrixCamer);

    /// ========== World Plane  ========== ///

    //Plane Me
    modelPlane->draw(matrixProj*matrixCamer);

    // Buffer Test
//    qDebug("E");
//    if (m_renderFbo == nullptr) {
//        QOpenGLFramebufferObjectFormat format;
//        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//        format.setSamples(16);
//        m_renderFbo = new QOpenGLFramebufferObject(size(), format);
//    }
//    qDebug("J");
//    m_renderFbo->bind();
//    modelPlane->draw(matrixProj*matrixCamer);
////    context()->;
//    m_renderFbo->release();
//    qDebug("W");

    /// ========= World Erth  ========== ///

    {
        modelErth->draw(matrixProj,matrixCamer);
//        QMatrix4x4 matrixErth;
//        matrixErth.setToIdentity();
//        matrixErth.translate(modelErth->getModelPosition());
//        matrixErth.rotate(modelErth->getModelRotate().x(), 1.0, 0.0, 0.0);
//        matrixErth.rotate(modelErth->getModelRotate().y(), 0.0, 1.0, 0.0);
//        matrixErth.rotate(modelErth->getModelRotate().z(), 0.0, 0.0, 1.0);
//        cullDrawEntity(vaoErth, matrixCamer*matrixErth, matrixProj);
    }

    /// ========== World Home ========= ///
    for(int i=0; i<listEntityHomeGood->count(); ++i)
    {
//        QMatrix4x4 matrixHome;
//        matrixHome.setToIdentity();
//        matrixHome.translate(listEntityHomeGood->at(i)->getPosition());
//        matrixHome.rotate(listEntityHomeGood->at(i)->getRotation().x(), 1.0, 0.0, 0.0);
//        matrixHome.rotate(listEntityHomeGood->at(i)->getRotation().y(), 0.0, 1.0, 0.0);
//        matrixHome.rotate(listEntityHomeGood->at(i)->getRotation().z(), 0.0, 0.0, 1.0);
//        cullDrawEntity(vaoHomeGood, matrixCamer*matrixHome, matrixProj);
        listEntityHomeGood->at(i)->draw(matrixProj, matrixCamer);
    }
//    for(int i=0; i<listEntityHomeBad->count(); ++i)
//    {
//        QMatrix4x4 matrixHome;
//        matrixHome.setToIdentity();
//        matrixHome.translate(listEntityHomeBad->at(i)->getPosition());
//        matrixHome.rotate(listEntityHomeBad->at(i)->getRotation().x(), 1.0, 0.0, 0.0);
//        matrixHome.rotate(listEntityHomeBad->at(i)->getRotation().y(), 0.0, 1.0, 0.0);
//        matrixHome.rotate(listEntityHomeBad->at(i)->getRotation().z(), 0.0, 0.0, 1.0);
//        cullDrawEntity(vaoHomeBad, matrixCamer*matrixHome, matrixProj);
//    }

    // Tree
//    for(int i=0; i<listEntityHomeGood->count(); ++i)
//    {
//        QMatrix4x4 matrixTree;
//        matrixTree.setToIdentity();
//        matrixTree.translate(listEntityTree->at(i)->getPosition());
//        matrixTree.rotate(listEntityTree->at(i)->getRotation().x(), 1.0, 0.0, 0.0);
//        matrixTree.rotate(listEntityTree->at(i)->getRotation().y(), 0.0, 1.0, 0.0);
//        matrixTree.rotate(listEntityTree->at(i)->getRotation().z(), 0.0, 0.0, 1.0);
//        cullDrawEntity(vaoTree, matrixCamer*matrixTree, matrixProj);
//    }

    /// =========== World Rocket ========== ///
    for(int i=0; i<listControllerRocket->count(); ++i)
    {

        QMatrix4x4 matrixRocket;
        matrixRocket.setToIdentity();
        matrixRocket.translate(listControllerRocket->at(i)->getEntityRocket()->getPosition());
        matrixRocket.rotate(listControllerRocket->at(i)->getEntityRocket()->getRotation().y(), 0.0, 1.0, 0.0);
        matrixRocket.rotate(listControllerRocket->at(i)->getEntityRocket()->getRotation().z(), 0.0, 0.0, 1.0);
        matrixRocket.rotate(listControllerRocket->at(i)->getEntityRocket()->getRotation().x(), 1.0, 0.0, 0.0);

        qDebug("DRAW OO");
        cullDrawEntity(vaoRocket, matrixCamer*matrixRocket, matrixProj);
        qDebug("DRAW OO2");

//        // Smoke
//        bool isSmoke = listControllerRocket->at(i)->getEntityRocket()->isEnableSmoke();
//        if(isSmoke)
//        {
//            QMatrix4x4 matrixSmoke;
//            matrixSmoke.setToIdentity();
//            matrixSmoke.translate(listControllerRocket->at(i)->getEntityRocket()->getEntitySmoke()->getPosition());
//            matrixSmoke.rotate(listControllerRocket->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().y(), 0,1,0);
//            matrixSmoke.rotate(listControllerRocket->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().z(), 0,0,1);
//            matrixSmoke.rotate(listControllerRocket->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().x(), 1,0,0);
//            matrixSmoke.scale(listControllerRocket->at(i)->getEntityRocket()->getEntitySmoke()->getScale());
//            cullDrawEntity(vaoSmoke, matrixCamer*matrixSmoke, matrixProj, 0.5f);
//        }
    }


    /// =========== World Fire Partecale =========== ///
    for(int i=0; i<listEntityFireEmitter->count(); ++i)
    {
        for(int j=0; j<listEntityFireEmitter->at(i)->listParticale().count(); ++j)
        {
            QVector3D vecPos = listEntityFireEmitter->at(i)->listParticale().at(j)->getPosition();
            QVector3D vecScl = listEntityFireEmitter->at(i)->listParticale().at(j)->getScale();
            float valTrans = listEntityFireEmitter->at(i)->listParticale().at(j)->getTransparency();

            QMatrix4x4 matrixFire;
            matrixFire.setToIdentity();
            matrixFire.translate(vecPos);
            matrixFire.rotate(0,1,0,0);
            matrixFire.rotate(0,0,1,0);
            matrixFire.rotate(0,0,0,1);
            matrixFire.scale(vecScl);
            cullDrawEntity(vaoFire, matrixCamer*matrixFire, matrixProj, valTrans);
        }
    }

    /// =========== World Car  =========== ///
    modelCar->draw(matrixProj*matrixCamer);

    ///============ World Gun ============///
    if(checkingGunNow == "Gun")
    {
        modelGun->draw(matrixProj, matrixCamer);
    }
//    else if(checkingGunNow == "Thrower")
//    {
//        modelGunThrower->draw(matrixProj*matrixCamer);
//    }

    ///============ World Bullet ============///
    for(int i=0; i<listControllerBullet->count(); ++i)
    {
        QMatrix4x4 matrixBullet;
        matrixBullet.setToIdentity();
        matrixBullet.translate(listControllerBullet->at(i)->getEntityRocket()->getPosition());
        matrixBullet.rotate(listControllerBullet->at(i)->getEntityRocket()->getRotation().y(), 0.0, 1.0, 0.0);
        matrixBullet.rotate(listControllerBullet->at(i)->getEntityRocket()->getRotation().z(), 0.0, 0.0, 1.0);
        matrixBullet.rotate(listControllerBullet->at(i)->getEntityRocket()->getRotation().x(), 1.0, 0.0, 0.0);
        cullDrawEntity(vaoBullet, matrixCamer*matrixBullet, matrixProj);


//        // Smoke
//        bool isSmoke = listControllerBullet->at(i)->getEntityRocket()->isEnableSmoke();
//        if(isSmoke)
//        {
//            QMatrix4x4 matrixSmoke;
//            matrixSmoke.setToIdentity();
//            matrixSmoke.translate(listControllerBullet->at(i)->getEntityRocket()->getEntitySmoke()->getPosition());
//            matrixSmoke.rotate(listControllerBullet->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().y(), 0,1,0);
//            matrixSmoke.rotate(listControllerBullet->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().z(), 0,0,1);
//            matrixSmoke.rotate(listControllerBullet->at(i)->getEntityRocket()->getEntitySmoke()->getRotation().x(), 1,0,0);
//            matrixSmoke.scale(listControllerBullet->at(i)->getEntityRocket()->getEntitySmoke()->getScale());
//            cullDrawEntity(vaoSmoke, matrixCamer*matrixSmoke, matrixProj, 0.5f);
//        }
    }


    ///============ World Water ============///
//    modelWater->draw(matrixProj, matrixCamer);

//    doneCurrent();
    // Pass W8_Raq+9f // Wzuum8 // 048699522004
//    ImgBuffers = m_renderFbo->toImage(); //grabFramebuffer(); // Entity After This not show result Img
//    qDebug("Z");
//    setTexErth();
//    this->glGetFramebufferAttachmentParameteriv()



}

void MainWidget::setTexErth()
{
//    vaoErth->setTextureImg2(ImgBuffers);
}

/// ================== Draw ================== ///
void MainWidget::cullDrawEntity(VAOEntity *entity, QMatrix4x4 mvMatrix, QMatrix4x4 pMatrix, float transparent)
{
    for(int i=0; i<entity->getListInfo()->count(); ++i)
    {
        shaderProgram.bind();
        shaderProgram.setUniformValue("mvMatrix", mvMatrix);
        shaderProgram.setUniformValue("pMatrix", pMatrix);
//        shaderProgram.setUniformValue("v_color", QVector3D(0,1,0.5));
        shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
        shaderProgram.setUniformValue("u_lightPower", 1.0f);
        shaderProgram.setUniformValue("fragFlag", 1.0f);
        shaderProgram.setUniformValue("texturTransparency", transparent);
        entity->getListInfo()->at(i)->vao->bind();
        entity->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, entity->getListInfo()->at(i)->numVertex); // create List
        entity->getTextureImg()->release();
        entity->getListInfo()->at(i)->vao->release();

        shaderProgram.release();
    }

}
void MainWidget::cullDrawSkyBox(QMatrix4x4 mvMatrix, QMatrix4x4 pMatrix)
{
    shaderProgram.bind();
    shaderProgram.setUniformValue("mvMatrix", mvMatrix);
    shaderProgram.setUniformValue("pMatrix", pMatrix);
//    shaderProgram.setUniformValue("v_color", QVector3D(0,1,0.5));
    shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
    shaderProgram.setUniformValue("u_lightPower", 1.0f);
    shaderProgram.setUniformValue("fragFlag", 1.0f);
    shaderProgram.setUniformValue("texturTransparency", 1.0f);

    vaoSkyBox->getListInfo()->at(0)->vao->bind();
    vaoSkyBox->getTextureImg()->bind();
    glDrawArrays(GL_TRIANGLES, 0, vaoSkyBox->getListInfo()->at(0)->numVertex);
    vaoSkyBox->getTextureImg()->release();
    vaoSkyBox->getListInfo()->at(0)->vao->release();

    shaderProgram.release();
}

/// ================== Create Entity ================== ///
bool MainWidget::buildVBOEntity()
{
    // سبب عدم إدخالها في المودل هو لأنه سيتكرر إنشاء نفس العنصر مثل المنزل

    /// الترتيب مهم بين عناصر ملف بليندر وعناصر المنشئه بملف المودل
    /// يتطلب التعديل على ملف بليندر لترتيب العناصر
    /// أو تعديل ترتيب المنشئه وفقاً لترتيب البليندر

//    vaoSkyBox = new VAOEntity(&shaderProgram);

    // Erth
    vaoErth = new VAOEntity(&shaderProgram);
//    vaoErth->setPath(":/blender/FilesObject/objErth.obj");
//    vaoErth->setPath("/home/aloool14/Documents/blender/Erth/Erth100Cut11_2.obj");
    vaoErth->setPath(":/blender/FilesObject/Erth20x20.obj");
    vaoErth->setTextureImg(":/blender/FilesObject/texErth.png");

    // Home Good
    vaoHomeGood = new VAOEntity(&shaderProgram);
//    vaoHomeGood->setPath("/home/aloool14/Documents/blender/Homes/Build3.obj");
    vaoHomeGood->setPath(":/blender/FilesObject/Build.obj");
    vaoHomeGood->setTextureImg(":/blender/FilesObject/texHome.png");
    // Home Bad
//    vaoHomeBad = new VAOEntity(&shaderProgram);
//    vaoHomeBad->setPath(":/blender/FilesObject/objBuild.obj");
//    vaoHomeBad->setTextureImg(":/blender/FilesObject/texHome2.png");
    // Tree
//    vaoTree = new VAOEntity(&shaderProgram);
//    vaoTree->setPath("/home/aloool14/Documents/blender/Tree/birch_tree.obj");
//    vaoTree->setTextureImg("/home/aloool14/Documents/blender/Tree/textureTree.png");

    // Human
//    vaoHuman = new VAOEntity(&shaderProgram);
//    vaoHuman->setPath("/home/aloool14/Documents/blender/people/Human/Human8.obj");
//    vaoHuman->setTextureImg(":/blender/FilesObject/texHuman.png");

    // Plane
    vaoPlane = new VAOEntity(&shaderProgram);
    vaoPlane->setPath(":/blender/FilesObject/objPlaneShort.obj");
    vaoPlane->setTextureImg( ":/blender/FilesObject/texPlane.jpg");
    // Car
    vaoCar = new VAOEntity(&shaderProgram);
    vaoCar->setPath(":/blender/FilesObject/objCarPoly.obj");
    vaoCar->setTextureImg(":/blender/FilesObject/texCarPoly.png");

    // Gun
    vaoGun = new VAOEntity(&shaderProgram);
    vaoGun->setPath(":/blender/FilesObject/Gun1.obj");
    vaoGun->setTextureImg(":/blender/FilesObject/gun1.jpg");
    qDebug("LOOOOOL");
    // Gun Thrower
//    vaoGunThrower = new VAOEntity(&shaderProgram);
//    vaoGunThrower->setPath("/home/aloool14/Documents/blender/Gun/Thrower3.obj");
//    vaoGunThrower->setTextureImg("/home/aloool14/Documents/blender/Gun/texThrower2.png");
//     Rocket
    vaoRocket = new VAOEntity(&shaderProgram);
    vaoRocket->setPath(":/blender/FilesObject/objRocket.obj");
    vaoRocket->setTextureImg(":/blender/FilesObject/texRocket.png");
    // Bullet
    vaoBullet = new VAOEntity(&shaderProgram);
    vaoBullet->setPath(":/blender/FilesObject/Bullet.obj");
    vaoBullet->setTextureImg(":/blender/FilesObject/texRocket.png");
    // Smoke
//    vaoSmoke = new VAOEntity(&shaderProgram);
//    vaoSmoke->setPath("/home/aloool14/Documents/blender/Smoke/objSmoke2.obj");
//    vaoSmoke->setTextureImg("/home/aloool14/Documents/blender/Smoke/texSmoke1.png");
    // Point
//    vaoPoint = new VAOEntity(&shaderProgram);
//    vaoPoint->setPath("/home/aloool14/Documents/blender/Point/PointSmall2.obj");
//    vaoPoint->setTextureImg("/home/aloool14/Documents/blender/Point/texPoint.png");

    //Fire
    QVector<QVector3D> vecVertex, vecNormal;
    QVector<QVector2D> vecTexture;
    /// Full Vec
    {
        vecVertex   <<  QVector3D(-0.05, 0.05, -0.05)  <<  QVector3D(6e-05, 0.150404, -6e-05)
                    <<  QVector3D(0.05, 0.05, -0.05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(-0.05, -0.05, 0.05)  <<  QVector3D(0.05, -0.05, 0.05)
                    <<  QVector3D(-0.05, 0.05, 0.05)  <<  QVector3D(-0.05, -0.05, -0.05)
                    <<  QVector3D(-0.05, -0.05, 0.05)  <<  QVector3D(-0.05, -0.05, -0.05)
                    <<  QVector3D(-1.6e-05, -0.150033, 1.6e-05)  <<  QVector3D(-0.05, -0.05, 0.05)
                    <<  QVector3D(0.05, 0.05, -0.05)  <<  QVector3D(0.05, -0.05, 0.05)
                    <<  QVector3D(0.05, -0.05, -0.05)  <<  QVector3D(-0.05, 0.05, -0.05)
                    <<  QVector3D(0.05, -0.05, -0.05)  <<  QVector3D(-0.05, -0.05, -0.05)
                    <<  QVector3D(-6e-05, 0.150404, -6e-05)  <<  QVector3D(6e-05, 0.150404, 6e-05)
                    <<  QVector3D(6e-05, 0.150404, -6e-05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(-6e-05, 0.150404, 6e-05)  <<  QVector3D(-0.05, 0.05, 0.05)
                    <<  QVector3D(-0.05, 0.05, 0.05)  <<  QVector3D(-6e-05, 0.150404, -6e-05)
                    <<  QVector3D(-0.05, 0.05, -0.05)  <<  QVector3D(0.05, 0.05, -0.05)
                    <<  QVector3D(6e-05, 0.150404, 6e-05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(-1.6e-05, -0.150033, 1.6e-05)
                    <<  QVector3D(-1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(0.05, -0.05, 0.05)
                    <<  QVector3D(1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(0.05, -0.05, -0.05)
                    <<  QVector3D(0.05, -0.05, -0.05)  <<  QVector3D(-1.6e-05, -0.150033, -1.6e-05)
                    <<  QVector3D(-0.05, -0.05, -0.05)  <<  QVector3D(-0.05, -0.05, 0.05)
                    <<  QVector3D(1.6e-05, -0.150033, 1.6e-05)  <<  QVector3D(0.05, -0.05, 0.05)
                    <<  QVector3D(-0.05, 0.05, -0.05)  <<  QVector3D(-6e-05, 0.150404, -6e-05)
                    <<  QVector3D(6e-05, 0.150404, -6e-05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(-0.05, 0.05, 0.05)  <<  QVector3D(-0.05, -0.05, 0.05)
                    <<  QVector3D(-0.05, 0.05, 0.05)  <<  QVector3D(-0.05, 0.05, -0.05)
                    <<  QVector3D(-0.05, -0.05, -0.05)  <<  QVector3D(-0.05, -0.05, -0.05)
                    <<  QVector3D(-1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(-1.6e-05, -0.150033, 1.6e-05)
                    <<  QVector3D(0.05, 0.05, -0.05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(0.05, -0.05, 0.05)  <<  QVector3D(-0.05, 0.05, -0.05)
                    <<  QVector3D(0.05, 0.05, -0.05)  <<  QVector3D(0.05, -0.05, -0.05)
                    <<  QVector3D(-6e-05, 0.150404, -6e-05)  <<  QVector3D(-6e-05, 0.150404, 6e-05)
                    <<  QVector3D(6e-05, 0.150404, 6e-05)  <<  QVector3D(0.05, 0.05, 0.05)
                    <<  QVector3D(6e-05, 0.150404, 6e-05)  <<  QVector3D(-6e-05, 0.150404, 6e-05)
                    <<  QVector3D(-0.05, 0.05, 0.05)  <<  QVector3D(-6e-05, 0.150404, 6e-05)
                    <<  QVector3D(-6e-05, 0.150404, -6e-05)  <<  QVector3D(0.05, 0.05, -0.05)
                    <<  QVector3D(6e-05, 0.150404, -6e-05)  <<  QVector3D(6e-05, 0.150404, 6e-05)
                    <<  QVector3D(1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(1.6e-05, -0.150033, 1.6e-05)
                    <<  QVector3D(-1.6e-05, -0.150033, 1.6e-05)  <<  QVector3D(0.05, -0.05, 0.05)
                    <<  QVector3D(1.6e-05, -0.150033, 1.6e-05)  <<  QVector3D(1.6e-05, -0.150033, -1.6e-05)
                    <<  QVector3D(0.05, -0.05, -0.05)  <<  QVector3D(1.6e-05, -0.150033, -1.6e-05)
                    <<  QVector3D(-1.6e-05, -0.150033, -1.6e-05)  <<  QVector3D(-0.05, -0.05, 0.05)
                    <<  QVector3D(-1.6e-05, -0.150033, 1.6e-05)  <<  QVector3D(1.6e-05, -0.150033, 1.6e-05);

        vecNormal  <<  QVector3D(0, 0.4453, -0.8954)  <<  QVector3D(0, 0.4453, -0.8954)
                    <<  QVector3D(0, 0.4453, -0.8954)  <<  QVector3D(0, 0, 1)
                    <<  QVector3D(0, 0, 1)  <<  QVector3D(0, 0, 1)
                    <<  QVector3D(-1, 0, 0)  <<  QVector3D(-1, 0, 0)
                    <<  QVector3D(-1, 0, 0)  <<  QVector3D(-0.8945, -0.447, 0)
                    <<  QVector3D(-0.8945, -0.447, 0)  <<  QVector3D(-0.8945, -0.447, 0)
                    <<  QVector3D(1, 0, 0)  <<  QVector3D(1, 0, 0)
                    <<  QVector3D(1, 0, 0)  <<  QVector3D(0, 0, -1)
                    <<  QVector3D(0, 0, -1)  <<  QVector3D(0, 0, -1)
                    <<  QVector3D(0, 1, 0)  <<  QVector3D(0, 1, 0)
                    <<  QVector3D(0, 1, 0)  <<  QVector3D(0, 0.4453, 0.8954)
                    <<  QVector3D(0, 0.4453, 0.8954)  <<  QVector3D(0, 0.4453, 0.8954)
                    <<  QVector3D(-0.8954, 0.4453, 0)  <<  QVector3D(-0.8954, 0.4453, 0)
                    <<  QVector3D(-0.8954, 0.4453, 0)  <<  QVector3D(0.8954, 0.4453, 0)
                    <<  QVector3D(0.8954, 0.4453, 0)  <<  QVector3D(0.8954, 0.4453, 0)
                    <<  QVector3D(0, -1, 0)  <<  QVector3D(0, -1, 0)
                    <<  QVector3D(0, -1, 0)  <<  QVector3D(0.8945, -0.447, 0)
                    <<  QVector3D(0.8945, -0.447, 0)  <<  QVector3D(0.8945, -0.447, 0)
                    <<  QVector3D(0, -0.447, -0.8945)  <<  QVector3D(0, -0.447, -0.8945)
                    <<  QVector3D(0, -0.447, -0.8945)  <<  QVector3D(0, -0.447, 0.8945)
                    <<  QVector3D(0, -0.447, 0.8945)  <<  QVector3D(0, -0.447, 0.8945)
                    <<  QVector3D(0, 0.4454, -0.8954)  <<  QVector3D(0, 0.4454, -0.8954)
                    <<  QVector3D(0, 0.4454, -0.8954)  <<  QVector3D(0, 0, 1)
                    <<  QVector3D(0, 0, 1)  <<  QVector3D(0, 0, 1)
                    <<  QVector3D(-1, 0, 0)  <<  QVector3D(-1, 0, 0)
                    <<  QVector3D(-1, 0, 0)  <<  QVector3D(-0.8945, -0.447, 0)
                    <<  QVector3D(-0.8945, -0.447, 0)  <<  QVector3D(-0.8945, -0.447, 0)
                    <<  QVector3D(1, 0, 0)  <<  QVector3D(1, 0, 0)
                    <<  QVector3D(1, 0, 0)  <<  QVector3D(0, 0, -1)
                    <<  QVector3D(0, 0, -1)  <<  QVector3D(0, 0, -1)
                    <<  QVector3D(0, 1, 0)  <<  QVector3D(0, 1, 0)
                    <<  QVector3D(0, 1, 0)  <<  QVector3D(0, 0.4454, 0.8954)
                    <<  QVector3D(0, 0.4454, 0.8954)  <<  QVector3D(0, 0.4454, 0.8954)
                    <<  QVector3D(-0.8954, 0.4453, 0)  <<  QVector3D(-0.8954, 0.4453, 0)
                    <<  QVector3D(-0.8954, 0.4453, 0)  <<  QVector3D(0.8954, 0.4453, 0)
                    <<  QVector3D(0.8954, 0.4453, 0)  <<  QVector3D(0.8954, 0.4453, 0)
                    <<  QVector3D(0, -1, 0)  <<  QVector3D(0, -1, 0)
                    <<  QVector3D(0, -1, 0)  <<  QVector3D(0.8945, -0.447, 0)
                    <<  QVector3D(0.8945, -0.447, 0)  <<  QVector3D(0.8945, -0.447, 0)
                    <<  QVector3D(0, -0.4469, -0.8946)  <<  QVector3D(0, -0.4469, -0.8946)
                    <<  QVector3D(0, -0.4469, -0.8946)  <<  QVector3D(0, -0.4469, 0.8946)
                    <<  QVector3D(0, -0.4469, 0.8946)  <<  QVector3D(0, -0.4469, 0.8946);

        vecTexture  <<  QVector2D(0.250261, 0.992557)  <<  QVector2D(0.5, 0.992557)
                    <<  QVector2D(0.5, 0.992557)  <<  QVector2D(0.749739, 1.00596)
                    <<  QVector2D(0.999478, -0.005955)  <<  QVector2D(0.749739, -0.005955)
                    <<  QVector2D(0.000522, 1.00596)  <<  QVector2D(0.250261, -0.005955)
                    <<  QVector2D(0.000522, -0.005956)  <<  QVector2D(0.250261, 0.003506)
                    <<  QVector2D(0.000522, 0.003506)  <<  QVector2D(0.000522, 0.003506)
                    <<  QVector2D(0.5, 1.00596)  <<  QVector2D(0.749739, -0.005955)
                    <<  QVector2D(0.5, -0.005955)  <<  QVector2D(0.250261, 1.00596)
                    <<  QVector2D(0.5, -0.005955)  <<  QVector2D(0.250261, -0.005955)
                    <<  QVector2D(0.5, 0.995327)  <<  QVector2D(0.749739, 0.856722)
                    <<  QVector2D(0.5, 0.856722)  <<  QVector2D(0.749739, 0.994526)
                    <<  QVector2D(0.999478, 0.994526)  <<  QVector2D(0.999478, 0.994526)
                    <<  QVector2D(0.000522, 0.992557)  <<  QVector2D(0.250261, 0.992557)
                    <<  QVector2D(0.250261, 0.992557)  <<  QVector2D(0.5, 0.990589)
                    <<  QVector2D(0.749739, 0.990589)  <<  QVector2D(0.749739, 0.990589)
                    <<  QVector2D(0.5, 0.137372)  <<  QVector2D(0.749739, -0.001232)
                    <<  QVector2D(0.5, -0.001233)  <<  QVector2D(0.749739, 0.007443)
                    <<  QVector2D(0.5, 0.007443)  <<  QVector2D(0.5, 0.007443)
                    <<  QVector2D(0.5, 0.001537)  <<  QVector2D(0.250261, 0.001537)
                    <<  QVector2D(0.250261, 0.001537)  <<  QVector2D(0.999478, 0.003506)
                    <<  QVector2D(0.749739, 0.003506)  <<  QVector2D(0.749739, 0.003506)
                    <<  QVector2D(0.250261, 0.992557)  <<  QVector2D(0.250261, 0.992557)
                    <<  QVector2D(0.5, 0.992557)  <<  QVector2D(0.749739, 1.00596)
                    <<  QVector2D(0.999478, 1.00596)  <<  QVector2D(0.999478, -0.005955)
                    <<  QVector2D(0.000522, 1.00596)  <<  QVector2D(0.250261, 1.00596)
                    <<  QVector2D(0.250261, -0.005955)  <<  QVector2D(0.250261, 0.003506)
                    <<  QVector2D(0.250261, 0.003506)  <<  QVector2D(0.000522, 0.003506)
                    <<  QVector2D(0.5, 1.00596)  <<  QVector2D(0.749739, 1.00596)
                    <<  QVector2D(0.749739, -0.005955)  <<  QVector2D(0.250261, 1.00596)
                    <<  QVector2D(0.5, 1.00596)  <<  QVector2D(0.5, -0.005955)
                    <<  QVector2D(0.5, 0.995327)  <<  QVector2D(0.749739, 0.995327)
                    <<  QVector2D(0.749739, 0.856722)  <<  QVector2D(0.749739, 0.994526)
                    <<  QVector2D(0.749739, 0.994526)  <<  QVector2D(0.999478, 0.994526)
                    <<  QVector2D(0.000522, 0.992557)  <<  QVector2D(0.000522, 0.992557)
                    <<  QVector2D(0.250261, 0.992557)  <<  QVector2D(0.5, 0.990589)
                    <<  QVector2D(0.5, 0.990589)  <<  QVector2D(0.749739, 0.990589)
                    <<  QVector2D(0.5, 0.137372)  <<  QVector2D(0.749739, 0.137372)
                    <<  QVector2D(0.749739, -0.001232)  <<  QVector2D(0.749739, 0.007443)
                    <<  QVector2D(0.749739, 0.007443)  <<  QVector2D(0.5, 0.007443)
                    <<  QVector2D(0.5, 0.001537)  <<  QVector2D(0.5, 0.001537)
                    <<  QVector2D(0.250261, 0.001537)  <<  QVector2D(0.999478, 0.003506)
                    <<  QVector2D(0.999478, 0.003506)  <<  QVector2D(0.749739, 0.003506);
    }

    vaoFire = new VAOEntity(&shaderProgram);
    vaoFire->setPathVertex(vecVertex,vecNormal,vecTexture);
    vaoFire->setTextureImg(":/blender/FilesObject/texFire.png");

    return true;
}

void MainWidget::createErth()
{
    modelErth = new ModelErth(&shaderProgram, vaoErth);
    modelErth->setModelName("Erth");
    modelErth->setModelLimit(vaoErth->getLimited());
    modelErth->setModelPosition(QVector3D(0,0,0));
    modelErth->setModelRotate(QVector3D(0,0,0));
    modelErth->setModelScale(QVector3D(1,1,1));
//    modelErth->createEntity();
}
void MainWidget::createHome()
{
    float nums = 0;
    for(int i=0; i<3; ++i)
    {
        nums+=90; if(nums >= 360){ nums = 0;}
        //Pos
        float valueX = fmod( int(QTime::currentTime().msec()*random()) , modelErth->getModelLimited()->limitedBottom.x());
        float valueZ = fmod( int(QTime::currentTime().msec()*rand()) , modelErth->getModelLimited()->limitedBottom.z());
//        float valueY = entityErth->getTopY();
        float valueY = getUPs(QVector3D(valueX, 0, valueZ));

        ModelHome *entityHome = new ModelHome(&shaderProgram, vaoHomeGood);
        QString nm = "Home"+QString::number(i);
        entityHome->setModelName(nm);
        entityHome->setModelLimit(vaoHomeGood->getLimited());
        entityHome->setModelPosition(QVector3D(valueX, valueY, valueZ));
        entityHome->setModelRotate(QVector3D(0,90,0));
        entityHome->setModelScale(QVector3D(1,1,1));

        listEntityHomeGood->append(entityHome);

        Entity *entityTree = new Entity;
        entityTree->setPosition(QVector3D(valueX, valueY, valueZ));
        entityTree->setRotation(QVector3D(0,90,0));
//        entityTree->setLimited(vaoTree->getLimited());
//        entityTree->setVecTring(vaoTree->getListVecTriangle()->at(0));
//        entityTree->setLimitedParts(vaoTree->getLimitedPart2());
        listEntityTree->append(entityTree);
    }
}

float MainWidget::getUPs(QVector3D posHome)
{
    float spaceRng = modelErth->getInfo()->numSpaceRng;
    int numMaxX = modelErth->getInfo()->countMaxX;
    int numMaxZ = modelErth->getInfo()->countMaxZ;
    float posHX = posHome.x();
    float posHZ = posHome.z();
    int IDx=0;
    int IDz=0;

    if(posHX >= 0)
    {
        int px = floor(fabs(posHX)/spaceRng);
        IDx = numMaxX-(px+1);
    }
    else if (posHX < 0)
    {
        int px = ceil(fabs(posHX) / spaceRng);
        IDx = numMaxX+(px-1);
    }

    if(posHZ >= 0.0f)
    {
        int pz = floor(fabs(posHZ)/spaceRng);
        IDz = numMaxZ-(pz+1);
    }
    else if(posHZ < 0.0f)
    {
        int pz = ceil(fabs(posHZ) / spaceRng);
        IDz = numMaxZ+(pz-1);
    }

//    float Ups = *(*(entityErth->getInfo()->arry+IDx)+IDz); // (*(*arrayErth)[IDx])[IDz]
//    float Ups = entityErth->getInfo()->arry->at(IDx)->at(IDz);
//    float Ups = (*(*entityErth->getInfo()->arry)[IDx])[IDz];
//    qDebug()<<"UPs "<<Ups<<" | "<<IDx<<" "<<IDz;
//    qDebug()<<"Ref2: "<<entityErth->getInfo()->arry<<" | "<<(*entityErth->getInfo()->arry)[0]
//            <<" | "<<(*(*entityErth->getInfo()->arry)[302])[540];
    float Ups = modelErth->getInfo()->arry[IDx][IDz];
    return Ups;
}
void MainWidget::createHuman()
{

    modelRobot = new ModelRobot(&shaderProgram, vaoHuman);
    modelRobot->setModelPosition(QVector3D(0,0, 0));
    modelRobot->setModelRotate(QVector3D(0,180,0));
    modelRobot->createEntity();
    connect(modelRobot, &ModelRobot::emtUpdate, this, &MainWidget::slotUpdateHuman);
}
void MainWidget::createCar()
{
    modelCar = new ModelCar(&shaderProgram, vaoCar);
    modelCar->setModelName("Car");
    modelCar->setModelLimit(vaoCar->getLimited());
    modelCar->setModelPosition(QVector3D(0.0f,abs(modelErth->getModelLimited()->limitedTop.y()
                                              +modelCar->getModelLimited()->limitedBottom.y()),
                                         -1.0f));
    modelCar->setModelRotate(QVector3D(0.0f, 0.0f, 0.0f));
    modelCar->setModelScale(QVector3D(1,1,1));
    modelCar->createEntity();

    connect(modelCar, &ModelCar::emtUpdate, this, &MainWidget::slotUpdate);
}
void MainWidget::createPlane()
{
    modelPlane = new ModelPlane(&shaderProgram, vaoPlane);
    modelPlane->setModelName("Plane");
    modelPlane->setModelLimit(vaoPlane->getLimited());
//    modelPlane->setModelPosition(QVector3D(20,modelErth->getModelLimited()->limitedTop.y()
//                                           +fabs(modelPlane->getModelLimited()->limitedBottom.y()),
//                                           -2.0f));
    modelPlane->setModelPosition(QVector3D(1,1,-1));
    modelPlane->setModelRotate(QVector3D(0.0f,0.0f,0.0f));
    modelPlane->setModelScale(QVector3D(1,1,1));
    modelPlane->setDirection(QVector3D(0,0,0));
    modelPlane->createEntity();
    connect(modelPlane, &ModelPlane::emtUpdate, this, &MainWidget::slotUpdate);

}
void MainWidget::createGun()
{
    modelGun = new ModelGun(&shaderProgram, vaoGun);
    modelGun->setModelPosition(modelCollada->getModelPosition()+QVector3D(0, 2.0, 0)/*modelHuman->getPosThrower()*/);
    modelGun->setModelRotate(QVector3D(0,0,0));

    modelGun->setModelLimit(vaoGun->getLimited());
}
void MainWidget::createBullet(QVector3D posGun)
{
    /// Calc
    QVector3D dirBullet;
    float mode_x=0.0f, mode_y=0.0f;;
    {
        if(camStateZoom || modelCollada->checkLink())
        { mode_x = -fmod(camRotFreeX,360.0f); }
        else { mode_x = 0; }

        mode_y = modelCollada->getModelRotate().y();

        dirBullet = calcRotatePosition(QVector3D(0,0,-1), QVector3D(mode_x,mode_y,0));
    }

    ControllerRocket *conBullet = new ControllerRocket;
    conBullet->getEntityRocket()->setPosition(modelCollada->getModelPosition()/*modelHuman->getPosBullet()*/+QVector3D(0,2,0));
    conBullet->getEntityRocket()->setRotation(QVector3D(mode_x, mode_y, modelCollada->getModelRotate().z()));
    conBullet->getEntityRocket()->setDirection(dirBullet);
    conBullet->getEntityRocket()->setStartPos(modelCollada->getModelPosition()/*modelHuman->getPosBullet()*/+QVector3D(0,2,0), 0.5f, 100.0f, 1.0f);
    conBullet->getEntityRocket()->setLimited(vaoBullet->getLimited());
    conBullet->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    listControllerBullet->append(conBullet);

    connect(conBullet, &ControllerRocket::emtUpdate, this, &MainWidget::slotUpdate);
    connect(conBullet, &ControllerRocket::emtDeleteRocket, this, &MainWidget::slotDeleteBullet);
}

void MainWidget::createPoint()
{
    entityPoint = new Entity;
    entityPoint->setPosition(QVector3D(0.0f,1.5f,5.95f));
    entityPoint->setRotation(QVector3D(0.0f,0.0f,0.0f));
    entityPoint->setLimited(vaoPoint->getLimited());
}

void MainWidget::createWater()
{
    modelWater = new ModelWater(&shaderProgram, vaoGun); // Edit old vaoCar
    modelWater->setModelPosition(QVector3D(modelErth->getModelLimited()->limitedBottom.x(), 0,
                                           modelErth->getModelLimited()->limitedTop.z()));
    modelWater->createEntity();
}

void MainWidget::createThrower()
{
    modelGunThrower = new ModelGunThrower(&shaderProgram, vaoGunThrower);
    modelGunThrower->setModelPosition(modelCollada->getModelPosition()/*modelHuman->getPosThrower()*/);
//    modelGunThrower->setModelPosition(QVector3D(0, modelHuman->getListEntity().at(5)->getLimited()->limitedBottom.y(), 0));
    modelGunThrower->setModelRotate(QVector3D(0.0f, 0.0f, 0.0f));
    modelGunThrower->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    modelGunThrower->createEntity();

}
void MainWidget::createRocket(QVector3D posPlane)
{

    ControllerRocket *conRocket = new ControllerRocket;
    conRocket->getEntityRocket()->setPosition(modelPlane->getModelPosition());
    conRocket->getEntityRocket()->setRotation(modelPlane->getModelRotate());
    conRocket->getEntityRocket()->setDirection(modelPlane->getDirection());
    conRocket->getEntityRocket()->setStartPos(modelPlane->getModelPosition());
    conRocket->getEntityRocket()->setLimited(vaoRocket->getLimited());
//    conRocket->getEntityRocket()->setVecTring(vaoRocket->getListInfo()->at(0)->vecTringle);
    conRocket->getEntityRocket()->setEnableSmoke(true);
    conRocket->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    listControllerRocket->append(conRocket);

    connect(conRocket, &ControllerRocket::emtUpdate, this, &MainWidget::slotUpdate);
    connect(conRocket, &ControllerRocket::emtDeleteRocket, this, &MainWidget::slotDeleteRocket);
    qDebug("OPOPOPOPOPOO");

}
void MainWidget::createFire(QVector3D posFire)
{
    ControllerFire *controllerFire = new ControllerFire(posFire,3);
    connect(controllerFire, &ControllerFire::emtDeleterFire,this, &MainWidget::slotDeleteFire);
    connect(controllerFire, &ControllerFire::emtCreateFire ,this, &MainWidget::slotCreateFire);
    listControllerFire->append(controllerFire);
}

void MainWidget::loadImageSkyBox()
{
    QString ImgBox = "/home/aloool14/Pictures/Textures/texSkBx.jpg";
    mTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    {
        const QImage posx = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);
        const QImage negx = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);

        const QImage posy = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);
        const QImage negy = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);

        const QImage posz = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);
        const QImage negz = QImage(ImgBox).convertToFormat(QImage::Format_RGBA8888);

        mTexture->create();
        mTexture->setSize(posx.width(), posx.height(), posx.depth());
        mTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
        mTexture->allocateStorage();

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posx.constBits(), Q_NULLPTR);

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posy.constBits(), Q_NULLPTR);

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posz.constBits(), Q_NULLPTR);

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negx.constBits(), Q_NULLPTR);

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negy.constBits(), Q_NULLPTR);

        mTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negz.constBits(), Q_NULLPTR);

        mTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
        mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        mTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    }
}
void MainWidget::createSkyBox()
{

    loadImageSkyBox();
    float lenBox = 10.0f;
    QVector3D vb1 = QVector3D(-lenBox,  +lenBox/2, -lenBox);
    QVector3D vb2 = QVector3D(-lenBox,  0.1, -lenBox);
    QVector3D vb3 = QVector3D(+lenBox,  0.1, -lenBox);
    QVector3D vb4 = QVector3D(+lenBox,  +lenBox/2, -lenBox);

    QVector3D vt1 = QVector3D(+lenBox,  +lenBox/2, +lenBox);
    QVector3D vt2 = QVector3D(+lenBox,  0.1, +lenBox);
    QVector3D vt3 = QVector3D(-lenBox,  0.1, +lenBox);
    QVector3D vt4 = QVector3D(-lenBox,  +lenBox/2, +lenBox);

    QVector3D vertices[] =
    {
        // Face Back
        {-lenBox,  lenBox, -lenBox},
        {-lenBox, -lenBox, -lenBox},
        {+lenBox, -lenBox, -lenBox},

        {+lenBox, -lenBox, -lenBox},
        {+lenBox, +lenBox, -lenBox},
        {-lenBox, +lenBox, -lenBox},


        // Face Left
        {-lenBox, -lenBox, +lenBox},
        {-lenBox, -lenBox, -lenBox},
        {-lenBox, +lenBox, -lenBox},
        {-lenBox, +lenBox, -lenBox},
        {-lenBox, +lenBox, +lenBox},
        {-lenBox, -lenBox, +lenBox},

        {+lenBox, -lenBox, -lenBox},
        {+lenBox, -lenBox, +lenBox},
        {+lenBox, +lenBox, +lenBox},
        {+lenBox, +lenBox, +lenBox},
        {+lenBox, +lenBox, -lenBox},
        {+lenBox, -lenBox, -lenBox},

        {-lenBox, -lenBox, +lenBox},
        {-lenBox, +lenBox, +lenBox},
        {+lenBox, +lenBox, +lenBox},
        {+lenBox, +lenBox, +lenBox},
        {+lenBox, -lenBox, +lenBox},
        {-lenBox, -lenBox, +lenBox},

        {-lenBox, +lenBox, -lenBox},
        {+lenBox, +lenBox, -lenBox},
        {+lenBox, +lenBox, +lenBox},
        {+lenBox, +lenBox, +lenBox},
        {-lenBox, +lenBox, +lenBox},
        {-lenBox, +lenBox, -lenBox},

        {-lenBox, -lenBox, -lenBox},
        {-lenBox, -lenBox, +lenBox},
        {+lenBox, -lenBox, -lenBox},
        {+lenBox, -lenBox, -lenBox},
        {-lenBox, -lenBox, +lenBox},
        {+lenBox, -lenBox, +lenBox}
    };

    QVector2D textures[] =
     {
         {0.0f, 1.0f},
         {0.0f, 0.0f},
         {1.0f, 0.0f},

         {1.0f, 0.0f},
         {1.0f, 1.0f},
         {0.0f, 1.0f},

         {-1.0f, -1.0f},
         {-1.0f, -1.0f},
         {-1.0f, +1.0f},
         {-1.0f, +1.0f},
         {-1.0f, +1.0f},
         {-1.0f, -1.0f},

         {+1.0f, -1.0f},
         {+1.0f, -1.0f},
         {+1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, -1.0f},

         {-1.0f, -1.0f},
         {-1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, -1.0f},
         {-1.0f, -1.0f},

         {-1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, +1.0f},
         {+1.0f, +1.0f},
         {-1.0f, +1.0f},
         {-1.0f, +1.0f},

         {-1.0f, -1.0f},
         {-1.0f, -1.0f},
         {+1.0f, -1.0f},
         {+1.0f, -1.0f},
         {-1.0f, -1.0f},
         {+1.0f, -1.0f}
     };

    QVector<QVector3D> vecSkyBox;
    vecSkyBox
            // Right
            << vb3 << vt2 << vb4
            << vb4 << vt2 << vt1
            // BackZ
            << vb2 << vb3 << vb1
            << vb1 << vb3 << vb4
            // Left
            << vt3 << vb2 << vt4
            << vt4 << vb2 << vb1
            // Amam
            << vt2 << vt3 << vt1
            << vt1 << vt3 << vt4
            // Up
            << vt1 << vt4 << vb4
            << vb4 << vt4 << vb1
            // Down
/*            << vb3 << vb2 << vt2
            << vt2 << vb2 << vt3*/ ;


    QVector<QVector2D> texSkyBox;
    texSkyBox << QVector2D(0, 0) << QVector2D(0.25, 0) << QVector2D(0, 0.50)
              << QVector2D(0, 0.50) << QVector2D(0.25, 0) << QVector2D(0.25, 0.50)

              << QVector2D(0.25, 0) << QVector2D(0.50, 0) << QVector2D(0.25, 0.50)
              << QVector2D(0.25, 0.50) << QVector2D(0.50, 0) << QVector2D(0.50, 0.50)

              << QVector2D(0.50, 0) << QVector2D(0.75, 0) << QVector2D(0.50, 0.50)
              << QVector2D(0.50, 0.50) << QVector2D(0.75, 0) << QVector2D(0.75, 0.50)

              << QVector2D(0.75, 0) << QVector2D(1, 0) << QVector2D(0.75, 0.50)
              << QVector2D(0.75, 0.50) << QVector2D(1, 0) << QVector2D(1, 0.50)

              << QVector2D(0, 0.50) << QVector2D(1, 0.50) << QVector2D(0,1)
              << QVector2D(0, 1) << QVector2D(1, 0.50) << QVector2D(1, 1)
/*              << QVector2D(0,0) << QVector2D(1,0) << QVector2D(0,1)
              << QVector2D(0,1) << QVector2D(1,0) << QVector2D(1,1)*/;
//    texSkyBox << QVector2D(1, 0)
//              <<   QVector2D(0, 1)
//              <<   QVector2D(0, 0)
/*              <<   QVector2D(1, 0)
              <<   QVector2D(1, 1)
              <<   QVector2D(0, 1)*/;
//    foreach (QVector3D vecs, textures) {
//        texSkyBox.append(vecs);
//    }
    QVector<QVector3D> vecNormal;

    vaoSkyBox = new VAOEntity(&shaderProgram);
    vaoSkyBox->setPathVertex(vecSkyBox, vecNormal, texSkyBox);
    vaoSkyBox->setTextureImg("/home/aloool14/Pictures/Textures/texSkBx.jpg");
//    vaoSky = new QOpenGLVertexArrayObject;
//    vaoSky->create();
//    vaoSky->bind();

//    QOpenGLBuffer *mVertexBuf = new QOpenGLBuffer;
//    mVertexBuf->create();
//    mVertexBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
//    mVertexBuf->bind();
//    mVertexBuf->allocate(vertices, 36 * sizeof(QVector3D));
//    shaderProgram.bindAttributeLocation("a_vertex",0);
//    shaderProgram.enableAttributeArray(0);
//    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);
//    //
//    QOpenGLBuffer *mTextureBuf = new QOpenGLBuffer;
//    mTextureBuf->create();
//    mTextureBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
//    mTextureBuf->bind();
//    mTextureBuf->allocate(textures, 36 * sizeof(QVector2D));
//    shaderProgram.bindAttributeLocation("a_texcoord",2);
//    shaderProgram.enableAttributeArray(2);
//    shaderProgram.setAttributeBuffer(2, GL_FLOAT, 0, 2);

//    vaoSky->release();

}

/// ================== SLOT DELETE    ========================= ///
void MainWidget::slotUpdate()
{

    qDebug("UPOODATE");
    update();
}

void MainWidget::slotUpdateHuman()
{
    modelGun->setModelPosition(modelCollada->getModelPosition()+QVector3D(0, 2.0, 0)/*modelHuman->getPosThrower()*/);
    modelGun->setModelRotate(QVector3D(modelGun->getModelRotate().x(),
                                       modelCollada->getModelRotate().y(),
                                       modelGun->getModelRotate().z()));

//    modelGunThrower->setModelPosition(modelCollada->getModelPosition()/*modelHuman->getPosThrower()*/);
//    modelGunThrower->setModelRotate(modelCollada->getModelRotate());

//    update();
}
void MainWidget::slotDeleteRocket(ControllerRocket *thisRockets)
{
//    qDebug("Dele&R");
    createFire(thisRockets->getEntityRocket()->getPosition());
    listControllerRocket->removeOne(thisRockets);
    update();
}
void MainWidget::slotDeleteBullet(ControllerRocket *thisBullet)
{
    createFire(thisBullet->getEntityRocket()->getPosition());

    for(int i=0; i<listControllerBullet->count(); ++i)
    {
        ControllerRocket *e = listControllerBullet->at(i);
        listControllerBullet->removeAt(i);
        qDebug("R");
        /*delete*/ e->deleteLater();
//        e = 0;
    }
//    listControllerBullet->removeOne(thisBullet);

    update();
    // Here remove but Coll Or NotColl
//    ControllerRocket *cons = thisBullet->getContril();
//    cons->stopControl();
//    listConCollision.removeOne(cons);
//    cons->deleteLater();

//    thisBullet->deleteLater();
}
void MainWidget::slotCreateFire(EntityEmitterFire *thisFire)
{
    listEntityFireEmitter->append(thisFire);
//    update();
}
void MainWidget::slotDeleteFire(EntityEmitterFire *thisFire, ControllerFire *thi)
{
    listControllerFire->removeOne(thi);
    listEntityFireEmitter->removeOne(thisFire);
//    update();
}

/// ================== collision ======================== ///
void MainWidget::settingCollision()
{
    //Plane
    ControllerCollision *collisionPlane = new ControllerCollision;
//    connect(modelPlane, &ModelPlane::emtStatePlane, collisionPlane, &ControllerCollision::stateControl);
    connect(collisionPlane, &ControllerCollision::emtConCollPlaneErth, this, &MainWidget::slotCollPlaneErth);
    connect(collisionPlane, &ControllerCollision::emtConCollPlaneHome, this, &MainWidget::slotCollPlanetHome);
    connect(collisionPlane, &ControllerCollision::emtConCollPlaneCar, this, &MainWidget::slotCollPlaneCar);
    connect(collisionPlane, &ControllerCollision::emtConCollPlaneHuman, this, &MainWidget::slotCollPlaneHuman);
    collisionPlane->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    collisionPlane->startColPlane(3);
    modelPlane->setControl(collisionPlane);

    //Car
    ControllerCollision *collisionCar = new ControllerCollision;
//    connect(modelCar, &ModelCar::emtStateCar, collisionCar, &ControllerCollision::stateControl);
    connect(collisionCar, &ControllerCollision::emtConCollCarErth, this, &MainWidget::slotCollCarErth);
    connect(collisionCar, &ControllerCollision::emtConCollCarHome, this, &MainWidget::slotCollCarHome);
    connect(collisionCar, &ControllerCollision::emtConCollCarPlane, this, &MainWidget::slotCollCarPlane);
    connect(collisionCar, &ControllerCollision::emtConCollCarHuman, this, &MainWidget::slotCollCarHuman);
    collisionCar->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    collisionCar->startColCar(3);
    modelCar->setControl(collisionCar);
    
    //Human
//    ControllerCollision *collisionHuman = new ControllerCollision;
////    connect(modelHuman, &ModelHuman::emtStateHuman, collisionHuman, &ControllerCollision::stateControl);
//    connect(collisionHuman, &ControllerCollision::emtConCollHumanErth, this, &MainWidget::slotCollHumanErth);
//    connect(collisionHuman, &ControllerCollision::emtConCollHumanHome, this, &MainWidget::slotCollHumanHome);
//    connect(collisionHuman, &ControllerCollision::emtConCollHumanPlane, this, &MainWidget::slotCollHumanPlane);
//    connect(collisionHuman, &ControllerCollision::emtConCollHumanCar, this, &MainWidget::slotCollHumanCar);
//    collisionHuman->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
//    collisionHuman->startColHuman(3);
//    modelHuman->setControl(collisionHuman);

    //Collada
    ControllerCollision *collisionCollada = new ControllerCollision;
//    connect(modelHuman, &ModelHuman::emtStateHuman, collisionHuman, &ControllerCollision::stateControl);
    connect(collisionCollada, &ControllerCollision::emtConCollHumanErth, this, &MainWidget::slotCollHumanErth);
    connect(collisionCollada, &ControllerCollision::emtConCollHumanHome, this, &MainWidget::slotCollHumanHome);
    connect(collisionCollada, &ControllerCollision::emtConCollHumanPlane, this, &MainWidget::slotCollHumanPlane);
    connect(collisionCollada, &ControllerCollision::emtConCollHumanCar, this, &MainWidget::slotCollHumanCar);
    collisionCollada->allEntityCollision(modelErth, listEntityHomeGood, modelCar, modelPlane, modelRobot, modelCollada);
    collisionCollada->startColCollada(3);
    modelCollada->setControl(collisionCollada);

    qDebug("END SETTING COLLISION");
}

// Plane Collision
void MainWidget::slotCollPlaneErth(ModelPlane *p, Entity *e, ControllerCollision *c)
{
//    qDebug("ColP&E");
}
void MainWidget::slotCollPlanetHome(ModelPlane *p, Entity *h, ControllerCollision *c)
{
//    qDebug("ColP&H");
}
void MainWidget::slotCollPlaneCar(ModelPlane *p, ModelCar *car, ControllerCollision *c)
{
//    qDebug("ColP&C");
}
void MainWidget::slotCollPlaneHuman(ModelPlane *p, ModelRobot *human, ControllerCollision *c)
{
//    qDebug("Colp&Hu");
}

// Car Collision
void MainWidget::slotCollCarErth(ModelCar *c, Entity *e, ControllerCollision *co)
{
//    qDebug("ColC&E");
}
void MainWidget::slotCollCarHome(ModelCar *c, Entity *h, ControllerCollision *co)
{
//    qDebug("ColC&H");
//    modelCar->sendCollisionCar(0.0f);
//    modelCar->setTimerCar();
}
void MainWidget::slotCollCarPlane(ModelCar *c, ModelPlane *plane, ControllerCollision *co)
{
//    qDebug("ColC&P");
}
void MainWidget::slotCollCarHuman(ModelCar *c, ModelRobot *human, ControllerCollision *co)
{
//    qDebug("ColC&Hu");
}

// Human Collision
void MainWidget::slotCollHumanErth(ModelRobot *h, Entity *e, ControllerCollision *c)
{
//    qDebug("ColH&E");
}
void MainWidget::slotCollHumanHome(ModelRobot *h, Entity *home, ControllerCollision *c)
{
//    qDebug("ColH&H");
}
void MainWidget::slotCollHumanPlane(ModelRobot *h, ModelPlane *plane, ControllerCollision *c)
{
//    qDebug("ColH&P");
}
void MainWidget::slotCollHumanCar(ModelRobot *h, ModelCar *car, ControllerCollision *c)
{
//    qDebug("ColH&C");
}

/// ================== GamePad Controller ================== ///

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_0)
    {
        qDebug("0000");
    } else if (event->key() == Qt::Key_5)
    {
        setupGamePad();
//        if (managGamePS.isEmpty())
//        {
//            QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));
//            managGamePS = QGamepadManager::instance()->connectedGamepads();
//            if (managGamePS.isEmpty())
//            {
//                qDebug() << "Did not find any connected gamepads";
//        //        return;
//            } else {
//                qDebug("Connected Ok");
//            }
//        } else {
//            qDebug("isConnected True");
//        }
    }
}

void MainWidget::setupGamePad()
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));
    auto managGamePS = QGamepadManager::instance()->connectedGamepads();
    if (managGamePS.isEmpty())
    {
        qDebug() << "Did not find any connected gamepads";
        return;
    } else { qDebug("Countune"); }

//    m_gamePS->deleteLater();
    m_gamePS = new QGamepad(*managGamePS.begin(), this);
    connect(m_gamePS, &QGamepad::axisRightXChanged, this, &MainWidget::emtAxisRightX);
    connect(m_gamePS, &QGamepad::axisRightYChanged, this, &MainWidget::emtAxisRightY);
    connect(m_gamePS, &QGamepad::axisLeftXChanged, this, &MainWidget::emtAxisLeftX);
    connect(m_gamePS, &QGamepad::axisLeftYChanged, this, &MainWidget::emtAxisLeftY);
    connect(m_gamePS, &QGamepad::buttonAChanged, this, &MainWidget::emt_A);
    connect(m_gamePS, &QGamepad::buttonBChanged, this, &MainWidget::emt_B);
    connect(m_gamePS, &QGamepad::buttonXChanged, this, &MainWidget::emt_X);
    connect(m_gamePS, &QGamepad::buttonYChanged, this, &MainWidget::emt_Y);
    connect(m_gamePS, &QGamepad::buttonUpChanged, this, &MainWidget::emt_Up);
    connect(m_gamePS, &QGamepad::buttonDownChanged, this, &MainWidget::emt_Down);
    connect(m_gamePS, &QGamepad::buttonL1Changed, this, &MainWidget::emt_L1);
    connect(m_gamePS, &QGamepad::buttonL2Changed, this, &MainWidget::emt_L2);
    connect(m_gamePS, &QGamepad::buttonR1Changed, this, &MainWidget::emt_R1);
    connect(m_gamePS, &QGamepad::buttonR2Changed, this, &MainWidget::emt_R2);
    connect(m_gamePS, &QGamepad::buttonR3Changed, this, &MainWidget::emt_R3);
    connect(m_gamePS, &QGamepad::buttonRightChanged, this, &MainWidget::emt_Right);
    connect(m_gamePS, &QGamepad::buttonLeftChanged, this, &MainWidget::emt_Left);
}

void MainWidget::emtAxisRightX(double v)
{
    if(v<-0.1f || v>0.1f)
    {
        if(v > 0.4f || v < -0.4f)
        { m_axisRightX=v*2.5f; }
        else
        { m_axisRightX=v*1.5f; }

        if(showBack)
        {
        }
        else if(controlsPlane)
        {
            modelPlane->setAxisRightX(v);
        }
        else
        {
            if(checkingModelNow == "non")
            {
                if(modelCollada->checkLink())
                {
                    modelCollada->setAxisRotLink(v);
//                    modelCollada->setModelRotate(modelCollada->getModelRotate()+QVector3D(0,m_axisRightX/2,0));
//                    modelCollada->setModelRotate(QVector3D(modelCollada->getModelRotate().x(),
//                                                         modelCollada->getModelRotate().y()+m_axisRightX/2,
//                                                         modelCollada->getModelRotate().z()));
                }
                else
                {
                    modelCollada->setAxisRightX(v);
                }
            }
            else if(checkingModelNow == "car")
            {
                qDebug("Axis Car R");
                modelCar->setAxisRightX(v);
            }
            else if (checkingModelNow == "plane")
            {
                modelPlane->setAxisRightX(v);
            }
//            update();
        }
    }
    else
    {
        if(m_axisRightX != 0.000f)
        {
            m_axisRightX=0.0f;
            if(showBack)
            {
            }
            else if(controlsPlane)
            {
                modelPlane->setAxisRightX(m_axisRightX);//
            }
            else
            {
                if(checkingModelNow == "non")
                {
                    if(modelCollada->checkLink())
                    {
                        modelCollada->setAxisRotLink(m_axisRightX);
                    }
                    else
                    {
                        modelCollada->setAxisRightX(m_axisRightX);
                    }
                }
                else if(checkingModelNow == "plane")
                {
                    modelPlane->setAxisRightX(m_axisRightX);
                }
                else if(checkingModelNow == "car")
                {
                    modelCar->setAxisRightX(m_axisRightX);
                }
            }
        }
    }
}

void MainWidget::emtAxisRightY(double v)
{
    if(!modelCollada->checkLink())
        modelCollada->setAxisRightY(v);
}

void MainWidget::emtAxisLeftX(double v)
{
    if(v<-0.1f || v>0.1f)
    {
        m_axisLeftX = v;
        modelCollada->setAxisLeftX(v);// rotCam
    }
    else
    {
        m_axisLeftX = 0.0f;
    }
}

void MainWidget::emtAxisLeftY(double v)
{
    if(v<-0.1f || v>0.1f)
    {
        m_axisLeftY = v;

        if(showBack)
        {
        }
        else if(controlsPlane)
        {
            showBackLink = false;
            modelPlane->setAxisLeft(v);
        }
        else
        {

            if(checkingModelNow == "non")
            {
                if(modelCollada->checkLink())
                {
                    showBackLink = true;
                }
                else
                {
                    showBackLink = false;
                }

            }
            else if(checkingModelNow == "plane")
            {
//                modelPlane->setAxisLeft(v);
                showBackLink = false;
            }
            else if(checkingModelNow == "car")
            {
                showBackLink = false;
            }

        }

    }
    else
    {
        if(m_axisLeftY != 0.0f)
        {
            m_axisLeftY = 0.0f;
            modelPlane->setAxisLeft(m_axisLeftY);

            if(!modelCollada->checkLink())
            {
            }
            else {
//                modelHuman->seta
            }

        }
    }


}

void MainWidget::emt_Up(bool v)
{
    if(v)
    {
        modelPlane->speedUP();
    }
}
void MainWidget::emt_Down(bool v)
{
    if(v)
    {
        if(showBack)
        {
        }
        else
        {
            modelPlane->speedDown();
        }
    }
}

void MainWidget::emt_B(bool v)
{ // Circle
    // يجب وضع مؤقت بين الطلقات حتى لا يحمّل على دالة الرسم ويحصل ثقل في الحركة
    if(v)
    {
        createRocket(modelPlane->getModelPosition());
//        createBullet(modelPlane->getModelPosition());
    }
}

void MainWidget::emt_X(bool v)
{// Triangle
    //
    if(v)
    {
        if(checkingModelNow == "non")
        {
            QVector2D vectorAC = QVector2D( modelCollada->getModelPosition().x()-modelCar->getModelPosition().x(),
                                            modelCollada->getModelPosition().z()-modelCar->getModelPosition().z());
            if(abs(vectorAC.x()) < 1.5f && abs(vectorAC.y()) < 1.5f)
            {
                checkingModelNow = "car";
                modelCollada->setOnTransport(true);
                return;
            }

            QVector2D vectorAC2 = QVector2D( modelCollada->getModelPosition().x()-modelPlane->getModelPosition().x(),
                                             modelCollada->getModelPosition().z()-modelPlane->getModelPosition().z());
            if(abs(vectorAC2.x()) < 1.5f && abs(vectorAC2.y()) < 1.5f)
            {
                checkingModelNow = "plane";
                modelCollada->setOnTransport(true);
                return;
            }

        } else {
            if(checkingModelNow=="car")
            {
                QVector3D posDown = QVector3D(modelCar->getModelLimited()->limitedBottom.x()-1, 0, 0);
                float lenthPos = sqrt( pow(posDown.x(),2) + pow(posDown.z(),2) );
                float angelPos = qRadiansToDegrees( atan2(posDown.x(), posDown.z()) ) ;

                float angelCar = modelCar->getModelRotate().y();
                float doubleAngel = angelPos + angelCar;

                QVector3D newPosDown = QVector3D(lenthPos* sin(qDegreesToRadians(doubleAngel)),
                                              0,
                                              lenthPos* cos(qDegreesToRadians(doubleAngel)));

                modelCollada->setModelPosition(modelCar->getModelPosition() + newPosDown);
//                modelCollada->setModelRotate(QVector3D(0, modelCar->getModelRotate().y(), 0));

            }
            else if(checkingModelNow=="plane")
            {
                QVector3D posDown = QVector3D(modelPlane->getModelLimited()->limitedBottom.x()-1, 0, 0);

                float lenthPos = sqrt( pow(posDown.x(),2) + pow(posDown.z(),2) );
                float angelPos = qRadiansToDegrees( atan2(posDown.x(), posDown.z()) ) ;
                float angelPlane = modelPlane->getModelRotate().y();
                float doubleAngel = angelPos + angelPlane;

                QVector3D newPosDown = QVector3D(lenthPos* sin(qDegreesToRadians(doubleAngel)),
                                              0,
                                              lenthPos* cos(qDegreesToRadians(doubleAngel)));

                modelCollada->setModelPosition(modelPlane->getModelPosition() + newPosDown);
//                modelCollada->setModelRotate(QVector3D(0, modelPlane->getModelRotate().y(), 0));
            }

            checkingModelNow = "non";
            modelCollada->setOnTransport(false);
        }
    }
}

void MainWidget::emt_L1(bool v)
{
    camStateZoom = v;
    if(v)
    {
//        modelRobot->animitionGunZoom(v);
//        modelRobot->setEnableZoom(v);

//        camPosTran = QVector3D(-0.2f, -1.45f, 0.0f);
        camPosTran = /*modelCollada->getModelPosition()+*/QVector3D(0.0f, -2.0f, 0.4f);
//        entityPoint->setPosition(-camPosTran+QVector3D(0,0,-0.05f));

        if(!timerRotZoom->isActive())
        {
            timerRotZoom->start(10);
        }
    }
    else
    {
//        modelRobot->animitionGunZoom(v);
//        modelRobot->setEnableZoom(v);

        camPosTran = QVector3D(0.0f, -1.5f, -6.0f);
//        entityPoint->setPosition(-camPosTran+QVector3D(0,0,-0.05f));

        if(timerRotZoom->isActive())
        {
            timerRotZoom->stop();
        }
    }
}
void MainWidget::emt_L2(bool v)
{
    showBack = v;
    if(v)
    {
        if(!timerRotCamer->isActive())
        {
            timerRotCamer->start(10);
        }
    }
    else
    {
        if(timerRotCamer->isActive())
        {
            timerRotCamer->stop();
        }
        update();
    }

}

void MainWidget::emt_R1(bool v)
{
    if(v)
    {
        if(checkingGunNow == "Gun")
        {
            createBullet(modelCollada->getModelPosition());
            if(!timerCreateBullet->isActive())
            {
                timerCreateBullet->start(100);
            }

        }
        else if(checkingGunNow == "Thrower")
        {
            /// Calc
            QVector3D dirWedge, posWedge;
            float mode_x=0.0f, mode_y=0.0f;
            {
                if(showBack || camStateZoom)
                {
                    mode_x = -fmod(camRotFreeX,360.0f);
                    mode_y = -fmod(camRotFreeY,360.0f);
                }
                else
                {
                    mode_x = 0.0f;
                    mode_y = 0.0f;
                }

                dirWedge = calcRotatePosition(QVector3D(0,0,-1), QVector3D(mode_x,modelCollada->getModelRotate().y(),0));
//                posWedge = calcRotatePosition(, )
            }
            modelGunThrower->shutThrower(modelCollada->getModelPosition()/*getPosThrower()*/,
                                         QVector3D(mode_x,modelCollada->getModelRotate().y(),0), dirWedge);
        }
    }
    else
    {
        if(checkingGunNow == "Gun")
        {
            if(timerCreateBullet->isActive())
            {
                timerCreateBullet->stop();
            }
        }
    }
}
void MainWidget::emt_R2(bool v)
{
    if(!v)
    {
        //modelPlane->closeWheels();
        ///=======================///

        controlsPlane=false;
        if(!modelCollada->checkLink())
        {
//            modelRobot->setState_StandSid();
        }

//        if(modelRobot->stateHuman()=="SitDown")
//        {
//            modelRobot->animitionStandUp();
//        }
//        else if(modelRobot->stateHuman()=="StandUp")
//        {
//            modelRobot->animitionSitDown();
//        }
    } else {
        controlsPlane=true;
    }
}
void MainWidget::emt_R3(bool v)
{
    if(v)
    {
        modelCollada->setHumanDown();
    }
}
void MainWidget::emt_A(bool v)
{// X

    if(v)
    {
        if(showBack) {
            modelCar->speedDown();
        } else {
            modelCar->speedUp();
        }

    }
}
void MainWidget::emt_Y(bool v)
{//Quarter
    if(v)
    {
        if(checkingModelNow == "car"){
            modelCar->speedDown();
        } else if( checkingModelNow == "non") {
            if(!modelCollada->checkLink())
            {
                modelCollada->jump();
            }
        }

    }
}
void MainWidget::emt_Right(bool v)
{
    if(v)
    {
        if(checkingGunNow == "non")
        {
            checkingGunNow = "Gun";
        }
        else if(checkingGunNow == "Gun")
        {
            checkingGunNow = "Thrower";
        }
        else if(checkingGunNow == "Thrower")
        {
            checkingGunNow = "non";
        }
    }
}
void MainWidget::emt_Left(bool v)
{
    if(v)
    {
        if(checkingGunNow == "non")
        {
            checkingGunNow = "Thrower";
        }
        else if(checkingGunNow == "Thrower")
        {
            checkingGunNow = "Gun";
        }
        else if(checkingGunNow == "Gun")
        {
            checkingGunNow = "non";
        }
    }
}
void MainWidget::getValModel()
{
    if(checkingModelNow == "car")
    {
        qDebug("-*-*-*-");
        qDebug()<<"C: "<<modelCar->getModelRotate();
        qDebug()<<"H: "<<modelCollada->getModelRotate();
        modelCollada->setModelPosition(modelCar->getModelPosition());
//        modelCollada->setModelRotate(modelCar->getModelRotate());
//        modelCollada->setModelRotate( QVector3D( modelCollada->getModelRotate().x(),
//                                                 modelCar->getModelRotate().y(),
//                                                 modelCollada->getModelRotate().z()));
        camPos = modelCar->getModelPosition();
        camRot = modelCar->getModelRotate();

        qDebug("-0-0-0-");
        qDebug()<<"xC: "<<modelCar->getModelRotate();
        qDebug()<<"xH: "<<modelCollada->getModelRotate();

    } else if(checkingModelNow == "plane")
    {
        modelCollada->setModelPosition(modelPlane->getModelPosition());
//        modelCollada->setModelRotate(modelPlane->getModelRotate());

        modelCollada->setModelRotate( QVector3D( modelCollada->getModelRotate().x(),
                                                 modelPlane->getModelRotate().y(),
                                                 modelCollada->getModelRotate().z()));
        camPos = modelPlane->getModelPosition();
        camRot = modelPlane->getModelRotate();
    } else if(checkingModelNow == "non")
    {
        if(camStateZoom)
        {
            QVector3D posCamGun;
            if(checkingGunNow == "Thrower")
            {
                posCamGun = calcRotatePosition(QVector3D(0.0f, modelGunThrower->getListEntity().at(0)->getLimited()->limitedTop.y()+0.05f, 0.0f),
                                               modelGunThrower->getModelRotate());
            }
            else if(checkingGunNow == "Gun")
            {
//                posCamGun = calcRotatePosition(QVector3D(0.0f, entityGun->getLimited()->limitedTop.y()+0.05f, 0.0f),
//                                               entityGun->getRotation());
                posCamGun = calcRotatePosition(QVector3D(0.0f, modelGun->getModelLimited()->limitedTop.y()+0.05f, 0.0f),
                                               modelGun->getModelRotate());
            }
            else
            {

            }

            camPos = modelCollada->getModelPosition()/*modelHuman->getPosThrower()*/ + posCamGun;
            camRot = modelCollada->getModelRotate();
        }
        else
        {
            camPos = modelCollada->getModelPosition();
            camRot = modelCollada->getModelRotate() + QVector3D(90,0,0);
        }


    }
}

void MainWidget::setStatePlayers(MainWidget::Players p)
{
    //
    if(p.testFlag(MainWidget::Human))
    {
//        qDebug("Human");
        camPos = modelRobot->getModelPosition();
        camRot = modelRobot->getModelRotate();
    } else if(p.testFlag(MainWidget::Car))
    {
//        qDebug("Car");
        camPos = modelCar->getModelPosition();
        camRot = modelCar->getModelRotate();
    } else if(p.testFlag(MainWidget::Plane))
    {
//        qDebug("Plane");
        camPos = modelPlane->getModelPosition();
        camRot = modelPlane->getModelRotate();
    }
}

/// ================== Camera  ========================= ///

void MainWidget::rotationCamera()
{
        if(!modelCollada->checkLink())
        {
            camRotFreeY-= (m_axisRightX/10);
        }
        //        update();
}

void MainWidget::rotationZoom()
{

    if(camRotFreeX<=90.0f && camRotFreeX>=-90.0f)
    {
        camRotFreeX += (m_axisLeftY);
        if(camRotFreeX<=90.0f && camRotFreeX>=-90.0f)
        {
        }
        else
        {
            camRotFreeX -= (m_axisLeftY);
        }
    }
        if(checkingGunNow == "Thrower")
        {
            modelGunThrower->setRotUp(-camRotFreeX);
        }
        else if(checkingGunNow == "Gun")
        {
//            entityGun->setRotation(QVector3D(-camRotFreeX, entityGun->getRotation().y(),entityGun->getRotation().z()));
            modelGun->setModelRotate(QVector3D(-camRotFreeX, modelGun->getModelRotate().y(),modelGun->getModelRotate().z()));
        }

        if(camStateZoom)
        {
            modelCollada->setAxisLeftX(m_axisLeftX/5);
        }

}

void MainWidget::slotCreateBullet()
{
    createBullet(modelCollada->getModelPosition());
}


QVector3D MainWidget::calcRotatePosition(QVector3D pos, QVector3D rots)
{
    QVector3D posPoint = pos;
    QVector3D rotEntity = rots;

    /// ============== STEP 1 ============= ///
    /// X
    float lenthVec = sqrt( pow(posPoint.y(),2) + pow(posPoint.z(),2) );
    float angelVec = qRadiansToDegrees( atan2(posPoint.z(), posPoint.y()) ) ;
    float doubleRotVec = angelVec+rotEntity.x();
    QVector3D posEntityNew = QVector3D(posPoint.x(),
                                       lenthVec* cos(qDegreesToRadians(doubleRotVec)),
                                       lenthVec* sin(qDegreesToRadians(doubleRotVec)));

    /// Z
    float lenthVec3 = sqrt( pow(posEntityNew.x(),2) + pow(posEntityNew.y(),2) );
    float angelVec3 = qRadiansToDegrees( atan2(posEntityNew.y(), posEntityNew.x()) ) ; // True y->x :: Erro x->y
    float doubleRotVec3 = angelVec3+rotEntity.z();
    QVector3D posEntityNew2 = QVector3D(lenthVec3* cos(qDegreesToRadians(doubleRotVec3)),
                                        lenthVec3* sin(qDegreesToRadians(doubleRotVec3)),
                                        posEntityNew.z());

    /// y
    float lenthVec2 = sqrt( pow(posEntityNew2.x(),2) + pow(posEntityNew2.z(),2) );
    float angelVec2 = qRadiansToDegrees( atan2(posEntityNew2.x(), posEntityNew2.z()) ) ;
    float doubleRotVec2 = angelVec2+rotEntity.y();
    QVector3D posEntityNew3 = QVector3D(lenthVec2* sin(qDegreesToRadians(doubleRotVec2)),
                                        posEntityNew2.y(),
                                        lenthVec2* cos(qDegreesToRadians(doubleRotVec2)));

    return posEntityNew3;
}


void MainWidget::createCollada()
{
    float sz = 1.8f;
    modelCollada = new ModelHumanH(&shaderCollada);
    modelCollada->setModelName("Human Collada");
    modelCollada->setModelPosition(QVector3D(2,0,8));
    modelCollada->setModelRotate(QVector3D(-90,0,0));
    modelCollada->setModelScale(QVector3D(sz,sz,sz));
//    modelCollada->setModelLimit();
    if(modelCollada->createEntity())
    {
        qDebug("GOOD LOAD Assimp ALL");
        connect(modelCollada, &ModelHumanH::emtUpdate, this, &MainWidget::slotUpdateHuman);
    }
}
