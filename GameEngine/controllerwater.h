#ifndef CONTROLLERWATER_H
#define CONTROLLERWATER_H

#include <QObject>
#include <QThread>
#include "modelwater.h"
#include <QOpenGLShaderProgram>
#include "vaoentity.h"

class ControllerWater : public QObject
{
    Q_OBJECT
public:
    explicit ControllerWater(QOpenGLShaderProgram *shader, VAOEntity *vao, QVector3D vecPosFire, int strPriority=1, QObject *parent = nullptr);
    ~ControllerWater();
    ModelWater *getModelWater();


signals:
    void goWork();

public slots:
    void slotInfoBuffer(QVector<VertexData> vecData, QVector<int> vecIndex);


private:
    QThread thread ;
    ModelWater *modelWater;

    QOpenGLShaderProgram *m_shader;
    VAOEntity *m_vao;

};

#endif // CONTROLLERWATER_H
