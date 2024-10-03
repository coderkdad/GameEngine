#include "vaocolladaassimp.h"
#include <QTextStream>
#include <QOpenGLTexture>


VAOColladaAssimp::VAOColladaAssimp(QOpenGLShaderProgram *shader) : m_shader(shader)
{
    limitFull = new QCubeLimited;
}

void VAOColladaAssimp::setPath(const char *path)
{
    simp = new AssimpLib(path);

    importers();
    setupPose(simp->countBone());

}

QVector<VAOS *> VAOColladaAssimp::getVAOS()
{
    return vecVAOS;
}

void VAOColladaAssimp::setTexture(QString paths)
{
    mTexture = new QOpenGLTexture(QImage(paths).mirrored());
    mTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
}

QOpenGLTexture *VAOColladaAssimp::getTexture()
{
    return mTexture;
}

QCubeLimited *VAOColladaAssimp::getLimited()
{
    return limitFull;
}

//QCubeLimited VAOColladaAssimp::getLimitedFull()
//{
////    if(state)
//}

void VAOColladaAssimp::setStateAnim(uint state)
{
    if(state > simp->getAnimation().size()-1)
    {
        mStateAnim = 0;
        return;
    }

    mStateAnim = state;
}



void VAOColladaAssimp::importers()
{
    std::vector<Meshs*> msh = simp->getMeshes();

    for(uint i=0; i<msh.size(); ++i)
    {
        QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject;
        uint numVao = 0;

        createVAOSimp(&msh.at(i)->vertices, vao, &numVao);

        VAOS *v = new VAOS;
        v->name = msh.at(i)->name.c_str();
        v->vao = vao;
        v->numVertices = numVao;
        v->vecIDBones = msh.at(i)->vecIDBones;
        v->limited = *createLimit(&msh.at(i)->vertices);
        qDebug()<<"LOLS Collada: "<<v->name<<"\nlimit"<<v->limited.limitedTop<<" | "<<v->limited.limitedBottom;
        vecVAOS.append(v);

//        limitFull = createLimit(&msh.at(i)->vertices); // ##
    }
}

void VAOColladaAssimp::createVAOSimp(std::vector<Vertex*> *veVertices, QOpenGLVertexArrayObject *mVAO, uint *numVertex)
{
    QVector<QVector3D> *vecVertex = new QVector<QVector3D>;
    QVector<QVector3D> *vecNormals = new QVector<QVector3D>;
    QVector<QVector2D> *vecTextureUV = new QVector<QVector2D>;
    QVector<QVector4D> *vecJoints = new QVector<QVector4D>;
    QVector<QVector4D> *vecWeights = new QVector<QVector4D>;

    for(uint i=0; i<veVertices->size(); ++i)
    {
        vecVertex->append(QVector3D(veVertices->at(i)->position.x,veVertices->at(i)->position.y,veVertices->at(i)->position.z));
        vecNormals->append(QVector3D(veVertices->at(i)->normal.x,veVertices->at(i)->normal.y,veVertices->at(i)->normal.z));
        vecTextureUV->append(QVector2D(veVertices->at(i)->uv.x,veVertices->at(i)->uv.y));

        QVector4D vecJointA(veVertices->at(i)->boneID.x, veVertices->at(i)->boneID.y,
                            veVertices->at(i)->boneID.z, veVertices->at(i)->boneID.w);
        QVector4D vecJointWeight(veVertices->at(i)->boneWeight.x, veVertices->at(i)->boneWeight.y,
                                 veVertices->at(i)->boneWeight.z, veVertices->at(i)->boneWeight.w);

        vecJoints->append(vecJointA);
        vecWeights->append(vecJointWeight);
    }

    mVAO->create();
    mVAO->bind();
    //Vertex
    QOpenGLBuffer *vboVertex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertex->create();
    vboVertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertex->bind();
    vboVertex->allocate(vecVertex->constData(), vecVertex->count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_vertex",0);
    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // Normal
    QOpenGLBuffer *vboNormal = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormal->create();
    vboNormal->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormal->bind();
    vboNormal->allocate(vecNormals->constData(), vecNormals->count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_normal",1);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(1,GL_FLOAT, 0, 3);
    // Texture
    QOpenGLBuffer *vboTexture = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexture->create();
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->bind();
    vboTexture->allocate(vecTextureUV->constData(), vecTextureUV->count() *2* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_texcoord", 2);
    m_shader->enableAttributeArray(2);
    m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2);

    //Joint
    QOpenGLBuffer *vboJoint = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboJoint->create();
    vboJoint->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboJoint->bind();
    vboJoint->allocate(vecJoints->constData(), vecJoints->count() *4* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_joints",3);
    m_shader->enableAttributeArray(3);
    m_shader->setAttributeBuffer(3, GL_FLOAT, 0, 4);
    //Weight
    QOpenGLBuffer *vboWeight = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboWeight->create();
    vboWeight->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboWeight->bind();
    vboWeight->allocate(vecWeights->constData(), vecWeights->count() *4* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_Weights",4);
    m_shader->enableAttributeArray(4);
    m_shader->setAttributeBuffer(4, GL_FLOAT, 0, 4);

    mVAO->release();
    *numVertex = vecVertex->count();
}

QCubeLimited *VAOColladaAssimp::createLimit(std::vector<Vertex*> *vecVertex)
{
    QCubeLimited *limitEntity = new QCubeLimited;
    limitEntity->limitedTop = QVector3D(0,0,0);
    limitEntity->limitedBottom = QVector3D(0,0,0);

    for(uint i=0; i<vecVertex->size(); ++i)
    {
        float vertexX=vecVertex->at(i)->position.x;
        float vertexY=vecVertex->at(i)->position.y;
        float vertexZ=vecVertex->at(i)->position.z;

        if(i==0) // first Value
        {
            limitEntity->limitedTop = QVector3D(vertexX, vertexY, vertexZ);
            limitEntity->limitedBottom = QVector3D(vertexX, vertexY, vertexZ);
        } else {
            limitEntity->limitedTop = QVector3D(fmax(vertexX, limitEntity->limitedTop.x()),
                                               fmax(vertexY, limitEntity->limitedTop.y()),
                                               fmax(vertexZ, limitEntity->limitedTop.z()) );

            limitEntity->limitedBottom = QVector3D(fmin(vertexX, limitEntity->limitedBottom.x()),
                                                  fmin(vertexY, limitEntity->limitedBottom.y()),
                                                  fmin(vertexZ, limitEntity->limitedBottom.z()) );
        }
    }

    // checked Limit Full
    {
        limitFull->limitedTop = QVector3D(fmax(limitEntity->limitedTop.x(), limitFull->limitedTop.x()),
                                          fmax(limitEntity->limitedTop.y(), limitFull->limitedTop.y()),
                                          fmax(limitEntity->limitedTop.z(), limitFull->limitedTop.z()));

        limitFull->limitedBottom = QVector3D(fmin(limitEntity->limitedBottom.x(), limitFull->limitedBottom.x()),
                                             fmin(limitEntity->limitedBottom.y(), limitFull->limitedBottom.y()),
                                             fmin(limitEntity->limitedBottom.z(), limitFull->limitedBottom.z()));
    }

    return limitEntity;
}


void VAOColladaAssimp::setupPose(uint countBone)
{
    for(uint i=0; i<countBone; ++i)
    {
        vecMatrixPose.append(new QMatrix4x4());
        vecMatrixPose2.append(new QMatrix4x4());
    }
}

QVector<QMatrix4x4 *> *VAOColladaAssimp::getPose()
{
    glm::mat4 identity(1.0f);

    Animations *anim = simp->getAnimation().at(mStateAnim);

    animatorPose(anim, simp->getBone(),
                 0, &vecMatrixPose, &vecMatrixPose2,
                 identity, simp->getGlobalMatrixINV());

    if(mStateAnim == 0)
    {
        counterAnim += 5; // Go
    }
    else if(mStateAnim == 1)
    {
        counterAnim += 1; // standing
    }
    else if(mStateAnim == 2)
    {
        counterAnim = 0; // sedown
    }
    else if (mStateAnim == 3)
    {
        counterAnim += 5; // GoSedown
    }
    else if(mStateAnim == 4)
    {
        counterAnim = 0; // JumpUp
    }

    if(counterAnim > 249)
        counterAnim=0;

    return &vecMatrixPose;
}

QVector<QMatrix4x4 *> *VAOColladaAssimp::getPose2()
{
    return &vecMatrixPose2;
}

void VAOColladaAssimp::animatorPose(Animations *animation, Bone *skeleton,
                                    float dt, QVector<QMatrix4x4 *> *output, QVector<QMatrix4x4 *> *output2,
                                    glm::mat4 &parentTransform, glm::mat4 globalINVTransform)
{

    BoneTransformTrack& btt = animation->boneTransforms[skeleton->name.c_str()];
//    qDebug()<<"XXX: "<<skeleton->name.c_str();

    glm::vec3 position(1.0f);
    glm::vec3 positionAffter(1.0f);
    glm::quat rotation(0,0,0,0);
    glm::vec3 scale(1.0);
    glm::mat4 ofset(1.0f);
    if(btt.position.size() == 0)
    {
    }
    else
    {
        uint IDs = counterAnim/1;
        position = btt.position[ IDs ];
        rotation = btt.rotation[ IDs ];
        scale = btt.scaling[ IDs ];
        ofset = skeleton->offset;
    }

    glm::mat4 positionMat = glm::mat4(1.0f);
    glm::mat4 scaleMat = glm::mat4(1.0f);


    // calculate localTransform
    positionMat = glm::translate(positionMat, position);
    glm::mat4 rotationMat = glm::toMat4(rotation);
    scaleMat = glm::scale(scaleMat, scale);


    // this is rotate free item
    if(skeleton->name == "Armature_Bone_001")
    {
        glm::vec3 rots(0,1,0);
        glm::quat qut(45.0f, rots);
        rotationMat = glm::rotate(rotationMat, 45.0f, glm::vec3(1,0,0));
    }

    glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
    glm::mat4 globalTransform = parentTransform * localTransform;
    glm::mat4 resultAll = globalINVTransform * globalTransform * ofset;

    glm::mat4 resultAll2 = glm::mat4(1.0f);
    glm::mat4 posAffterMat(1.0);
    if(skeleton->name == "Armature_BoneArmBottomRXXXXXXXX")
    {
        for (Bone *child : skeleton->children) {
            BoneTransformTrack& bttt = animation->boneTransforms[child->name.c_str()];
            uint IDs = counterAnim/1;
            glm::vec3 posAftter(1.0f);
            posAftter = bttt.position[ IDs ];
            posAffterMat = glm::translate(posAffterMat, glm::vec3(1.0));
        }
        glm::mat4 localTransform = (positionMat*posAffterMat) * rotationMat * scaleMat;
        glm::mat4 globalTransform = parentTransform * localTransform;
        resultAll2 = globalTransform;

//        resultAll2 = globalINVTransform * globalTransform * ofset;
    }
    else
    {
        resultAll2 = globalTransform;
    }


    *output->at(skeleton->id) = convertToQMatrix(&resultAll);
    *output2->at(skeleton->id) = convertToQMatrix(&resultAll2);

    //update values for children bones
    for (Bone *child : skeleton->children) {
        animatorPose(animation, child, 0, output, output2, globalTransform, globalINVTransform);
    }

}

QMatrix4x4 VAOColladaAssimp::convertToQMatrix(glm::mat4 *matrix)
{
    float *a = glm::value_ptr(*matrix);

    QMatrix4x4 q_mtrx(a[0], a[1], a[2], a[3],
                      a[4], a[5], a[6], a[7],
                      a[8], a[9], a[10], a[11],
                      a[12], a[13], a[14], a[15]);

    return q_mtrx;
}
