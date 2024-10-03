#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QVector3D>
#include <QMap>
#include <QQuaternion>
#include <QMatrix4x4>

class TransformJoint
{
public:
    TransformJoint(QVector3D pos, QQuaternion rot);
    ~TransformJoint();
    QMatrix4x4 getLocalTransform();
    TransformJoint static interpolate(TransformJoint *frameA, TransformJoint *frameB, float progression);

private:
    QVector3D position;
    QQuaternion rotate;

    QVector3D static interpolate(QVector3D start, QVector3D end, float progression);
};



class KeyFrame
{
public:
    KeyFrame(float timeStamp, QMap<QString, QMatrix4x4 *> *mapjointFrame);
    ~KeyFrame();
    float getTimeStemp();
    QMap<QString, QMatrix4x4 *> getJointFrame();

private:
    float timeStamp;
    QMap<QString, QMatrix4x4 *> *pose;

};

#endif // KEYFRAME_H
