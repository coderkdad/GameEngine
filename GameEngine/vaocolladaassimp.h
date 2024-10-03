#ifndef VAOCOLLADAASSIMP_H
#define VAOCOLLADAASSIMP_H

#include "vaoentity.h" //#
#include "assimplib.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>


class QOpenGLTexture;

struct VAOS {
    QString name = "";
    QOpenGLVertexArrayObject *vao = nullptr;
    uint numVertices = 0;
    std::vector<uint> vecIDBones;
    QCubeLimited limited;
};

class VAOColladaAssimp
{
public:
    VAOColladaAssimp(QOpenGLShaderProgram *shader);
    void setPath(const char *path);
    void setTexture(QString paths);

    QVector<VAOS *> getVAOS();
    QOpenGLTexture *getTexture();
    QCubeLimited *getLimited();
    QVector<QMatrix4x4 *> *getPose();
    QVector<QMatrix4x4 *> *getPose2();
    void setStateAnim(uint state);

private:
    AssimpLib *simp;
    QOpenGLShaderProgram *m_shader;
    QVector<VAOS*> vecVAOS;
    QOpenGLTexture *mTexture;
    QCubeLimited *limitFull;


    void importers();
    void createVAOSimp(std::vector<Vertex *> *veVertices, QOpenGLVertexArrayObject *mVAO, uint *numVertex);
    QCubeLimited *createLimit(std::vector<Vertex *> *vecVertex);

    // Animator
    void setupPose(uint countBone);
    void animatorPose(Animations *animation, Bone *skeleton,
                      float dt, QVector<QMatrix4x4 *> *output, QVector<QMatrix4x4 *> *output2,
                      glm::mat4 &parentTransform, glm::mat4 globalINVTransform);

    QVector<QMatrix4x4 *> vecMatrixPose;
    float counterAnim = 0;

    QMatrix4x4 convertToQMatrix(glm::mat4 *matrix);

    uint mStateAnim = 0;

    //////
    QVector<QMatrix4x4 *> vecMatrixPose2;

};

#endif // VAOCOLLADAASSIMP_H
