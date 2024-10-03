#ifndef MODELWATER_H
#define MODELWATER_H

#include <QOpenGLFunctions>
#include <QObject>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QThread>
#include <QMetaType>
#include "basemodel.h"

class ControllerCollision;
class QOpenGLShaderProgram;
class VAOEntity;
class Entity;
class PerlinNoise;
class QOpenGLTexture;
class QTimer;
//class VertexData ;
class WaterSupport;

struct VertexData
{
    VertexData()
    {
    }
    VertexData(QVector3D p, QVector2D t) :
        position(p), texCoord(t)
    {
    }

    QVector3D position;
    QVector2D texCoord;
};

class ModelWater : public QObject, public BaseModel
{
    Q_OBJECT
public:
    explicit ModelWater(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent = nullptr);
    ~ModelWater();

    void createEntity();
//    void setModelPosition(QVector3D pos);
//    void setModelRotate(QVector3D rot);
//    void setModelScale(QVector3D scal);
//    QVector3D getModelPosition();
//    QVector3D getModelRotate();
//    QVector3D getModelScale();

    void draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam);
    void setControl(ControllerCollision *controlCol);

signals:
    void goWork();

public slots:
    void slotInfoBuffer(QVector<VertexData> *vecData, QVector<int> *vecIndex);

private slots:
//    void slotWater();

private:
    // Thread
    QThread thread;
    void createThread();
    WaterSupport *waterSupporting;


    QOpenGLShaderProgram *m_shader;
    VAOEntity *m_vao;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLTexture *m_textures;

//    QVector3D modelPos = QVector3D(0,0,0);
//    QVector3D modelRot = QVector3D(0,0,0);
//    QVector3D modelScal = QVector3D(1,1,1);
    ControllerCollision *m_controlCol;

    int cubeWidth = 512;
    int cubeHeight = 512;

    // Entitys
    Entity *m_entityWater;
//    PerlinNoise *noiseB;
//    void createNoise();
    void initWater();
    void initTexture();
    void drawWater(QOpenGLShaderProgram *shader);
//    QVector<float> vecNoise ;

//    QTimer *timerWater;
//    void reSortNoise();


    QVector<VertexData> *vecDatas = nullptr;
    QVector<int> *vecIndexs = nullptr;


};
Q_DECLARE_METATYPE(VertexData);

#endif // MODELWATER_H
