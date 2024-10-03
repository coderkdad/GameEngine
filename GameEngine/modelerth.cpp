#include "modelerth.h"
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include "vaoentity.h"

ModelErth::ModelErth(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent)
    : QObject(parent),
    m_shader(shader),
    m_vao(vao),
    m_info(vao->getListInfo()->at(0))
{
}

ModelErth::~ModelErth()
{
//    m_vao = nullptr;
    qDebug("X Erth");
}

void ModelErth::createEntity()
{
}

void ModelErth::setInfo(EntityInfo *info, bool fillAll)
{
    if(fillAll)
    {
        m_info = info;
        setModelLimit(info->limited);
    }
    else
    {
        m_info = info;
    }

}
EntityInfo *ModelErth::getInfo()
{
    return m_info; // استخدامها مؤقت للمصفوفة المرتفعات
}

void ModelErth::draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam)
{
    QMatrix4x4 matrixErth;
    matrixErth.translate(getModelPosition());
    matrixErth.rotate(getModelRotate().y(), 0,1,0);
    matrixErth.rotate(getModelRotate().z(), 0,0,1);
    matrixErth.rotate(getModelRotate().x(), 1,0,0);
    matrixErth.scale(getModelScale());

    m_shader->bind();
    m_shader->setUniformValue("pMatrix", matrixPro);
    m_shader->setUniformValue("mvMatrix", matrixCam * matrixErth);
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


