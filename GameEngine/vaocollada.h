#ifndef VAOCOLLADA_H
#define VAOCOLLADA_H

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QTransform>


#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "assimplib.h"


class QOpenGLVertexArrayObject;
class QOpenGLShaderProgram;
class QOpenGLTexture;
class KeyFrame;
class TransformJoint;

struct VerticesGroup // Mesh
{
    QVector3D position;
    QVector3D normal;
    QVector2D texture;

    QVector3D jointID;
    QVector3D weight;
    //
    QList<QVector2D> *vecJW;
};

struct JointInfo // Joint
{
    int index;
    QString name;
    QMatrix4x4 *matrixShape;
    QMatrix4x4 *matrixINV;
    QMatrix4x4 *matrixJoint;
    QList<int> *listChild;
};

class Joint // Joint
{
public:
    Joint() {};
    void setIndex(int index) { m_index=index; }
    void setName(QString name) { m_name=name; }
    void setParent(Joint *parent) { m_parent=parent; }
    void addChilds(QList<Joint*> *listIndex) { m_listChild=listIndex; }
    void setMatrixLocal(QMatrix4x4 *matrix) { m_matrixLocal=matrix; }
    void setMatrixINV(QMatrix4x4 *matrix) { m_matrixINV=matrix; }
    void setMatrixShape(QMatrix4x4 *matrix) {  m_matrixShape=matrix; }
    void setMatrixAnimation(QMatrix4x4 matrixAnimation) { m_matrixAnimation = matrixAnimation; }
    void setStateMove(bool state) { m_state = state; }

    int getIndex() { return m_index; }
    QString getName() { return m_name; }
    Joint *getParent() { return m_parent; }
    QList<Joint*> *getChild() { return m_listChild; }
    QMatrix4x4 *getMatrixLocal() { return m_matrixLocal; }
    QMatrix4x4 *getMatrixINV() { return m_matrixINV; }
    QMatrix4x4 *getMatrixShape() { return m_matrixShape; }
    QMatrix4x4 getMatrixAnimation() { return m_matrixAnimation;}
    bool isMove() { return m_state;}

    QMatrix4x4 getMatrixLocalFinal()
    {
        QMatrix4x4 matrixFinal;
        QMatrix4x4 invMatrix;
        if(m_parent == NULL)
        {
            matrixFinal = *getMatrixLocal();
            invMatrix = matrixFinal.inverted();
        }
        else
        {
            matrixFinal = m_parent->getMatrixLocalFinal() * (*getMatrixLocal() * getMatrixAnimation());
            invMatrix =  (m_parent->getMatrixLocalFinal() * *getMatrixLocal()).inverted();
        }

//        *m_matrixINV = invMatrix;
        return matrixFinal;
    }

    QMatrix4x4 getMatrixFinal()
    {
        QMatrix4x4 mat;
        if(false)
        {
            mat = m_matrixAnimation;
        }
        else
        {
            mat= (getMatrixLocalFinal() ) * *m_matrixINV;
        }
        return mat;
    }


private:
    int m_index;
    QString m_name;
    Joint *m_parent = NULL;
    QList<Joint*> *m_listChild;
    QMatrix4x4 *m_matrixLocal;
    QMatrix4x4 *m_matrixINV;
    QMatrix4x4 *m_matrixShape;
    QMatrix4x4 m_matrixAnimation;
    bool m_state = false;
};


class VAOCollada
{
public:
    VAOCollada(QOpenGLShaderProgram *shader);

    void setPath(QString paths);
    void setTexture(QString paths);

    QOpenGLVertexArrayObject *getVAO();
    QOpenGLTexture *getTexture();
    int getCountV();
    QVector<VerticesGroup *> *getListGroub();
    QVector<Joint *> *getListJoints();

    QVector<KeyFrame *> *getListKeyFrame();
    float getLengthFrames();


private:
    QOpenGLShaderProgram *m_shader;
    QOpenGLVertexArrayObject *m_VAO;
    QOpenGLTexture *m_textureImg;
    int m_Count_V;

    void pushLibGeometries(QString line);
    void pushLibControllers(QString line);
    void pushLibVisualScenes(QString line);
    void pushLibAnimation(QString line);
    void popLibAnimation();
    void readAllFaces();
    QVector3D nearListJW(QVector<QList<QVector2D> *> *vecList2D);
    void createVAO();
    void createVAOSimp(std::vector<Vertex *> &vecFaces);

    QString ID_Geometrie;
    QString ID_Controller;

    QVector<VerticesGroup*> *vecGroups;
    QVector<QVector3D> *vecVertex;
    QVector<QVector3D> *vecNormal;
    QVector<QVector2D> *vecTexture;
    QStringList *listFaces;

    QVector<Joint*> *vecJoints;
    QStringList *listWeights;
    QStringList *listVCount;
    QVector< QList<QVector2D> *> *vecListJW;

    QMatrix4x4 *m_matrixBShape;
    QList<QMatrix4x4*> listMatrixLocal;

    QOpenGLVertexArrayObject *vao;

    QStringList *listNames;
    QVector< QList<Joint*> *> *vecListChild;

    // Key Frame Animation
    QStringList *listTimeFeyFrames;

    QMap<QString, QMatrix4x4> *mapKeyFrames;
    QMap<float, QMap<QString, QMatrix4x4> *> *mapAllFrames;

//    QMap<QString, TransformJoint*> *mapTransform;
    QVector<QMap<QString, TransformJoint*> *> *vecMapTransform;
    QVector<KeyFrame*> *vecKeyFrames;

    QVector<QString> vec_NamesTrans;
    QVector< QVector<TransformJoint*> *> vec_Transform;
    QVector< QVector<QMatrix4x4*> *> vec_MatAnim;

    // Test Assimp
    void importers();
    Assimp::Importer importer;



    void funReadFile(QVector<QVector3D> &vecs);
};



#endif // VAOCOLLADA_H
