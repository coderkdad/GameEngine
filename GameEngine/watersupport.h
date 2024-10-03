#ifndef WATERSUPPORT_H
#define WATERSUPPORT_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QMetaType>


class PerlinNoise;
class QTimer;
class VertexData;

class WaterSupport : public QObject
{
    Q_OBJECT
public:
    explicit WaterSupport(QObject *parent = nullptr);
    ~WaterSupport();

    void startThread();

signals:
    void emtInfoBuffer(QVector<VertexData> *vecData, QVector<int> *vecIndex);

private slots:
    void slotReSort();

private:

    int cubeWidth = 512;
    int cubeHeight = 512;

    PerlinNoise *noiseB;
    void createNoise();
    void initsWater();

    QVector<float> vecNoise;

    QTimer *timerWater;
    void reSortNoise();
    QVector<VertexData> *vecData = nullptr;
    QVector<int> *vecIndex = nullptr;

};

#endif // WATERSUPPORT_H
