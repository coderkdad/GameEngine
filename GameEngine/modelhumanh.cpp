#include "modelhumanh.h"
#include "vaocolladaassimp.h"
#include "qmath.h"
#include <QTimer>
#include <controllercollision.h>

#define SPEED_T_HUMAN 30.0f // 30 تداخل عمليات الكشف عن التصادم
#define SPEED_T_ANIMI1 20.0f
#define SPEED_T_ANIMI2 40.0f

// State Human
#define STATE_RUN 0
#define STATE_STANDUP 1
#define STATE_SEDOWN 2
#define STATE_RUNDOWN 3
#define STATE_JUMPUP 4



ModelHumanH::ModelHumanH(QOpenGLShaderProgram *shader, QObject *parent) :
    QObject(parent),
    m_shader(shader)
{
    timerHumanGo = new QTimer;
    connect(timerHumanGo, &QTimer::timeout, this, &ModelHumanH::moveHuman);

//    connect(this, &ModelColladaH::emtStopTimerMove,this, &ModelColladaH::slotStopTimerMove);
//    connect(this, &ModelColladaH::emtCollision,this, &ModelColladaH::slotCollision);
//    connect(this, &ModelColladaH::emtNotCollision,this, &ModelColladaH::slotNotCollision);

    timerJump = new QTimer;
    connect(timerJump, &QTimer::timeout, this, &ModelHumanH::slotTimerJump);

    limFull = new QCubeLimited;
    setModelLimit(limFull);
    settingLimited();

}

bool ModelHumanH::createEntity()
{
    m_vaoAssimp = new VAOColladaAssimp(m_shader);
    m_vaoAssimp->setPath("/home/kdadmon/Documents/MyProjectShare/EngineGame/exBuffer_105/example/FilesObject/testCubes_522.dae"); // Problem import from Resources
    m_vaoAssimp->setTexture(":/blender/FilesObject/texHuman.png");

    return true;
}

void ModelHumanH::setModelPosition(QVector3D pos)
{
    BaseModel::setModelPosition(pos);
    emit emtUpdate();
}

void ModelHumanH::setModelRotate(QVector3D rot)
{
    BaseModel::setModelRotate(rot);
    emit emtUpdate();
}

void ModelHumanH::setDirection(QVector3D dir)
{
    m_dir = dir;
}

QVector3D ModelHumanH::getDirection()
{
    return m_dir;
}

void ModelHumanH::settingLimited()
{
    if(mStateHumanDown)
    {
        limFull->limitedTop = QVector3D(0.5f, 1.6f, 0.3f);
        limFull->limitedBottom = QVector3D(-0.5f, 0.0f, -0.6f);
    }
    else
    {
        limFull->limitedTop = QVector3D(0.5f, 2.1f, 0.2f);
        limFull->limitedBottom = QVector3D(-0.5f, 0.0f, -0.4f);
    }
}

void ModelHumanH::setControl(ControllerCollision *controlCol)
{
    m_controlCol=controlCol;
    connect(controlCol, &ControllerCollision::emtCollisionCollada, this, &ModelHumanH::slotCollisionCollada);
    connect(controlCol, &ControllerCollision::emtNotCollisionCollada, this, &ModelHumanH::slotNotCollisionCollada);
}

void ModelHumanH::draw(QMatrix4x4 matrixProCam, QMatrix4x4 matrixCam)
{

    {
        //    animator->update();
        ///==================///
        // من المهم عند تغيير النموذج تغيير الاتي
        // المسارات للكولادا والتكستر
        // حجم الماتريكس التي تحمل المفاصل لتكون على عددها
        // هذه الماتريكس في دالة الرسم وملف الشيدر
        // تغيير حجم القفز بين بيانات الفيس حسب البيانات الموجودة
        // وذلك في دالة صنع المثلثات
        ///==================///
        //
        //  تتبع أسماء المفاصل
        //  في دالة pushLibAnimation
        //
        // عدد الأوفسيت التي قد يكون في الكولادا لون أو لا يكون
        // في دالة readAllFaces
        //
        //
        //
        //==================///

    }

    QVector<QMatrix4x4*> *vectorMatrix(m_vaoAssimp->getPose());

    uint numJointt = vectorMatrix->count();

    QMatrix4x4 arrayMatrixJoints[numJointt];

    for(uint i=0; i<numJointt; ++i)
    {
        arrayMatrixJoints[i].setColumn(0, vectorMatrix->at(i)->row(0));
        arrayMatrixJoints[i].setColumn(1, vectorMatrix->at(i)->row(1));
        arrayMatrixJoints[i].setColumn(2, vectorMatrix->at(i)->row(2));
        arrayMatrixJoints[i].setColumn(3, vectorMatrix->at(i)->row(3));
    }

    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(getModelPosition());
    matrixModel.rotate(getModelRotate().y(), 0,1,0);
    matrixModel.rotate(getModelRotate().z(), 0,0,1);
    matrixModel.rotate(getModelRotate().x(), 1,0,0);
    matrixModel.scale(getModelScale());

    m_shader->bind();
    m_shader->setUniformValue("mvMatrix", (matrixModel));
    m_shader->setUniformValue("pMatrix", matrixProCam);
    m_shader->setUniformValueArray("jointTransforms", arrayMatrixJoints, numJointt);
    m_shader->setUniformValue("fragColor", 1.0f);
    m_shader->setUniformValue("texturTransparency", 1.0f);

    for(int i=0; i<m_vaoAssimp->getVAOS().count(); ++i)
    {
        m_vaoAssimp->getVAOS().at(i)->vao->bind();
        m_vaoAssimp->getTexture()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vaoAssimp->getVAOS().at(i)->numVertices);
        m_vaoAssimp->getTexture()->release();
        m_vaoAssimp->getVAOS().at(i)->vao->release();
    }

    m_shader->release();

}

void ModelHumanH::setHumanDown()
{
    if(mStateHumanDown)
    {
        mStateHumanDown=false;
        setStateHumanGo(false, STATE_STANDUP);
    }
    else
    {
        mStateHumanDown=true;
        setStateHumanGo(false, STATE_SEDOWN);
    }


}

void ModelHumanH::setAxisRightX(double axis)
{
    m_AxisRightX = axis;

    if(axis > 0.4f || axis < -0.4f)
    {
        m_AxisRightXEdit = axis*2.5f;
    }
    else if(axis > 0.1f || axis < -0.1f)
    {
        m_AxisRightXEdit = axis*1.5f;
    }
    else
    {
        m_AxisRightXEdit = 0.0f;
    }
}
void ModelHumanH::setAxisRightY(double axis)
{
    m_AxisRightY = axis;
//    qDebug()<<"Axis: "<<axis;

    axisAll();
}
void ModelHumanH::setAxisLeftX(double axis)
{

}
void ModelHumanH::setAxisLeftY(double axis)
{

}

void ModelHumanH::axisAll()
{
    float lenF = sqrt( pow(m_AxisRightY, 2) + pow(m_AxisRightX, 2)) ;

    if(m_AxisRightY<0.0f)
    {
        if(lenF>0.2f && lenF<=0.6f)
        {
            speedHuman = -0.1f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }

        }
        else if(lenF>0.6f)
        {
            speedHuman = -0.2f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }
        }
        else if (lenF < 0.2f)
        {
            speedHuman = 0;

            if(timerHumanGo->isActive())
            {
                timerHumanGo->stop();
            }
        }

    }
    else if(m_AxisRightY>0.0f)
    {

        if (lenF < 0.2f)
        {
            speedHuman = 0;

            if(timerHumanGo->isActive())
            {
                timerHumanGo->stop();
            }
        }
        else
        {
            speedHuman = 0.1f;

            if(!timerHumanGo->isActive())
            {
                timerHumanGo->start(SPEED_T_HUMAN);
            }
        }
    }
    else if(m_AxisRightY == 0.0f)
    {
        speedHuman = 0;

        if(timerHumanGo->isActive())
        {
            timerHumanGo->stop();
        }
    }


    if(timerHumanGo->isActive())
    {
//        mStateHumanGo = true;
        if(m_OnErth)
        {
            setStateHumanGo(true, STATE_RUN);
        }
    }
    else
    {
//        mStateHumanGo = false;
        if(m_OnErth)
        {
            setStateHumanGo(false, STATE_STANDUP);
        }
    }

}

void ModelHumanH::moveHuman()
{
    qDebug()<<"Move......"<<mStateHumanGo;

    float rotY = getModelRotate().y() - m_AxisRightXEdit;

    if(rotY >= 360.0f)
    {
        rotY = fmod(rotY,360.0f);
    }
    else if(rotY < 0.0f)
    {
        rotY = 360.0f + rotY;
    }

    float dashX = sin(qDegreesToRadians(rotY));
    float dashZ = cos(qDegreesToRadians(rotY));

    //check Area
    float posX = getModelPosition().x()+(dashX*speedHuman);
    float posZ = getModelPosition().z()+(dashZ*speedHuman);
    if(posX > 100 || posX < -100 || posZ > 100 || posZ < -100)
    {
        // Not Move - Out Area Map
    }
    else
    {
        setModelPosition(QVector3D(posX, getModelPosition().y(), posZ ));

    }

    setModelRotate(QVector3D(getModelRotate().x(), rotY, getModelRotate().z()));
    setDirection(QVector3D(dashX, getDirection().y(), dashZ));

    if(mStateHumanGo)
    {
        mStateHumanGo=false;
        speedHumanOld = speedHuman;
        m_AxisRightXEditOld = m_AxisRightXEdit;

        qDebug()<<"ZZ: "<<getModelPosition()<<" | "<<getModelRotate()<<":: "<<speedHumanOld;

        m_controlCol->stateControl2(getModelPosition().x(),
                                    getModelPosition().y(),
                                    getModelPosition().z(),
                                    getModelRotate().x(),
                                    getModelRotate().y(),
                                    getModelRotate().z());
    }
}

void ModelHumanH::jump()
{
    if(m_OnErth)
    {
        if(mStateHumanDown)
        {
            if(timerHumanGo->isActive())
            {
                setStateHumanGo(true,STATE_RUN);
            }
            else
            {
                setStateHumanGo(true,STATE_STANDUP);
            }
        }
        else
        {
            oldJump_Y=getModelPosition().y();
            timerJump->start(20);
            m_OnErth = false;
            m_controlCol->setStateGrv(false);
            setStateHumanGo(true, STATE_JUMPUP);
        }
    }
}

void ModelHumanH::slotTimerJump()
{
    float newY_Jump = getModelPosition().y();

    if(newY_Jump < (oldJump_Y+3.5f))
    {
        setModelPosition(QVector3D(getModelPosition().x(),
                                   getModelPosition().y()+0.05f,
                                   getModelPosition().z()));
    }
    else
    {
        timerJump->stop();
        m_controlCol->setStateGrv(true);
        m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
                                    getModelRotate().x(), getModelRotate().y(), getModelRotate().z());
    }
}

void ModelHumanH::setLink(bool stateLink)
{
    m_stateLink = stateLink;
}
bool ModelHumanH::checkLink()
{
    return m_stateLink;
}

void ModelHumanH::setAxisRotLink(double axis)
{

}

void ModelHumanH::slotRotLink()
{

}

void ModelHumanH::sendOnErth(bool on)
{
    qDebug("CH*");
    mStateHumanGo=true;

    if(m_OnErth != on)
    {
        m_OnErth = on;

        if(m_OnErth)
        {
            if(timerHumanGo->isActive())
            {
                setStateHumanGo(true, STATE_RUN);
            }
            else
            {
                setStateHumanGo(true, STATE_STANDUP);
            }
        }
        else
        {
            setStateHumanGo(true, STATE_JUMPUP);
        }
    }
}

QMatrix4x4 ModelHumanH::getPosGun(int x)
{
    QVector<QMatrix4x4*> *vectorMatrix(m_vaoAssimp->getPose2());

    uint numJointt = vectorMatrix->count();

    QMatrix4x4 arrayMatrixJoints[numJointt];

    for(uint i=0; i<numJointt; ++i)
    {
        arrayMatrixJoints[i].setColumn(0, vectorMatrix->at(i)->row(0));
        arrayMatrixJoints[i].setColumn(1, vectorMatrix->at(i)->row(1));
        arrayMatrixJoints[i].setColumn(2, vectorMatrix->at(i)->row(2));
        arrayMatrixJoints[i].setColumn(3, vectorMatrix->at(i)->row(3));
    }

//    return arrayMatrixJoints[x];
    return *vectorMatrix->at(x);
}

void ModelHumanH::setOnTransport(bool check)
{
    m_stateOn = check;
}
bool ModelHumanH::checkOn()
{
    return m_stateOn;
}

void ModelHumanH::slotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth)
{
    qDebug("YES EMIT COLLISION COLLADA");
    timerHumanGo->stop();
    mStateHumanGo = false;

    setModelPosition(QVector3D(posx,posy,posz));
    setModelRotate(QVector3D(rotx,roty+m_AxisRightXEditOld,rotz));
    qDebug()<<"BB: "<<getModelPosition()<<" | "<<getModelRotate()<<":: "<<speedHumanOld;
    setStateHumanGo(false, STATE_STANDUP);

    speedHuman = -(speedHumanOld);
//    m_AxisRightXEdit = -m_AxisRightXEditOld;
    moveHuman();
    speedHuman = -(speedHuman);
//    m_AxisRightXEdit = -m_AxisRightXEdit;
//    setModelRotate(QVector3D(getModelRotate().x(), getModelRotate().y()+m_AxisRightXEditOld,getModelRotate().z()));
    qDebug()<<"#CC: "<<getModelPosition();

    //cleanCollision
    m_controlCol->stateControl2(getModelPosition().x(), getModelPosition().y(), getModelPosition().z(),
                                getModelRotate().x(), getModelRotate().y(), getModelRotate().z());

    //run Check Collision
//    mStateHumanGo = true;
    sendOnErth(onErth);
}

void ModelHumanH::slotNotCollisionCollada(float posx, float posy, float posz, float rotx, float roty, float rotz, bool onErth)
{
    setModelPosition(QVector3D(posx,posy,posz));
//    setModelRotate(rot);
    sendOnErth(onErth);
}


void ModelHumanH::setStateHumanGo(bool state, int stateNow)
{

    if(stateNow==STATE_RUN)
    {
        if(mStateHumanDown)
        {
            m_vaoAssimp->setStateAnim(STATE_RUNDOWN); // move Down
        }
        else
        {
            m_vaoAssimp->setStateAnim(STATE_RUN); // move
        }
    }
    else if(stateNow==STATE_STANDUP)
    {
        if(mStateHumanDown)
        {
            m_vaoAssimp->setStateAnim(STATE_SEDOWN); // sedown
        }
        else
        {
            m_vaoAssimp->setStateAnim(STATE_STANDUP); // standup
        }
    }
    else if(stateNow==STATE_SEDOWN)
    {
        m_vaoAssimp->setStateAnim(STATE_SEDOWN); // sedown
    }
    else if(stateNow==STATE_RUNDOWN)
    {
        m_vaoAssimp->setStateAnim(STATE_RUNDOWN); // move Down
    }
    else if(stateNow==STATE_JUMPUP)
    {
        m_vaoAssimp->setStateAnim(STATE_JUMPUP); // jumpUp
    }
    else if(stateNow==5)
    {

    }
    else if(stateNow==6)
    {

    }

}


