#include "vaocollada.h"
#include <QFile>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "keyframe.h"
#include "assimplib.h"

#include <QFile>
#include <QTextStream>

VAOCollada::VAOCollada(QOpenGLShaderProgram *shader) : m_shader(shader)
{
    vecGroups = new QVector<VerticesGroup*>;
    vecVertex = new QVector<QVector3D>;
    vecNormal = new QVector<QVector3D>;
    vecTexture = new QVector<QVector2D>;
    listFaces = new QStringList;

    vecJoints = new QVector<Joint*>;
    listWeights = new QStringList;
    listVCount = new QStringList;
    vecListJW = new QVector< QList<QVector2D> *>;

    listNames = new QStringList;
    vecListChild = new QVector< QList<Joint*> *>;
    m_matrixBShape = new QMatrix4x4;

    //Anim
    listTimeFeyFrames = new QStringList;
//    mapTransform = new QMap<QString, TransformJoint*>;
    vecMapTransform = new QVector<QMap<QString, TransformJoint*> *>;
    vecKeyFrames = new QVector<KeyFrame*>;

    importers();
}

void VAOCollada::funReadFile(QVector<QVector3D> &vecs)
{
    QFile files("/home/aloool14/Desktop/readVecs2.txt");
//    QFile files1("/home/aloool14/Desktop/readVecs1.txt");//god

    if (!files.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }
//    if (!files1.open(QIODevice::ReadWrite | QIODevice::Text))
//    {
//        qDebug("Its Not 2");
//        return;
//    }

    QTextStream out(&files);
//    QTextStream out1(&files1);

    for(int i=0; i<vecs.count(); ++i)
    {
        out <<QString::number(vecs.at(i).x())<<" "
            <<QString::number(vecs.at(i).y())<<" "
            <<QString::number(vecs.at(i).z())<<"\n";
    }

//    if(out.readAll() == out1.readAll())
//    {
//        qDebug("Good Read All Files");
//    }

}

void VAOCollada::setPath(QString paths)
{
    QFile file(paths);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Not Open File obj:-\n"<<paths;
        return;
    }

    /// ============== Start Read =============== ///

    int check = 0;
    while (!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        if(line == "<library_geometries>")
        {
            check = 1;
        }
        else if(line == "</library_geometries>")
        {
            check = 0;
        }
        else if(line == "<library_controllers>")
        {
            check = 2;

        }
        else if(line == "</library_controllers>")
        {
            check = 0;
        }
        else if(line == "<library_visual_scenes>")
        {
            check = 3;
        }
        else if(line == "</library_visual_scenes>")
        {
            check = 0;
        }
        else if(line == "<library_animations>")
        {
            check = 4;
        }
        else if(line == "</library_animations>")
        {
            popLibAnimation();
            check = 0;
        }



        if(check == 1)
        {
            pushLibGeometries(line);
        }
        else if(check == 2)
        {
            pushLibControllers(line);
        }
        else if(check == 3)
        {
            pushLibVisualScenes(line);
        }
        else if (check == 4)
        {
            pushLibAnimation(line);
        }
    }

}

void VAOCollada::pushLibGeometries(QString line)
{
    if(line.startsWith("<geometry id="))
    {
        ID_Geometrie = line.split(" ").at(1);
    }
    else if(line.startsWith("<p>"))
    {
        *listFaces = line.split(">").at(1).split("<").at(0).trimmed().split(" ");
    }
    else
    {
        if(line.startsWith("<float_array"))
        {
            if(line.contains("-positions-array"))
            {
                QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

                for(int i=0; i<listNum.count(); i+=3)
                {
                    QVector3D vecPos(listNum.at(i+0).toFloat(),
                                     listNum.at(i+1).toFloat(),
                                     listNum.at(i+2).toFloat());
                    vecVertex->append(vecPos);
                }
            }
            else if(line.contains("-normals-array"))
            {
                QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

                for(int i=0; i<listNum.count(); i+=3)
                {
                    QVector3D vecNorm(listNum.at(i+0).toFloat(),
                                     listNum.at(i+1).toFloat(),
                                     listNum.at(i+2).toFloat());
                    vecNormal->append(vecNorm);
                }
            }
            else if(line.contains("-map-0-array"))
            {
                QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

                for(int i=0; i<listNum.count(); i+=2)
                {
                    QVector2D vecTex(listNum.at(i+0).toFloat(),
                                     listNum.at(i+1).toFloat());
                    vecTexture->append(vecTex);
                }
            }

        }
    }
}

void VAOCollada::pushLibControllers(QString line)
{
    if(line.startsWith("<controller id="))
    {
        ID_Controller = line.split(" ").at(1);
    }
    else if(line.startsWith("<bind_shape_matrix"))
    {
        QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

        for(int i=0; i<listNum.count(); i+=4)
        {
            m_matrixBShape->setRow(i/4, QVector4D(listNum.at(i+0).toFloat(),
                                    listNum.at(i+1).toFloat(),
                                    listNum.at(i+2).toFloat(),
                                    listNum.at(i+3).toFloat()));
            m_matrixBShape->setToIdentity();
        }

    }
    else if(line.startsWith("<Name_array") && line.contains("-joints-array"))
    {
        QStringList listNameJoint = line.split(">").at(1).split("<").at(0).trimmed().split(" ");
        for(int i=0; i<listNameJoint.count(); ++i)
        {
            Joint *jont2 = new Joint;
            jont2->setIndex(i);
            jont2->setName(listNameJoint.at(i));
            jont2->setMatrixShape(m_matrixBShape);
            vecJoints->append(jont2);
        }

    }
    else if(line.startsWith("<float_array")) // Another Equal this condition
    {
        if(line.contains("-weights-array"))
        {
            *listWeights = line.split(">").at(1).split("<").at(0).trimmed().split(" ");
        }
        else if(line.contains("-bind_poses-array"))
        {
            QStringList listNum = line.split(">").at(1).split("<").at(0).split(" ");
            int m_counter=0;
            for(int i=0; i<vecJoints->count(); ++i)
            {
                QMatrix4x4 *matrixInv = new QMatrix4x4;
                matrixInv->setToIdentity();
                for(int r=0; r<4; ++r)
                {
                    QVector4D vec4D(listNum.at(m_counter+0).toFloat(),
                                    listNum.at(m_counter+1).toFloat(),
                                    listNum.at(m_counter+2).toFloat(),
                                    listNum.at(m_counter+3).toFloat());
                    matrixInv->setRow(r, vec4D);

                    m_counter+=4;
                }
                vecJoints->at(i)->setMatrixINV(matrixInv);
            }
        }

    }
    else if(line.startsWith("<vcount>"))
    {
        *listVCount = line.split(">").at(1).split("<").at(0).trimmed().split(" ");
    }
    else if(line.startsWith("<v>"))
    {
        QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

        int m_counter = 0;
        for(int i=0; i<listVCount->count(); ++i)
        {
            QList<QVector2D> *listJW = new QList<QVector2D>;

            for(int j=0; j<listVCount->at(i).toInt(); ++j)
            {
                QVector2D vecV(listNum.at(m_counter+0).toInt(),
                               listWeights->at(listNum.at(m_counter+1).toInt()).toFloat());
                listJW->append(vecV);
                m_counter+=2;
            }
            vecListJW->append(listJW);
        }
//        nearListJW(vecListJW);
        readAllFaces();
    }
}

void VAOCollada::pushLibVisualScenes(QString line)
{
    if(line.startsWith("<node") && line.contains("type=\"JOINT\""))
    {
        if(vecListChild->count() < 1)
        {
            QString nameJoint = line.split(" ").at(2).split("\"").at(1);
            QList<Joint*> *list = new QList<Joint*>;
            vecListChild->append(list);
            listNames->append(nameJoint);
        }
        else
        {
            QString nameJoint = line.split(" ").at(3).split("\"").at(1); // قد تختلف المسميات

            for(int i=0; i<vecJoints->count(); ++i)
            {
                if(vecJoints->at(i)->getName() == nameJoint)
                {
                     for(int ii=0; ii<vecJoints->count(); ++ii)
                    {
                        if(vecJoints->at(ii)->getName() == listNames->last())
                        {
                            vecJoints->at(i)->setParent(vecJoints->at(ii));
                        }
                    }

                    vecListChild->at(vecListChild->count()-1)->append(vecJoints->at(i));
                    listNames->append(vecJoints->at(i)->getName());
                    break;
                }
            }

            QList<Joint*> *list = new QList<Joint*>;
            vecListChild->append(list);
        }
    }
    else if(line.startsWith("<matrix"))
    {
        if(listNames->count() > 0)
        {
            QStringList listNum = line.split(">").at(1).split("<").at(0).split(" ");
            QMatrix4x4 *matrixJoint = new QMatrix4x4;
            matrixJoint->setToIdentity();
            for(int i=0; i<listNum.count(); i+=4)
            {
                matrixJoint->setRow(i/4, QVector4D(listNum.at(i+0).toFloat(),
                                        listNum.at(i+1).toFloat(),
                                        listNum.at(i+2).toFloat(),
                                        listNum.at(i+3).toFloat()));
            }

            for(int i=0; i<vecJoints->count(); ++i)
            {
                if(vecJoints->at(i)->getName() == listNames->last())
                {
                    vecJoints->at(i)->setMatrixLocal(matrixJoint);
                }
            }
        }
    }
    else if(line.startsWith("</node"))
    {
        if(vecListChild->count() > 0) // Checked Start Inter Node Type=JOINT
        {
            for(int i=0; i<vecJoints->count(); ++i)
            {
                if(vecJoints->at(i)->getName() == listNames->last())
                {
                    // Add Childs
                    vecJoints->at(i)->addChilds(vecListChild->last());

//                    qDebug()<<"List Child "<<listNames->last()<<": "<<vecJoints->at(i)->getChild()->count();
//                    for(int c=0; c<vecJoints->at(i)->getChild()->count(); ++c)
//                    {
//                        qDebug()<<vecJoints->at(i)->getChild()->at(c)->getName();
//                    }

                    listNames->removeLast();
                    vecListChild->removeLast();
                    break;
                }
            }
        }
    }
}

void VAOCollada::pushLibAnimation(QString line)
{
    if(line.startsWith("<float_array") && line.contains("-input-array"))
    {
        if(listTimeFeyFrames->count() == 0)
        {
            *listTimeFeyFrames = line.split(">").at(1).split("<").at(0).trimmed().split(" ");
        }
    }
    else if(line.startsWith("<float_array") && line.contains("-output-array"))
    {
        QString nameJoint = line.split("_").at(4);
        QStringList listNum = line.split(">").at(1).split("<").at(0).trimmed().split(" ");

        vec_NamesTrans.append(nameJoint);

        QVector<TransformJoint*> *vecTran = new QVector<TransformJoint*>;
        QVector<QMatrix4x4*> *vecMatAnimation = new QVector<QMatrix4x4*>;

        for(int i=0; i<listNum.count(); i+=16)
        {
            QMatrix4x4 *matAnim = new QMatrix4x4;
            for(int j=0; j<16; j+=4)
            {
                QVector4D vec4D(listNum.at(i+j+0).toFloat(),
                                listNum.at(i+j+1).toFloat(),
                                listNum.at(i+j+2).toFloat(),
                                listNum.at(i+j+3).toFloat());
                matAnim->setRow(j/4, vec4D);
            }

            QVector3D vecPos(matAnim->row(0).w(), matAnim->row(1).w(), matAnim->row(2).w());
            QMatrix3x3 mat3 = matAnim->toGenericMatrix<3,3>();
            QQuaternion qutRot = QQuaternion::fromRotationMatrix(mat3);
            TransformJoint *tran = new TransformJoint(vecPos, qutRot);

            vecTran->append(tran);
//            matAnim->rotate(-90, 0,0,1);
            vecMatAnimation->append(matAnim);
        }
        vec_Transform.append(vecTran);
        vec_MatAnim.append(vecMatAnimation);
    }

}

void VAOCollada::popLibAnimation()
{
    for(int i=0; i<listTimeFeyFrames->count(); ++i)
    {
        QMap<QString, TransformJoint*> *mapTransform = new QMap<QString, TransformJoint*>;
        QMap<QString, QMatrix4x4*> *mapMatAnimation = new QMap<QString, QMatrix4x4*>;

        for(int j=0; j<vec_NamesTrans.count(); ++j)
        {
            mapTransform->insert(vec_NamesTrans.at(j), vec_Transform.at(j)->at(i));
            mapMatAnimation->insert(vec_NamesTrans.at(j), vec_MatAnim.at(j)->at(i));



        }

//        KeyFrame *frame = new KeyFrame(listTimeFeyFrames->at(i).toFloat(), mapTransform);
        KeyFrame *frame = new KeyFrame(listTimeFeyFrames->at(i).toFloat(), mapMatAnimation);

        vecKeyFrames->append(frame);
    }
}

void VAOCollada::readAllFaces()
{
    for(int i=0; i<listFaces->count(); i+=3) // if found value Color : i+=4 || Not i+=3
    {
        VerticesGroup *vecGroup = new VerticesGroup;
        vecGroup->position = vecVertex->at(listFaces->at(i+0).toInt());
        vecGroup->normal = vecNormal->at(listFaces->at(i+1).toInt());
        vecGroup->texture = vecTexture->at(listFaces->at(i+2).toInt());
        vecGroup->vecJW = vecListJW->at(listFaces->at(i+0).toInt());
        vecGroups->append(vecGroup);
    }
    createVAO();
}

QVector3D VAOCollada::nearListJW(QVector<QList<QVector2D> *> *vecList2D)
{
//    QVector3D maxJoint;
    QList<QVector2D> listVec2DMax;
    QList<QVector2D> listSort;
    for(int x=0; x<vecList2D->count(); ++x)
    {
        listVec2DMax = *vecList2D->at(x);
        for(int l=0; l<listVec2DMax.count(); ++l)
        {
            QVector2D vec2D = listVec2DMax.at(l);
            if(l==0)
            {
                listSort.append(vec2D);
            }
            else
            {
                for(int s=0; s<listSort.count(); ++s)
                {
                    if(vec2D.y() > listSort.at(s).y())
                    {
                        listSort.insert(s, vec2D);
                        break;
                    }
                    else
                    {
                        if(s == listSort.count()-1)
                        {
                            listSort.append(vec2D);
                        }
                    }
                }
            }
        }
//        listVec2DMax
    }
}

void VAOCollada::createVAO()
{
    QVector<QVector3D> vecVertex;
    QVector<QVector3D> vecNormals;
    QVector<QVector2D> vecTextureUV;
    QVector<QVector3D> vecHoints;
    QVector<QVector3D> vecWeights;

    for(int i=0; i<vecGroups->count(); ++i)
    {
        vecVertex.append(vecGroups->at(i)->position);
        vecNormals.append(vecGroups->at(i)->normal);
        vecTextureUV.append(vecGroups->at(i)->texture);

        QVector3D jJoint(0,0,0), jWeight(0,0,0);
        for(int j=0; j<vecGroups->at(i)->vecJW->count(); ++j)
        {
            QVector2D vecd = vecGroups->at(i)->vecJW->at(j);

            if(j==0)
            {
                jJoint.setX(vecd.x());
                jWeight.setX(vecd.y());
            }else if(j==1)
            {
                jJoint.setY(vecd.x());
                jWeight.setY(vecd.y());
            }else if(j==2)
            {
                jJoint.setZ(vecd.x());
                jWeight.setZ(vecd.y());
            }
        }
//        qDebug()<<"JJ: "<<jJoint;
//        qDebug()<<"JW: "<<jWeight<<"\n";
        vecHoints.append(jJoint);
        vecWeights.append(jWeight);
    }
//    qDebug("READ Collada 0");
//    funReadFile(vecVertex);

    m_VAO = new QOpenGLVertexArrayObject;
    m_VAO->create();
    m_VAO->bind();
    //Vertex
    QOpenGLBuffer *vboVertex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertex->create();
    vboVertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertex->bind();
    vboVertex->allocate(vecVertex.constData(), vecVertex.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_vertex",0);
    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // Normal
    QOpenGLBuffer *vboNormal = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormal->create();
    vboNormal->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormal->bind();
    vboNormal->allocate(vecNormals.constData(), vecNormals.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_normal",1);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(1,GL_FLOAT, 0, 3);
    // Texture
    QOpenGLBuffer *vboTexture = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexture->create();
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->bind();
    vboTexture->allocate(vecTextureUV.constData(), vecTextureUV.count() *2* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_texcoord", 2);
    m_shader->enableAttributeArray(2);
    m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2);

    //Joint
    QOpenGLBuffer *vboJoint = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboJoint->create();
    vboJoint->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboJoint->bind();
    vboJoint->allocate(vecHoints.constData(), vecHoints.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_joints",3);
    m_shader->enableAttributeArray(3);
    m_shader->setAttributeBuffer(3, GL_FLOAT, 0, 3);
    //Weight
    QOpenGLBuffer *vboWeight = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboWeight->create();
    vboWeight->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboWeight->bind();
    vboWeight->allocate(vecWeights.constData(), vecWeights.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_Weights",4);
    m_shader->enableAttributeArray(4);
    m_shader->setAttributeBuffer(4, GL_FLOAT, 0, 3);

    m_VAO->release();
    m_Count_V = vecVertex.count();
}

QOpenGLVertexArrayObject *VAOCollada::getVAO()
{
    return m_VAO;
}

int VAOCollada::getCountV()
{
    return m_Count_V;
}

QVector<VerticesGroup *> *VAOCollada::getListGroub()
{
    return vecGroups;
}

QVector<Joint *> *VAOCollada::getListJoints()
{
    return vecJoints;
}

QVector<KeyFrame *> *VAOCollada::getListKeyFrame()
{
    return vecKeyFrames;
}

float VAOCollada::getLengthFrames()
{
    if(listTimeFeyFrames->count() < 1)
    {
        return 0.0f;
    }
    else
    {
        return listTimeFeyFrames->last().toFloat();
    }

}

void VAOCollada::setTexture(QString paths)
{
    m_textureImg = new QOpenGLTexture(QImage(paths).mirrored());
    m_textureImg->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textureImg->setMagnificationFilter(QOpenGLTexture::Linear);
}

QOpenGLTexture *VAOCollada::getTexture()
{
    return m_textureImg;
}


void VAOCollada::importers()
{
//    NOT USEED

//    AssimpLib *simp = new AssimpLib;
//    std::vector<Vertex*> *vecFace = simp->getFaces();
//    createVAOSimp(vecFace);
}


void VAOCollada::createVAOSimp(std::vector<Vertex*> &vecFaces)
{
    QVector<QVector3D> vecVertex;
    QVector<QVector3D> vecNormals;
    QVector<QVector2D> vecTextureUV;
    QVector<QVector4D> vecHoints;
    QVector<QVector4D> vecWeights;

    for(uint i=0; i<vecFaces.size(); ++i)
    {
        vecVertex.append(QVector3D(vecFaces[i]->position.x,vecFaces[i]->position.y,vecFaces[i]->position.z));
        vecNormals.append(QVector3D(vecFaces[i]->normal.x,vecFaces[i]->normal.y,vecFaces[i]->normal.z));
        vecTextureUV.append(QVector2D(vecFaces[i]->uv.x,vecFaces[i]->uv.y));

        QVector4D vecJointA(vecFaces[i]->boneID.x, vecFaces[i]->boneID.y,
                            vecFaces[i]->boneID.z, vecFaces[i]->boneID.w);
        QVector4D vecJointWeight(vecFaces[i]->boneWeight.x, vecFaces[i]->boneWeight.y,
                                 vecFaces[i]->boneWeight.z, vecFaces[i]->boneWeight.w);

        vecHoints.append(vecJointA);
        vecWeights.append(vecJointWeight);
    }

    m_VAO = new QOpenGLVertexArrayObject;
    m_VAO->create();
    m_VAO->bind();
    //Vertex
    QOpenGLBuffer *vboVertex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertex->create();
    vboVertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertex->bind();
    vboVertex->allocate(vecVertex.constData(), vecVertex.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_vertex",0);
    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // Normal
    QOpenGLBuffer *vboNormal = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormal->create();
    vboNormal->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormal->bind();
    vboNormal->allocate(vecNormals.constData(), vecNormals.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_normal",1);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(1,GL_FLOAT, 0, 3);
    // Texture
    QOpenGLBuffer *vboTexture = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexture->create();
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->bind();
    vboTexture->allocate(vecTextureUV.constData(), vecTextureUV.count() *2* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_texcoord", 2);
    m_shader->enableAttributeArray(2);
    m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2);

    //Joint
    QOpenGLBuffer *vboJoint = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboJoint->create();
    vboJoint->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboJoint->bind();
    vboJoint->allocate(vecHoints.constData(), vecHoints.count() *4* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_joints",3);
    m_shader->enableAttributeArray(3);
    m_shader->setAttributeBuffer(3, GL_FLOAT, 0, 4);
    //Weight
    QOpenGLBuffer *vboWeight = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboWeight->create();
    vboWeight->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboWeight->bind();
    vboWeight->allocate(vecWeights.constData(), vecWeights.count() *4* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_Weights",4);
    m_shader->enableAttributeArray(4);
    m_shader->setAttributeBuffer(4, GL_FLOAT, 0, 4);

    m_VAO->release();
    m_Count_V = vecVertex.count();
}















