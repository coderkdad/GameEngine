#ifndef ENTITYFIREPARTICLE_H
#define ENTITYFIREPARTICLE_H

#include "entityparticlefire.h"
//#include <QMutex>

class QTimer;
class EntityEmitterFire : public QObject ,public EntityParticleFire
{
    Q_OBJECT
public:
    explicit EntityEmitterFire(QVector3D vecPos=QVector3D(0,0,0), QObject *parent=nullptr);
    ~EntityEmitterFire();
    void createPartecal();
    QVector<EntityParticleFire *> listParticale();

signals:
    void emtFinished();

public slots:
    void setScaleFire();

private:
    QVector3D m_pos;
    QVector<EntityParticleFire*> *listFirePartecal;
    QTimer *timerScaleFire;
    int nums=0;

//    QMutex mMutex;

};

#endif // ENTITYFIREPARTICLE_H
