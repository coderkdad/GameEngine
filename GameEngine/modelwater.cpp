#include "modelwater.h"
#include <QMatrix4x4>
#include "controllercollision.h"
#include "vaoentity.h"
#include "entity.h"
#include "PerlinNoise.h"
#include <QFile>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QTimer>
#include "watersupport.h"

ModelWater::ModelWater(QOpenGLShaderProgram *shader, VAOEntity *vao, QObject *parent) : QObject(parent),
    m_shader(shader),
    m_vao(vao),
    indexBuf(QOpenGLBuffer::IndexBuffer), m_textures(0)
{
    qRegisterMetaType<VertexData>();
    qRegisterMetaType< QVector<VertexData> >();
}

ModelWater::~ModelWater()
{
    thread.quit();
    thread.wait();
    thread.deleteLater();
    waterSupporting->deleteLater();

    delete m_entityWater;

    m_textures->destroy();
    delete m_textures;

}

void ModelWater::createEntity()
{
    m_entityWater = new Entity;
    m_entityWater->setPosition(getModelPosition());


//    noiseB = new PerlinNoise;
//    createNoise();
    createThread();
//    initWater(); // This false
    initTexture(); // This false

//    timerWater = new QTimer;
//    connect(timerWater, SIGNAL(timeout()), this, SLOT(slotWater()));
//    timerWater->start(50);

}

//void ModelWater::setModelPosition(QVector3D pos)
//{
//    modelPos = pos;
//}
//void ModelWater::setModelRotate(QVector3D rot)
//{
//    modelRot = rot;
//}
//void ModelWater::setModelScale(QVector3D scal)
//{
//    modelScal = scal;
//}
//QVector3D ModelWater::getModelPosition()
//{
//    return modelPos;
//}
//QVector3D ModelWater::getModelRotate()
//{
//    return modelRot;
//}
//QVector3D ModelWater::getModelScale()
//{
//    return modelScal;
//}
void ModelWater::setControl(ControllerCollision *controlCol)
{
    m_controlCol = controlCol;
}

void ModelWater::slotInfoBuffer(QVector<VertexData> *vecData, QVector<int> *vecIndex)
{
    qDebug("emt Supportes");
//    vecDatas = vecData;
    if(vecDatas == nullptr)
    { vecDatas = new QVector<VertexData>(*vecData); }
    else
    { *vecDatas = *vecData; }

//    vecIndexs = vecIndex;
    if(vecIndexs == nullptr)
    { vecIndexs = new QVector<int>(*vecIndex); }
    else { *vecIndexs = *vecIndex; }
    qDebug()<<"G "<<vecDatas->size() << " ~ "<< vecData->at(1).position;
    initWater();

    // OUT Class

    {
    //void ModelWater::createNoise()
    //{

    //    int imageWidth = cubeWidth+1;
    //    int imageHeight = cubeHeight+1;
    //    float *noiseMap = new float[imageWidth * imageHeight];
    ////    QVector<float> vecNoise ;

    ////    ValueNoise noise;
    //    float frequency = 1; // مقدار الفرق بين النقاط في القيمة
    //    float amplitude = 1;
    //    float zz = 0.5f;
    //    for (int j = 0; j < imageHeight; ++j)
    //    {
    //        for (int i = 0; i < imageWidth; ++i)
    //        {
    //            // generate a float in the range [0:1]
    ////            float vNoise = (noiseA->eval(QVector2D(i, j)));
    ////            float vNoise = (noiseA->eval(QVector2D(i, j) * frequency)*amplitude); // rang noise [-1, 1]
    //            float vNoise = (noiseB->noise(i * frequency, j * frequency, zz)*amplitude);
    //            noiseMap[j * imageWidth + i] = ((vNoise));
    //            vecNoise.append((vNoise));
    ////            vecNoise.append(noises->eval(QVector2D(i, j) * frequency));
    ////            qDebug()<<"Noise: "<<noiseMap[j * imageWidth + i];
    //        }
    //    }

    //    // File
    //    QFile file("/home/aloool14/noise7.ppm");
    //    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    //        return;

    ////    qDebug("======================= \n");

    //    QTextStream out(&file);
    //    out << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";

    //    for(int h=imageHeight-1; h>=0; --h)
    ////    for(int h=0; h<imageHeight; ++h)
    //    {
    ////        for(int w=0; w<imageWidth; ++w)
    //        for(int w=imageWidth-1; w>=0; --w)
    //        {
    //            float colr = ( vecNoise.at(h*(imageWidth)+(w)) ) ;
    ////            char n = static_cast<char>(vecNoise.at(h*(imageWidth)+w) * 255);
    ////            qDebug()<<"Color: "<<colr;
    //            char n = '0';
    //            char nn = static_cast<char>(colr * 255);
    //            out << n << n << nn;
    //        }
    ////        qDebug("\n \n");
    //    }
    ////    qDebug("========= END COLOR =========");
    //    out.flush();
    //    file.close();
    //    //    qDebug("Fish File PPM");
    //}
    }
}


void ModelWater::initWater()
{
    {

        //    int countWidth = cubeWidth;
        //    int countHeight = cubeHeight;
        //    float invCountWidth = 1.0f / countWidth;
        //    float invCountHeight = 1.0f / countHeight;
        //    float spaces = 1;
        //    QVector<VertexData> vecData;
        ////    VertexData vecData[] = ;

        //    for(int h=0; h<=countHeight; ++h)
        //    {
        //        for(int w=0; w<=countWidth; ++w)
        //        {
        //            float n = (vecNoise.at(h*(countWidth+1)+w) + 1)*0.5;
        //            vecData.append(VertexData(QVector3D(w/2,h/2,n), QVector2D(w*invCountWidth, h*invCountHeight)));
        //        }
        //    }

        //    ////
        //    QVector<int> vecIndex;
        //    QVector<float> vecImg;
        //    for(int h=0; h<countHeight; ++h)
        //    {
        //        for(int w=0; w<countWidth; ++w)
        //        {
        //            vecIndex.append(int((h+1)*(countWidth+1)+w)); // top left
        //            vecIndex.append(int((h)*(countWidth+1)+w)); // bottom left
        //            vecIndex.append(int((h+1)*(countWidth+1)+(w+1))); // top right

        //            vecIndex.append(int((h+1)*(countWidth+1)+(w+1)));// top right
        //            vecIndex.append(int((h)*(countWidth+1)+w));// bottom left
        //            vecIndex.append(int((h)*(countWidth+1)+(w+1)));// bottom right
        //        }
        //    }


        //    emit emtInfoBuffer(&vecData, &vecIndex);
    }

    ///////////////////////
    if(arrayBuf.isCreated()) {qDebug("Ys Crted"); }
    else { qDebug("Not Crted"); arrayBuf.create();}


    arrayBuf.bind();
    arrayBuf.allocate(vecDatas->constData(), vecDatas->count() * sizeof(VertexData));

    if(indexBuf.isCreated()) {}
    else {indexBuf.create();}


    indexBuf.bind();
    qDebug("goooood arry 2");
    indexBuf.allocate(&vecIndexs, vecIndexs->count() * sizeof(int));
    qDebug("goooood indx");
}

void ModelWater::initTexture()
{
    m_textures = new QOpenGLTexture(QImage("/home/aloool14/Pictures/Textures/texSkBx.jpg").mirrored());
    m_textures->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textures->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures->setWrapMode(QOpenGLTexture::Repeat);
}

void ModelWater::draw(QMatrix4x4 matrixPro, QMatrix4x4 matrixCam)
{
    float transparent=1.0f;
    m_textures->bind();

    QMatrix4x4 matrixEntity;
    matrixEntity.translate(getModelPosition());
    matrixEntity.rotate(0, 0,1,0);
    matrixEntity.rotate(0, 0,0,1);
    matrixEntity.rotate(-90, 1,0,0);

    m_shader->bind();
    m_shader->setUniformValue("pMatrix", matrixPro);
    m_shader->setUniformValue("mvMatrix", matrixCam * matrixEntity);
    m_shader->setUniformValue("u_lightPosition", QVector4D(0.0, 2.0, 2.0, 1.0));
    m_shader->setUniformValue("u_lightPower", 1.0f);
    m_shader->setUniformValue("fragFlag", 1.0f);
    m_shader->setUniformValue("texturTransparency", transparent);

    if(arrayBuf.isCreated() && indexBuf.isCreated())
        drawWater(m_shader);

}

void ModelWater::drawWater(QOpenGLShaderProgram *shader)
{
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = shader->attributeLocation("a_vertex");
    shader->enableAttributeArray(vertexLocation);
    shader->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = shader->attributeLocation("a_texcoord");
    shader->enableAttributeArray(texcoordLocation);
    shader->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indexBuf.size(), GL_UNSIGNED_INT, 0);
    shader->release();
    qDebug("Drw");
}

void ModelWater::createThread()
{
    qDebug("create Threads");
    waterSupporting = new WaterSupport;
    waterSupporting->moveToThread(&thread);

    connect(this, &ModelWater::goWork, waterSupporting, &WaterSupport::startThread);
//    connect(this, SIGNAL(goWork()), waterSupporting, SLOT(startThread()));
    connect(waterSupporting, &WaterSupport::emtInfoBuffer, this, &ModelWater::slotInfoBuffer);

    thread.start(QThread::NormalPriority);

    emit goWork();
}
//void ModelWater::slotWater()
//{
//    reSortNoise();
//    initWater();
//}
//void ModelWater::reSortNoise()
//{
//    qDebug("resort");
//    QVector<float> vecRe;
//    for(int i=0; i<cubeWidth+1; ++i)
//    {
////        vecRe.append(vecNoise.takeAt(0));
//        vecNoise.append(vecNoise.takeAt(0));
//    }
//}
