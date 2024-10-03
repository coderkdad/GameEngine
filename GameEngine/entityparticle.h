#ifndef ENTITYPARTICLE_H
#define ENTITYPARTICLE_H

#include <QObject>
#include "entity.h"

class EntityParticle : public QObject, public Entity
{
    Q_OBJECT
    // Postion //Speed //Life //color&texture
public:
    explicit EntityParticle(QObject *parent = nullptr);
//    void setPostion(QVector3D pos);
//    void setTexture(QVector3D pos);
    void setSpeed(QVector3D speed);
    void setLife(int milisecTime);

signals:

};

#endif // ENTITYPARTICLE_H
