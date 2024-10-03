#ifndef MODELHOME_H
#define MODELHOME_H

#include <QObject>
#include <QVector3D>
#include "basemodel.h"

class QCubeLimited;
class Entity;
class VAOEntity;
class QOpenGLShaderProgram;

class ModelHome : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelHome(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelHome();
    void createEntity();
    QVector<QCubeLimited *> *getModelLimitParts();

    void draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam);

signals:
private:
    QOpenGLShaderProgram *m_shader = nullptr;
    VAOEntity *m_vao = nullptr;

    Entity *entityHome;

};

#endif // MODELHOME_H
