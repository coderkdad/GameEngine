#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QVector3D>

//struct QCubeLimited
//{
//    QVector3D limitedTop;
//    QVector3D limitedBottom;
//};

class QTriangle3D;
class EntityInfo;
class QCubeLimited;

class Entity
{
public:
    explicit Entity();
    ~Entity();
    //Setter
    void setPosition(QVector3D vector);
    void setRotation(QVector3D vector);
    void setScale(QVector3D vector);
    void setInfo(EntityInfo *info, bool fillAll=false);
    void setNameEntity(QString name);
    void setLimited(QCubeLimited *cubeLimit);
    void setLimitedParts(QVector<QCubeLimited *> *parts);
    void setVecTring(QVector<QTriangle3D> *vecTring);
    // Gettes
    QString nameEntity();
    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScale();
    QCubeLimited *getLimited();
    QVector<QCubeLimited *> *getLimitedParts();
    QVector<QTriangle3D> *getVecTring();
    EntityInfo *getInfo();
    //limited
    float getTopX();
    float getTopY();
    float getTopZ();
    float getBottomX();
    float getBottomY();
    float getBottomZ();

private:
    QVector3D m_Position = QVector3D(0,0,0);
    QVector3D m_Rotation = QVector3D(0,0,0);
    QVector3D m_Scale = QVector3D(1,1,1);
    EntityInfo *m_info;
    QString m_nameEntity="";
    QCubeLimited *m_limited;
    QVector<QCubeLimited*> *m_limitedParts;
    QVector<QTriangle3D> *m_vecTring;
//    QVector<QTriangle3D> *m_vec2;

};

#endif // ENTITY_H
