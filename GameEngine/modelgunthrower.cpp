#include "modelgunthrower.h"
#include "entity.h"
#include "vaoentity.h"
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QtMath>
#include "modelrobot.h"
#include "modelcar.h"
#include "modelplane.h"
#include "controllerthrower.h"
#include "entitywedge.h"
#include "modelerth.h"
#include "modelhome.h"


ModelGunThrower::ModelGunThrower(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent)
    : QObject(parent), m_shader(shader), m_vao(vao)
{
    timerMoveUpWedge = new QTimer;
    connect(timerMoveUpWedge, SIGNAL(timeout()), this, SLOT(slotTimerMoveUpHuman()));
}

ModelGunThrower::~ModelGunThrower()
{
    for(int l=0; l<listEntity.count(); ++l)
    {
        Entity *e = listEntity.takeAt(l);
        delete e;
        e = nullptr;
    }

    for(int l=0; l<listRope.count(); ++l)
    {
        Entity *e = listRope.takeAt(l);
        delete e;
        e = nullptr;
    }

    for(int l=0; l<listControllerWedge.count(); ++l)
    {
        ControllerThrower *e = listControllerWedge.takeAt(l);
        delete e;
        e = 0;
    }


    m_shader = nullptr;
    m_vao = nullptr;
    // Entitys
    m_erth = nullptr;
    m_listHome = nullptr;
    m_modelCar = nullptr;
    m_modelPlane = nullptr;
    m_modelHuman = nullptr;


    timerMoveUpWedge->deleteLater();
}

void ModelGunThrower::createEntity()
{
    entityThrower = new Entity;
    entityThrower->setPosition(QVector3D(0,0,0));
    entityThrower->setNameEntity(m_vao->getListInfo()->at(0)->name);
    entityThrower->setLimited(m_vao->getListInfo()->at(0)->limited);
    listEntity.append(entityThrower);

    entityRope = new Entity;
    entityRope->setPosition(QVector3D(0, 0, entityThrower->getLimited()->limitedBottom.z()));
    entityRope->setNameEntity(m_vao->getListInfo()->at(1)->name);
    entityRope->setLimited(m_vao->getListInfo()->at(1)->limited);
//    listEntity.append(entityRope);
    m_hieghtRope = fabs(entityRope->getLimited()->limitedBottom.z());

    {
        createConWedge();
    }

}
void ModelGunThrower::allEntityCollision(ModelErth *erth, QList<ModelHome *> *listHome,
                                         ModelCar *modelCar, ModelPlane *modelPlane, ModelRobot *modelHuman, ModelHumanH *modelCollada)
{
    m_erth = erth;
    m_listHome = listHome;
    m_modelHuman = modelHuman;
    m_modelCar = modelCar;
    m_modelPlane = modelPlane;
    m_modelCollada = modelCollada;

    connect(modelHuman, &ModelRobot::emtUpdate, this, &ModelGunThrower::slotUpdateHuman);
    connect(modelPlane, &ModelPlane::emtUpdate, this, &ModelGunThrower::slotUpdatePlane);
}
//void ModelGunThrower::setModelPosition(QVector3D pos)
//{
//    modelPos = pos;
//}
//void ModelGunThrower::setModelRotate(QVector3D rot)
//{
//    modelRot = rot;
//}
//void ModelGunThrower::setModelScale(QVector3D scal)
//{
//    modelScal = scal;
//}
void ModelGunThrower::setDirection(QVector3D dir)
{
    m_dir = dir;
}
//QVector3D ModelGunThrower::getModelPosition()
//{
//    return modelPos;
//}
//QVector3D ModelGunThrower::getModelRotate()
//{
//    return modelRot;
//}
//QVector3D ModelGunThrower::getModelScale()
//{
//    return modelScal;
//}

QVector3D ModelGunThrower::getDirection()
{
    return m_dir;
}
QList<Entity *> ModelGunThrower::getListEntity()
{
    return listEntity;
}

void ModelGunThrower::createConWedge()
{
    m_WedgeGo = false;
    m_WedgeLink = false;
    m_WedgeDelete = false;

    QVector3D posWed = calcRotPosition2(QVector3D(0, 0, entityThrower->getLimited()->limitedBottom.z()),
                                        QVector3D(m_rotUp, 0,0));

    ControllerThrower *conWedge = new ControllerThrower;
    conWedge->getEntityWedge()->setPosition(posWed);
    conWedge->getEntityWedge()->setStartPos(posWed, m_hieghtRope, 3.0f, 50); //
    conWedge->getEntityWedge()->setDirection(getDirection());
    conWedge->getEntityWedge()->setNameEntity(m_vao->getListInfo()->at(2)->name);
    conWedge->getEntityWedge()->setLimited(m_vao->getListInfo()->at(2)->limited);
    conWedge->getEntityWedge()->allEntityCollision(m_erth, m_listHome, m_modelCar, m_modelPlane, m_modelHuman);
    listControllerWedge.append(conWedge);

    connect(conWedge, &ControllerThrower::emtUpdate, this, &ModelGunThrower::slotUpdateWedge);
    connect(conWedge, &ControllerThrower::emtDeleteWedge, this, &ModelGunThrower::slotDeleteWedge);
    connect(conWedge, &ControllerThrower::emtCollisionThat, this, &ModelGunThrower::slotCollisionWedge);

}
void ModelGunThrower::createRope()
{
    Entity *entityRopeDouble = new Entity(*entityRope);
    if(listRope.count() == 0)
    {
        listRope.append(entityRopeDouble); // لا حاجة دعه يكمل بعد خروج اول قطعه مستقيمه حسب القاذف
    }
    else
    {
        int d = fmod(int(listRope.count()/50),2);
        float dd = 0.0f;
        if(d == 0)
        {
            dd=0.005f;
        }
        else
        {
            dd=-0.005f;
        }
        entityRopeDouble->setPosition(QVector3D(listRope.last()->getPosition().x() + getDirection().x()*m_hieghtRope,
                                                listRope.last()->getPosition().y() + getDirection().y()*m_hieghtRope,
                                                listRope.last()->getPosition().z() + getDirection().z()*m_hieghtRope));
        listRope.append(entityRopeDouble);
    }
}
void ModelGunThrower::clearThrower()
{
    /// Wedge
    foreach(ControllerThrower *Con, listControllerWedge)
    {
        listControllerWedge.removeOne(Con);
        delete Con;
    }
    listControllerWedge.clear();

    /// Rope
    foreach (Entity *e, listRope) {
        listRope.removeOne(e);
        delete e;
    }
    listRope.clear();
}

void ModelGunThrower::slotUpdateWedge()
{
    createRope();
}
void ModelGunThrower::slotDeleteWedge()
{
    m_WedgeDelete = true;

//    clearThrower();
//    createConWedge();
}

void ModelGunThrower::draw(QMatrix4x4 matrixProCam)
{
    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(getModelPosition());
    matrixModel.rotate(getModelRotate().y(), 0, 1, 0);
    matrixModel.rotate(getModelRotate().z(), 0, 0, 1);
    matrixModel.rotate(getModelRotate().x() +0.0f, 1, 0, 0);

    for(int i=0; i<getListEntity().count(); ++i)
    {
        if(i==1)
        {
//            break;
        }

        QMatrix4x4 matrixEntity;
        matrixEntity.setToIdentity();
        matrixEntity.translate(getListEntity().at(i)->getPosition());
        matrixEntity.rotate(getListEntity().at(i)->getRotation().y(), 0,1,0);
        matrixEntity.rotate(getListEntity().at(i)->getRotation().z(), 0,0,1);
        matrixEntity.rotate(getListEntity().at(i)->getRotation().x() +0.0f, 1,0,0);

        m_shader->bind();
        m_shader->setUniformValue("mvMatrix", (/*matrixGun**/ matrixModel*matrixEntity)); /// LIGHTING
        m_shader->setUniformValue("pMatrix", matrixProCam);
        m_shader->setUniformValue("fragFlag", 0.1f);
        m_shader->setUniformValue("texturTransparency", 1.0f);

        m_vao->getListInfo()->at(0)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(i)->numVertex);
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(0)->vao->release();

        m_shader->release();

    }

    for(int i=0; i<listRope.count(); ++i)
    {
        QMatrix4x4 matrixEntity;
        matrixEntity.setToIdentity();
        matrixEntity.translate(listRope.at(i)->getPosition());
        matrixEntity.rotate(listRope.at(i)->getRotation().y(), 0,1,0);
        matrixEntity.rotate(listRope.at(i)->getRotation().z(), 0,0,1);
        matrixEntity.rotate(listRope.at(i)->getRotation().x(), 1,0,0);

        m_shader->bind();
        if(m_WedgeGo)
        {
            m_shader->setUniformValue("mvMatrix", (/*matrixGun* matrixModel**/matrixEntity)); /// LIGHTING
        } else {
            m_shader->setUniformValue("mvMatrix", (/*matrixGun**/ matrixModel*matrixEntity)); /// LIGHTING
        }
        m_shader->setUniformValue("pMatrix", matrixProCam);
        m_shader->setUniformValue("fragFlag", 0.1f);
        m_shader->setUniformValue("texturTransparency", 1.0f);

        m_vao->getListInfo()->at(1)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(1)->numVertex);
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(1)->vao->release();

        m_shader->release();
    }

    for(int i=0; i<listControllerWedge.count(); ++i)
    {
        QMatrix4x4 matrixEntity;
        matrixEntity.setToIdentity();
        matrixEntity.translate(listControllerWedge.at(i)->getEntityWedge()->getPosition());
        matrixEntity.rotate(listControllerWedge.at(i)->getEntityWedge()->getRotation().y(), 0,1,0);
        matrixEntity.rotate(listControllerWedge.at(i)->getEntityWedge()->getRotation().z(), 0,0,1);
        matrixEntity.rotate(listControllerWedge.at(i)->getEntityWedge()->getRotation().x(), 1,0,0);

        m_shader->bind();
        if(m_WedgeGo)
        {
            m_shader->setUniformValue("mvMatrix", (/*matrixGun* matrixModel**/matrixEntity)); /// LIGHTING
        } else {
            m_shader->setUniformValue("mvMatrix", (/*matrixGun**/ matrixModel*matrixEntity)); /// LIGHTING
        }
        m_shader->setUniformValue("pMatrix", matrixProCam);
        m_shader->setUniformValue("fragFlag", 0.1f);
        m_shader->setUniformValue("texturTransparency", 1.0f);

        m_vao->getListInfo()->at(2)->vao->bind();
        m_vao->getTextureImg()->bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(0)->numVertex);
        m_vao->getTextureImg()->release();
        m_vao->getListInfo()->at(2)->vao->release();

        m_shader->release();
    }

}

void ModelGunThrower::shutThrower(QVector3D posWedge, QVector3D rotWedge, QVector3D dirWedge)
{

    if(listControllerWedge.count() > 0)
    {
        if(listControllerWedge.count() > 1)
        {
            clearThrower();
            createConWedge();
        }

        if(m_WedgeDelete)
        {
            clearThrower();
            createConWedge();
        }
        else if(m_WedgeLink)
        {
            timerMoveUpWedge->start(20);
        }
        else
        {
            if(!m_WedgeGo)
            {
                QVector3D vecPos = calcRotPosition2(QVector3D(0,0, entityThrower->getLimited()->limitedBottom.z()), rotWedge);
                QVector3D mposWedge = posWedge + vecPos;

                m_WedgeGo = true;
                setDirection(dirWedge);

                entityRope->setPosition(mposWedge);
                entityRope->setRotation(rotWedge);

                listControllerWedge.at(0)->getEntityWedge()->setPosition(mposWedge);
                listControllerWedge.at(0)->getEntityWedge()->setRotation(rotWedge);
                listControllerWedge.at(0)->getEntityWedge()->setStartPos(mposWedge, m_hieghtRope, 18.0f, 2);
                listControllerWedge.at(0)->getEntityWedge()->setDirection(getDirection());
                listControllerWedge.at(0)->runWedge();
            }
        }
    }
    else
    {
        createConWedge();
    }

}

void ModelGunThrower::setRotUp(float rotUp)
{
    m_rotUp = rotUp;

    QVector3D posThrower(0,0, entityThrower->getLimited()->limitedBottom.z());
    QVector3D rotThrower(m_rotUp, 0,0);

    entityThrower->setRotation(rotThrower);

    if(!m_WedgeGo)
    {
//        entityRope->setRotation(rotThrower);
//        entityRope->setPosition( calcRotPosition2(posThrower, rotThrower) );

        listControllerWedge.at(0)->getEntityWedge()->setRotation(rotThrower);
        listControllerWedge.at(0)->getEntityWedge()->setPosition( calcRotPosition2(posThrower, rotThrower) );
    }
}

void ModelGunThrower::slotCollisionWedge(QString nameEntity, QVector3D *posColl)
{
    m_WedgeLink = true;

    if(nameEntity.startsWith("Home"))
    {
        QStringList lis = nameEntity.split(" ");
        int idHoome = lis.at(1).toInt();
    }
    else if (nameEntity == "Car")
    {
        m_vecLinkHuman = m_modelCar->getModelPosition() - m_modelHuman->getModelPosition();
        m_modelHuman->setLink(true);
    }
    else if (nameEntity == "Plane")
    {

        m_vecLinkHuman = m_modelPlane->getModelPosition() - m_modelHuman->getModelPosition();
        m_vecLinkWedge = m_modelPlane->getModelPosition() - listControllerWedge.at(0)->getEntityWedge()->getPosition() ;// ->getModelPosition();
        if(listRope.count() > 0)
        {
            m_vecLinkRope  = m_modelPlane->getModelPosition() - listRope.at(0)->getPosition();
        }
        m_rotPlane = QVector3D(0, m_modelPlane->getModelRotate().y(), 0);
        m_rotHuman = QVector3D(listControllerWedge.at(0)->getEntityWedge()->getRotation().x(),
                               m_modelHuman->getModelRotate().y(), 0);
        m_modelHuman->setLink(true);
    }
    else
    {
        m_vecLinkHuman = QVector3D(0,0,0);
    }
}

void ModelGunThrower::slotUpdateHuman()
{
    // set Pos Start Rope and Rotate
    // Bettwen Thrower And Wedge
}
void ModelGunThrower::slotUpdatePlane()
{
    if(m_WedgeLink)
    {
        setPosAllMove();
    }
}

void ModelGunThrower::slotTimerMoveUpHuman()
{
    if(listRope.count() > 0)
    {
        setVecLink();
        listRope.removeFirst();
    }
    else
    {
        timerMoveUpWedge->stop();
        clearThrower();
        createConWedge();
        m_modelHuman->setLink(false);
    }
}


QVector3D ModelGunThrower::getPosFromEntity2(QVector3D posParent, QVector3D rotParent, QVector3D posChild)
{
    // posModel - rotModel - posBelly

     QVector3D posChilds = calcRotPosition2(posChild, rotParent);
     return posChilds + posParent;

}
QVector3D ModelGunThrower::calcRotPosition2(QVector3D pos, QVector3D rots)
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
    QVector3D posEntityNew3 = QVector3D(lenthVec3* cos(qDegreesToRadians(doubleRotVec3)),
                                        lenthVec3* sin(qDegreesToRadians(doubleRotVec3)),
                                        posEntityNew.z());

    /// y
    float lenthVec2 = sqrt( pow(posEntityNew3.x(),2) + pow(posEntityNew3.z(),2) );
    float angelVec2 = qRadiansToDegrees( atan2(posEntityNew3.x(), posEntityNew3.z()) ) ;
    float doubleRotVec2 = angelVec2+rotEntity.y();
    QVector3D posEntityNew2 = QVector3D(lenthVec2* sin(qDegreesToRadians(doubleRotVec2)),
                                        posEntityNew3.y(),
                                        lenthVec2* cos(qDegreesToRadians(doubleRotVec2)));

    return posEntityNew2;
}

void ModelGunThrower::setVecLink()
{
    m_vecLinkHuman = m_vecLinkHuman - (getDirection()*m_hieghtRope);
    m_vecLinkRope  = m_vecLinkRope  - (getDirection()*m_hieghtRope);
}

void ModelGunThrower::setPosAllMove()
{
    QVector3D posPlane = m_modelPlane->getModelPosition();
    QVector3D rotPlane = QVector3D(0, m_modelPlane->getModelRotate().y(), 0) - m_rotPlane;
    QVector3D rotFinal = m_rotHuman+rotPlane;
    
    // Human
    {
        QVector3D posLink = calcRotPosition2(m_vecLinkHuman, rotPlane);
        m_modelHuman->setModelPosition(posPlane - posLink);
//        m_modelHuman->setModelRotate(m_rotHuman+rotPlane); // Select AutoRot With Plane OR BaseRot
    }
    // Wedge
    {
        QVector3D posLink = calcRotPosition2(m_vecLinkWedge, rotPlane);
        listControllerWedge.at(0)->getEntityWedge()->setPosition(posPlane - posLink);
        listControllerWedge.at(0)->getEntityWedge()->setRotation(rotFinal);
    }
    // Rope
    for(int i=0; i<listRope.count(); ++i)
    {
        QVector3D posRope = m_vecLinkRope-(getDirection()*m_hieghtRope*i);
        QVector3D posLink = calcRotPosition2(posRope, rotPlane);
        listRope.at(i)->setPosition(posPlane - posLink);
        listRope.at(i)->setRotation(rotFinal);
    }
}









