#include "entity.h"
#include "vaoentity.h"

Entity::Entity()
{
//    m_vec2 = new QVector<QTriangle3D>;
}

Entity::~Entity()
{
    qDebug()<<"Des : "<<nameEntity();
//    m_vec2->clear();
//    delete m_vec2;
//    m_vec2 = nullptr;
}
///=============== Base ====================///
void Entity::setPosition(QVector3D vector)
{
    m_Position=vector;
}
QVector3D Entity::getPosition()
{
    return m_Position;
}

void Entity::setRotation(QVector3D vector)
{
    m_Rotation = vector;
}
QVector3D Entity::getRotation()
{
    return m_Rotation;
}

void Entity::setScale(QVector3D vector)
{
    m_Scale = vector;
}
QVector3D Entity::getScale()
{
    return m_Scale;
}

///=============== Info ====================///
void Entity::setInfo(EntityInfo *info, bool fillAll)
{
    if(fillAll)
    {
        m_info = info;
        m_nameEntity = info->name;
        m_limited = info->limited;
        m_limitedParts = info->partsLimited;
//        m_vecTring = info->vecTringle;
    }
    else
    {
        m_info = info;
    }

}
EntityInfo *Entity::getInfo()
{
    return m_info;
}

void Entity::setNameEntity(QString name)
{
    m_nameEntity = name;
}
QString Entity::nameEntity()
{
    return m_nameEntity;
}

void Entity::setLimited(QCubeLimited *cubeLimit)
{
    m_limited = cubeLimit;//
}
QCubeLimited *Entity::getLimited()
{
    return m_limited;
}

void Entity::setLimitedParts(QVector<QCubeLimited *> *parts)
{
    m_limitedParts = parts;
}
QVector<QCubeLimited *> *Entity::getLimitedParts()
{
    return m_limitedParts;
}

///=============== Trinagle ====================///
void Entity::setVecTring(QVector<QTriangle3D> *vecTring)
{
    m_vecTring = vecTring;//
}
QVector<QTriangle3D> *Entity::getVecTring()
{
    return m_vecTring;
}

float Entity::getTopX()
{
    return m_limited->limitedTop.x();
}
float Entity::getTopY()
{
    return m_limited->limitedTop.y();
}
float Entity::getTopZ()
{
    return m_limited->limitedTop.z();
}
float Entity::getBottomX()
{
    return m_limited->limitedBottom.x();
}
float Entity::getBottomY()
{
    return m_limited->limitedBottom.y();
}
float Entity::getBottomZ()
{
    return m_limited->limitedBottom.z();
}



















