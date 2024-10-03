#include "entityemitterfire.h"
#include "math.h"
#include <QtMath>
#include <QDebug>
#include <QTimer>

EntityEmitterFire::EntityEmitterFire(QVector3D vecPos, QObject *parent)
    : QObject(parent), m_pos(vecPos)
{
    listFirePartecal = new QVector<EntityParticleFire*>;
}

EntityEmitterFire::~EntityEmitterFire()
{
    for(int i=0; i<listFirePartecal->count(); ++i)
    {
        delete listFirePartecal->at(i);
    }
//    qDeleteAll(listFirePartecal);
    delete listFirePartecal;
    delete timerScaleFire;
    delete this;
}

void EntityEmitterFire::createPartecal()
{
    setPosition(m_pos);
//    qDebug()<<"THreads: "<<thread();

    for(int c=0; c<0; ++c)
    {
        EntityParticleFire *f = new EntityParticleFire;
        float mada = 0.5f;
        float mada2 = 1.0f;

        if(c==0)
        {
            m_pos = QVector3D(m_pos.x(),m_pos.y()+mada2,m_pos.z());
        }
        else if(c==1)
        {
            m_pos = QVector3D(m_pos.x()+mada,m_pos.y(),m_pos.z()+mada);
        }
        else if(c==2)
        {
            m_pos = QVector3D(m_pos.x()-mada,m_pos.y(),m_pos.z()+mada);
        }
        else if(c==3)
        {
            m_pos = QVector3D(m_pos.x()-mada,m_pos.y(),m_pos.z()-mada);
        }
        else if(c==4)
        {
            m_pos = QVector3D(m_pos.x()+mada,m_pos.y(),m_pos.z()-mada);
        }
        else if(c==5)
        {
            m_pos = QVector3D(m_pos.x()+mada2,m_pos.y(),m_pos.z()+mada2);
        }
        else if(c==6)
        {
            m_pos = QVector3D(m_pos.x()-mada2,m_pos.y(),m_pos.z()+mada2);
        }
        else if(c==7)
        {
            m_pos = QVector3D(m_pos.x()-mada2,m_pos.y(),m_pos.z()-mada2);
        }
        else if(c==8)
        {
            m_pos = QVector3D(m_pos.x()+mada2,m_pos.y(),m_pos.z()-mada2);
        }

        f->setPosition(m_pos);
        f->setScale(QVector3D(5,5,5));
        listFirePartecal->append(f);
    }


    QList<float> listAngel; listAngel<<0<<15<<30<<45<<60<<75<<90<<105<<120<<135<<150<<165<<180;
    float lenth=0.0f;

    for(int c=0; c<10; c++)
    {
        lenth+=0.2f;

        for(int cc=0; cc<listAngel.count(); cc++)
        {

            EntityParticleFire *f = new EntityParticleFire;

            float lenthX = cos(qDegreesToRadians(listAngel.at(cc)));
            float lenthY = sin(qDegreesToRadians(listAngel.at(cc)));
            float lenthZ = m_pos.z();

            f->setPosition(QVector3D(m_pos.x()+ (lenthX * lenth),
                                     m_pos.y()+ (lenthY * lenth),
                                     lenthZ));
            f->setScale(QVector3D(2.0f,2.0f,2.0f));
            listFirePartecal->append(f);
        }
    }



    timerScaleFire = new QTimer(this);
    connect(timerScaleFire, &QTimer::timeout, this, &EntityEmitterFire::setScaleFire);
    timerScaleFire->start(10);
    nums = listFirePartecal->count();
}

void EntityEmitterFire::setScaleFire()
{
    nums-=10;
    if(nums<0) {nums=0;}
        for(int i=nums; i<listFirePartecal->count(); ++i)
        {
//            qDebug()<<"Nums: "<<i;
            float valScale = listFirePartecal->at(i)->getScale().x();
            float valTrans= listParticale().at(i)->getTransparency();

            if(valScale>0.0f)
            {
                listFirePartecal->at(i)->setScale(QVector3D(valScale-0.02f,valScale-0.02f,valScale-0.02f));
            }
            if(valTrans > 0.0f)
            {
                listParticale().at(i)->setTransparency(valTrans-0.01f);
            }

        }
        float  valScaleFull = listFirePartecal->at(0)->getScale().x();
//        qDebug()<<"End Parts 0 = "<<valScaleFull;
        if(!(valScaleFull>0.0f))
        {
//            qDebug("Finish Fire";
            emit emtFinished();
            timerScaleFire->stop();
        }
}

QVector<EntityParticleFire *> EntityEmitterFire::listParticale()
{
    return *listFirePartecal;
}
