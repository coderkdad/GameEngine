#ifndef MODELERTH_H
#define MODELERTH_H

#include <QObject>
#include <QVector3D>
#include "basemodel.h"

class QOpenGLShaderProgram;
class VAOEntity;
class EntityInfo;
class QCubeLimited;
class Entity;

class ModelErth : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelErth(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelErth();
    void createEntity();

    void draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam);

    void setInfo(EntityInfo *info, bool fillAll=false);
    EntityInfo *getInfo();

signals:

private:
    QOpenGLShaderProgram *m_shader;
    VAOEntity *m_vao;

    EntityInfo *m_info;

};

#endif // MODELERTH_H
