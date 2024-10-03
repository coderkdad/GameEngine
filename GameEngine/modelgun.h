#ifndef MODELGUN_H
#define MODELGUN_H

#include <QObject>
#include <QVector3D>
#include "basemodel.h"

class QCubeLimited;
class Entity;
class VAOEntity;
class QOpenGLShaderProgram;

class ModelGun : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelGun(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelGun();
    void createEntity();
//    void setModelPosition(QVector3D pos);
//    void setModelRotate(QVector3D rot);
//    void setModelScale(QVector3D scal);
//    void setModelLimited(QCubeLimited *limit);
//    QVector3D getModelPosition();
//    QVector3D getModelRotate();
//    QVector3D getModelScale();
//    QCubeLimited *getModelLimited();

    void draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam);
signals:

private:
    QOpenGLShaderProgram *m_shader = nullptr;
    VAOEntity *m_vao = nullptr;

    Entity *entityGun;
//    QVector3D modelPos = QVector3D(0,0,0);
//    QVector3D modelRot = QVector3D(0,0,0);
//    QVector3D modelScal = QVector3D(1,1,1);
//    QCubeLimited *modelLimit;

};

#endif // MODELGUN_H
