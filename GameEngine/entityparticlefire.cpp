#include "entityparticlefire.h"
#include "math.h"
#include <unistd.h>
#include <QDebug>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>
EntityParticleFire::EntityParticleFire()
{
}

void EntityParticleFire::setPosition(QVector3D vecPos)
{
    m_position = vecPos;
}
QVector3D EntityParticleFire::getPosition()
{
    return m_position;
}

QVector3D EntityParticleFire::getRotation()
{
    return QVector3D(0,0,0);
}

void EntityParticleFire::setScale(QVector3D scl)
{
    m_scale=scl;
}
QVector3D EntityParticleFire::getScale()
{
    return m_scale;
}

void EntityParticleFire::setTransparency(float trans)
{
    m_transparency = trans;
}
float EntityParticleFire::getTransparency()
{
    return m_transparency;
}


