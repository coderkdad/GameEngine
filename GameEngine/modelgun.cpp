#include "modelgun.h"
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include "entity.h"
#include "vaoentity.h"

ModelGun::ModelGun(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) :
    QObject(parent),
    m_shader(shader),
    m_vao(vao)
{

}

ModelGun::~ModelGun()
{

}

void ModelGun::createEntity()
{
    entityGun = new Entity;
    entityGun->setNameEntity(m_vao->getListInfo()->at(0)->name);
    entityGun->setLimited(m_vao->getListInfo()->at(0)->limited);
}

//void ModelGun::setModelPosition(QVector3D pos)
//{
//    modelPos = pos;
//}
//void ModelGun::setModelRotate(QVector3D rot)
//{
//    modelRot = rot;
//}

//void ModelGun::setModelScale(QVector3D scal)
//{
//    modelScal = scal;
//}

//void ModelGun::setModelLimited(QCubeLimited *limit)
//{
//    modelLimit = limit;
//}
//QVector3D ModelGun::getModelPosition()
//{
//    return modelPos;
//}
//QVector3D ModelGun::getModelRotate()
//{
//    return modelRot;
//}

//QVector3D ModelGun::getModelScale()
//{
//    return modelScal;
//}

//QCubeLimited *ModelGun::getModelLimited()
//{
//    return  modelLimit;
//}

void ModelGun::draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam)
{

    QMatrix4x4 matrixGun;
    matrixGun.translate(getModelPosition());
    matrixGun.rotate(getModelRotate().y(), 0,1,0);
    matrixGun.rotate(getModelRotate().z(), 0,0,1);
    matrixGun.rotate(getModelRotate().x(), 1,0,0);
    matrixGun.scale(getModelScale());

    m_shader->bind();
    m_shader->setUniformValue("pMatrix", matrixPro);
    m_shader->setUniformValue("mvMatrix", matrixCam * matrixGun);
    m_shader->setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
    m_shader->setUniformValue("u_lightPower", 1.0f);
    m_shader->setUniformValue("fragFlag", 1.0f);
    m_shader->setUniformValue("texturTransparency", 1.0f);

    m_vao->getListInfo()->at(0)->vao->bind();
    m_vao->getTextureImg()->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vao->getListInfo()->at(0)->numVertex); // create List
    m_vao->getTextureImg()->release();
    m_vao->getListInfo()->at(0)->vao->release();

    m_shader->release();
}
