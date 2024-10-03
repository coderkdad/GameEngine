#ifndef MODELHUMANENEMY_H
#define MODELHUMANENEMY_H

#include <QObject>
#include <QOpenGLShaderProgram>

class Entity;
class VAOEntity;
class EntityInfo;
class QCubeLimited;

class ModelHumanEnemy : public QObject
{
    Q_OBJECT
public:
    explicit ModelHumanEnemy(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelHumanEnemy();
    void createEntity();
    void setModelPosition(QVector3D pos);
    void setModelRotate(QVector3D rot);
    void setModelScale(QVector3D scal);
    void setDirection(QVector3D dir);
    void setModelLimited();
    QVector3D getModelPosition();
    QVector3D getModelRotate();
    QVector3D getModelScale();
    QVector3D getDirection();
    QCubeLimited *getModelLimited();
    QList<Entity *> getListEntity();
signals:

};

#endif // MODELHUMANENEMY_H
