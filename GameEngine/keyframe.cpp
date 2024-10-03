#include "keyframe.h"
#include <QMap>

TransformJoint::TransformJoint(QVector3D pos, QQuaternion rot) :
    position(pos), rotate(rot)
{

}

TransformJoint::~TransformJoint()
{

}

QMatrix4x4 TransformJoint::getLocalTransform()
{
    QMatrix4x4 mat;
    mat.translate(position);
    mat.rotate(rotate); //
    return mat;
}

TransformJoint TransformJoint::interpolate(TransformJoint *frameA, TransformJoint *frameB, float progression)
{
    QVector3D poss = interpolate(frameA->position, frameB->position, progression);
    QQuaternion rott = QQuaternion::nlerp(frameA->rotate, frameB->rotate, progression);

    return TransformJoint(poss, rott);
}

QVector3D TransformJoint::interpolate(QVector3D start, QVector3D end, float progression)
{
    float x = start.x() + (end.x() - start.x()) * progression;
    float y = start.y() + (end.y() - start.y()) * progression;
    float z = start.z() + (end.z() - start.z()) * progression;
    return QVector3D(x,y,z);
}

/// ====================================== ///

KeyFrame::KeyFrame(float timeStamp, QMap<QString, QMatrix4x4 *> *mapjointFrame) :
    timeStamp(timeStamp), pose(mapjointFrame)
{

}

KeyFrame::~KeyFrame()
{

}

float KeyFrame::getTimeStemp()
{
    return timeStamp;
}

QMap<QString, QMatrix4x4 *> KeyFrame::getJointFrame()
{
    return *pose; // return refrence
}

