#ifndef VBOENTITY_H
#define VBOENTITY_H

#include "entity.h"
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
struct QCubeLimited
{
    QVector3D limitedTop;
    QVector3D limitedBottom;
};
struct QTriangle3D
{
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D p1Normal;
    QVector3D p2Normal;
    QVector3D p3Normal;
    QVector2D p1UV;
    QVector2D p2UV;
    QVector2D p3UV;
};
struct EntityInfo
{
    QString name;
    int numVertex;
    QOpenGLVertexArrayObject *vao = nullptr;
    QCubeLimited *limited = nullptr;
    QVector<QCubeLimited*> *partsLimited = nullptr;

//    float **arry;
    QVector< QVector<float> > arry;
    float numSpaceRng;
    int countMaxX;
    int countMaxZ;
};

class VAOEntity // this is Mesh
{
public:
    VAOEntity(QOpenGLShaderProgram *shader);
    ~VAOEntity();
    // Setter
    void setPath(QString paths);
    void setPathVertex(QVector<QVector3D> vecVrtx, QVector<QVector3D> vecNrml, QVector<QVector2D> vecTxtr);
    void setTextureImg(QString texture);
    void setTextureImgEdit(QImage texture);
    // Getter
    QList<EntityInfo *> *getListInfo();
    QOpenGLTexture *getTextureImg();
    QCubeLimited *getLimited();

private:
    // Vertices
    QOpenGLShaderProgram *m_shader = nullptr;
    QList<EntityInfo*> *listEntityInfo = nullptr;
    QOpenGLTexture *m_textureImg = nullptr;
    QCubeLimited *limitFull = nullptr;

    // limited
    void addingVAO(QVector<QTriangle3D> *mVecTriangles, QString name, QVector<QVector3D> vecVertexBase);
    QCubeLimited *createLimit(QVector<QVector3D> vertex, QString n);
    QVector<QCubeLimited *> *createLimitParts(QVector<QVector3D> vertex, QCubeLimited *cube);

    // Erth Ups
    void getUpsErth(QVector<QVector3D> vertex, QCubeLimited *cube, EntityInfo *info);

};

#endif // VBOENTITY_H
