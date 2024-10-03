#include "basemodel.h"
#include "vaoentity.h"

BaseModel::BaseModel()
{

}

BaseModel::~BaseModel()
{

}

void BaseModel::setModelName(QString nm)
{
    name = nm;
}
void BaseModel::setModelPosition(QVector3D pos)
{
    modelPos = pos;
}
void BaseModel::setModelRotate(QVector3D rot)
{
    modelRot = rot;
}
void BaseModel::setModelScale(QVector3D scal)
{
    modelScal = scal;
}
void BaseModel::setModelLimit(QCubeLimited *limit)
{
    modelLimitFull = limit;
}

Q_INVOKABLE QString BaseModel::getModelName()
{
    return name;
}

Q_INVOKABLE QVector3D BaseModel::getModelPosition()
{
    return modelPos;
}
Q_INVOKABLE QVector3D BaseModel::getModelRotate()
{
    return modelRot;
}
Q_INVOKABLE QVector3D BaseModel::getModelScale()
{
    return modelScal;
}
Q_INVOKABLE QCubeLimited *BaseModel::getModelLimited()
{
    return modelLimitFull;
}
