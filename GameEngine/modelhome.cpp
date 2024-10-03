#include "modelhome.h"
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include "entity.h"
#include "vaoentity.h"

ModelHome::ModelHome(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) :
  QObject(parent),
  m_shader(shader),
  m_vao(vao)
{

}

ModelHome::~ModelHome()
{

}

void ModelHome::createEntity()
{
    entityHome = new Entity;
    entityHome->setNameEntity(m_vao->getListInfo()->at(0)->name);
    entityHome->setLimited(m_vao->getListInfo()->at(0)->limited);
}

QVector<QCubeLimited *> *ModelHome::getModelLimitParts()
{
    return m_vao->getListInfo()->at(0)->partsLimited;
}

void ModelHome::draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam)
{

    QMatrix4x4 matrixHome;
    matrixHome.translate(getModelPosition());
    matrixHome.rotate(getModelRotate().y(), 0,1,0);
    matrixHome.rotate(getModelRotate().z(), 0,0,1);
    matrixHome.rotate(getModelRotate().x(), 1,0,0);
    matrixHome.scale(getModelScale());

    m_shader->bind();
    m_shader->setUniformValue("pMatrix", matrixPro);
    m_shader->setUniformValue("mvMatrix", matrixCam * matrixHome);
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
