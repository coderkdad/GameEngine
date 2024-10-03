#ifndef ENTITYFIRE_H
#define ENTITYFIRE_H

#include <QObject>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class EntityParticleFire
{
public:
    explicit EntityParticleFire();
    // Gettes
    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScale();
    float getTransparency();
    // Setter
    void setPosition(QVector3D vector);
    void setRotation(QVector3D vector);
    void setScale(QVector3D scl);
    void setTransparency(float trans);

private:

    QVector3D m_position;
    QVector3D m_rotation;
    QVector3D m_scale;
    float m_transparency=1.0f;

};


#endif // ENTITYFIRE_H
