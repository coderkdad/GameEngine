#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QVector3D>
#include <QString>

class QCubeLimited;

class BaseModel
{
public:
    explicit BaseModel();
    ~BaseModel();
    void setModelName(QString nm);
    void setModelPosition(QVector3D pos);
    void setModelRotate(QVector3D rot);
    void setModelScale(QVector3D scal);
    void setModelLimit(QCubeLimited *limit);

    Q_INVOKABLE QString getModelName();
    Q_INVOKABLE QVector3D getModelPosition();
    Q_INVOKABLE QVector3D getModelRotate();
    Q_INVOKABLE QVector3D getModelScale();
    Q_INVOKABLE QCubeLimited *getModelLimited();

private:
    QString name;
    QVector3D modelPos = QVector3D(0,0,0);
    QVector3D modelRot = QVector3D(0,0,0);
    QVector3D modelScal = QVector3D(1,1,1);
    QCubeLimited *modelLimitFull;
    void settingLimited();


};

#endif // BASEMODEL_H
