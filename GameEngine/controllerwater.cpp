#include "controllerwater.h"
#include <QVector3D>

ControllerWater::ControllerWater(QOpenGLShaderProgram *shader, VAOEntity *vao,
                                 QVector3D vecPosFire, int strPriority, QObject *parent)
    : QObject(parent),
      m_shader(shader),
      m_vao(vao)
{
    modelWater = new ModelWater(m_shader, m_vao);
    modelWater->setModelPosition(vecPosFire);
    modelWater->moveToThread(&thread);

    connect(this, &ControllerWater::goWork, modelWater, &ModelWater::slotCreate);
    connect(&thread, &QThread::finished, this, &ControllerWater::deleteLater);
    connect(modelWater, &ModelWater::emtInfoBuffer, this, &ControllerWater::slotInfoBuffer);

    if(strPriority==1){
        thread.start(QThread::HighestPriority);
    } else if(strPriority==2) {
        thread.start(QThread::NormalPriority);
    } else if(strPriority==3){
        thread.start(QThread::LowPriority);
    }
    emit goWork();
}

ControllerWater::~ControllerWater()
{

}

ModelWater *ControllerWater::getModelWater()
{
    return modelWater;
}

void ControllerWater::slotInfoBuffer(QVector<VertexData> vecData, QVector<int> vecIndex)
{
    //
}
