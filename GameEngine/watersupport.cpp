#include "watersupport.h"
#include "PerlinNoise.h"
#include <QVector3D>
#include <QVector2D>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "modelwater.h"


WaterSupport::WaterSupport(QObject *parent) : QObject(parent)
{
    vecIndex = new QVector<int>;
    vecData = new QVector<VertexData>;
}

WaterSupport::~WaterSupport()
{
    delete noiseB;

    timerWater->stop();
}

void WaterSupport::startThread()
{
    qDebug("start Thread :)");
    noiseB = new PerlinNoise;
    createNoise();
//    initsWater();

    timerWater = new QTimer(this);
    connect(timerWater, SIGNAL(timeout()), this, SLOT(slotReSort()));
    timerWater->start(10000);
}

void WaterSupport::slotReSort()
{
    reSortNoise();
    initsWater();
}

void WaterSupport::reSortNoise()
{
    qDebug("resort");
    QVector<float> vecRe;
    for(int i=0; i<cubeWidth+1; ++i)
    {
//        vecRe.append(vecNoise.takeAt(0));
        vecNoise.append(vecNoise.takeAt(0));
    }
}

void WaterSupport::createNoise()
{
    int imageWidth = cubeWidth+1;
    int imageHeight = cubeHeight+1;
    float *noiseMap = new float[imageWidth * imageHeight];
//    QVector<float> vecNoise ;

//    ValueNoise noise;
    float frequency = 1; // مقدار الفرق بين النقاط في القيمة
    float amplitude = 1;
    float zz = 0.5f;
    for (int j = 0; j < imageHeight; ++j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            // generate a float in the range [0:1]
//            float vNoise = (noiseA->eval(QVector2D(i, j)));
//            float vNoise = (noiseA->eval(QVector2D(i, j) * frequency)*amplitude); // rang noise [-1, 1]
            float vNoise = (noiseB->noise(i * frequency, j * frequency, zz)*amplitude);
            noiseMap[j * imageWidth + i] = ((vNoise));
            vecNoise.append((vNoise));
//            vecNoise.append(noises->eval(QVector2D(i, j) * frequency));
//            qDebug()<<"Noise: "<<noiseMap[j * imageWidth + i];
        }
    }

    // File
    QFile file("/home/aloool14/noise7.ppm");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

//    qDebug("======================= \n");

    QTextStream out(&file);
    out << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";

    for(int h=imageHeight-1; h>=0; --h)
//    for(int h=0; h<imageHeight; ++h)
    {
//        for(int w=0; w<imageWidth; ++w)
        for(int w=imageWidth-1; w>=0; --w)
        {
            float colr = ( vecNoise.at(h*(imageWidth)+(w)) ) ;
//            char n = static_cast<char>(vecNoise.at(h*(imageWidth)+w) * 255);
//            qDebug()<<"Color: "<<colr;
            char n = '0';
            char nn = static_cast<char>(colr * 255);
            out << n << n << nn;
        }
//        qDebug("\n \n");
    }
//    qDebug("========= END COLOR =========");
    out.flush();
    file.close();
    //    qDebug("Fish File PPM");
}

void WaterSupport::initsWater()
{
    vecData->clear();
    vecIndex->clear();

    int countWidth = cubeWidth;
    int countHeight = cubeHeight;
    float invCountWidth = 1.0f / countWidth;
    float invCountHeight = 1.0f / countHeight;
    float spaces = 1;
//    QVector<VertexData> vecData;

    for(int h=0; h<=countHeight; ++h)
    {
        for(int w=0; w<=countWidth; ++w)
        {
            float n = (vecNoise.at(h*(countWidth+1)+w) + 1)*0.5;
            vecData->append(VertexData(QVector3D(w/2,h/2,n), QVector2D(w*invCountWidth, h*invCountHeight)));
        }
    }

    ////
//    QVector<int> vecIndex;
    QVector<float> vecImg;
    for(int h=0; h<countHeight; ++h)
    {
        for(int w=0; w<countWidth; ++w)
        {
            vecIndex->append(int((h+1)*(countWidth+1)+w)); // top left
            vecIndex->append(int((h)*(countWidth+1)+w)); // bottom left
            vecIndex->append(int((h+1)*(countWidth+1)+(w+1))); // top right

            vecIndex->append(int((h+1)*(countWidth+1)+(w+1)));// top right
            vecIndex->append(int((h)*(countWidth+1)+w));// bottom left
            vecIndex->append(int((h)*(countWidth+1)+(w+1)));// bottom right
        }
    }


    qDebug()<<" L "<<vecData->size()<<" ~ "<<vecData->at(1).position;
    emit emtInfoBuffer(vecData, vecIndex);
}
