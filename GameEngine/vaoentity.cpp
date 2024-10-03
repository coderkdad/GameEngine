#include "vaoentity.h"
#include "math.h"
#include "qmath.h"
#include <QFile>

VAOEntity::VAOEntity(QOpenGLShaderProgram *shader) : m_shader(shader)
{

    listEntityInfo = new QList<EntityInfo*>;
    limitFull = new QCubeLimited;

}

VAOEntity::~VAOEntity()
{
    qDebug("Str");
    //========================//

    if(m_textureImg != nullptr)
    {
        if(m_textureImg->isCreated())
        {
            m_textureImg->destroy();
            m_textureImg = nullptr;
        }
    }
    qDebug("HHH");

    //========================//


    for(int i=0; i<listEntityInfo->count(); ++i)
    {
        EntityInfo *inf = listEntityInfo->takeAt(i);


        // vao
        QOpenGLVertexArrayObject *e = inf->vao;
        qDebug("VVV");
        e->destroy();
        e = nullptr;
        qDebug("ZZ");

        // texture
//        if(inf->texture->isCreated())
//        {
//            inf->texture->destroy();
//            inf->texture = nullptr;
//        }

        // vecTring
//        if(inf->vecTringle != nullptr)
//        {
//            inf->vecTringle->clear();
//            delete inf->vecTringle;
//            inf->vecTringle = nullptr;
//        }

        // limit
        delete inf->limited;
        inf->limited = nullptr;
        qDebug("Z");

        // partLimit
        if(inf->partsLimited != nullptr)
        {
            for(int i=0; i<inf->partsLimited->count(); ++i)
            {
                QCubeLimited *cub = inf->partsLimited->takeAt(i);
                delete cub;
                cub = nullptr;
            }
            inf->partsLimited->clear();
            delete inf->partsLimited;
            inf->partsLimited = nullptr;
        }
        qDebug("FSH");
    }

    //========================//

    delete limitFull;
    limitFull = nullptr;


    //========================//
    qDebug("FSH Finl");
    //========================//
}

void VAOEntity::setPath(QString paths)
{
    QFile file(paths);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Not Open File obj:-\n"<<paths;
        return;
    }
    //============= Read File =================//
    QVector<QVector3D> vecVertex;
    QVector<QVector3D> vecNormals;
    QVector<QVector2D> vecTextureUV;
    QVector<QTriangle3D> vecTriangles;

    QString nameO = "";
    int countVertex=0;
    int countNormals=0;
    int countTexture=0;

    while (!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        if(line.left(2) == "v ")
        {
            vecVertex.append(QVector3D(line.split(" ").at(1).toFloat(),
                                       line.split(" ").at(2).toFloat(),
                                       line.split(" ").at(3).toFloat()));
        }
        else if (line.left(2) == "vn")
        {
            vecNormals.append(QVector3D(line.split(" ").at(1).toFloat(),
                                        line.split(" ").at(2).toFloat(),
                                        line.split(" ").at(3).toFloat()));
        }
        else if (line.left(2) == "vt")
        {
            vecTextureUV.append(QVector2D(line.split(" ").at(1).toFloat(),
                                          line.split(" ").at(2).toFloat()));
        }
        else if (line.left(2) == "f ")
        {
            //proc
            QTriangle3D triangle;
            triangle.p1 = vecVertex.at((line.split(" ").at(1).split("/").at(0).toInt()- 1)-countVertex);
            triangle.p2 = vecVertex.at((line.split(" ").at(2).split("/").at(0).toInt()- 1)-countVertex);
            triangle.p3 = vecVertex.at((line.split(" ").at(3).split("/").at(0).toInt()- 1)-countVertex);

            if(vecTextureUV.count() > 0) { // للتحقق من وجود بيانات texture

                if(!line.split(" ").at(1).split("/").at(1).isEmpty()) {
                    triangle.p1UV = vecTextureUV.at((line.split(" ").at(1).split("/").at(1).toInt()- 1) - countTexture);
                } else {
                    triangle.p1UV = QVector2D(0.0, 0.0);
                }

                if(!line.split(" ").at(2).split("/").at(1).isEmpty()) {
                    triangle.p2UV = vecTextureUV.at((line.split(" ").at(2).split("/").at(1).toInt()- 1) - countTexture);
                } else {
                    triangle.p2UV = QVector2D(0.0, 0.0);
                }

                if(!line.split(" ").at(3).split("/").at(1).isEmpty()) {
                    triangle.p3UV = vecTextureUV.at((line.split(" ").at(3).split("/").at(1).toInt()- 1) - countTexture);
                } else {
                    triangle.p3UV = QVector2D(0.0, 0.0);
                }

            } else { // إذا لم يوجد لا نتركها فارغه بل نضع قيمه صفر
                triangle.p1UV = QVector2D(0.0, 0.0);
                triangle.p2UV = QVector2D(0.0, 0.0);
                triangle.p3UV = QVector2D(0.0, 0.0);
            }

            if(vecNormals.count() > 0) { // للتحقق من وجود قيم للـ normal
                triangle.p1Normal = vecNormals.at((line.split(" ").at(1).split("/").at(2).toInt()- 1) - countNormals);
                triangle.p2Normal = vecNormals.at((line.split(" ").at(2).split("/").at(2).toInt()- 1) - countNormals);
                triangle.p3Normal = vecNormals.at((line.split(" ").at(3).split("/").at(2).toInt()- 1) - countNormals);
            }
            vecTriangles.append(triangle);
        }
        else if (line.left(2) == "o ")
        {
            if(vecTriangles.count()<1) { }
            else {
                QVector<QTriangle3D> *mvecTriangles = new QVector<QTriangle3D>(vecTriangles);

                addingVAO(mvecTriangles, nameO, vecVertex);

                ///Calc Adding
                countVertex+=vecVertex.count();
                countNormals+=vecNormals.count();
                countTexture+=vecTextureUV.count();

                vecVertex.clear();
                vecNormals.clear();
                vecTextureUV.clear();
                vecTriangles.clear();
            }

            nameO = line.split(" ").at(1);
        }
    }
    file.close();
    // End Entity This ****
    if(vecTriangles.count()>1)
    {
        QVector<QTriangle3D> *mvecTriangles = new QVector<QTriangle3D>(vecTriangles);
        addingVAO(mvecTriangles, nameO, vecVertex);
    }

///=========================================================///
    //setting Limite
//    createLimitTriangles(vecVertex);
}
void VAOEntity::setPathVertex(QVector<QVector3D> vecVrtx, QVector<QVector3D> vecNrml, QVector<QVector2D> vecTxtr)
{
    // this is capsulation empty buffer normal and texture nad listLimit
    // onlyVertex=false;

    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject;
    vao->create();
    vao->bind();

    QOpenGLBuffer *vboVertex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertex->create();
    vboVertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertex->bind();
    vboVertex->allocate(vecVrtx.constData(), vecVrtx.count() *3* sizeof(QVector3D));
    m_shader->bindAttributeLocation("a_vertex",0);
    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // Normal
//    if(!(vecNrml.count() == 0))
//    {

        QOpenGLBuffer *vboNormal = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vboNormal->create();
        vboNormal->setUsagePattern(QOpenGLBuffer::StaticDraw);
        vboNormal->bind();
        vboNormal->allocate(vecNrml.constData(), vecNrml.count() *3* sizeof(QVector3D));
        m_shader->bindAttributeLocation("a_normal",4);
        m_shader->enableAttributeArray(4);
        m_shader->setAttributeBuffer(4,GL_FLOAT, 0, 3);
//    }
    // Texture
    QOpenGLBuffer *vboTexture = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexture->create();
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->bind();
    vboTexture->allocate(vecTxtr.constData(), vecTxtr.count() *3* sizeof(QVector2D));
    m_shader->bindAttributeLocation("a_texcoord", 2);
    m_shader->enableAttributeArray(2);
    m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2);

    vao->release();
    int numVertex = vecVrtx.count();

    EntityInfo *xxx = new EntityInfo;
    xxx->name = "LOCAL";
    xxx->vao = vao;
    xxx->numVertex = numVertex;
    xxx->limited = createLimit(vecVrtx,"m");
//    xxx->partsLimited = createLimitParts2();
    listEntityInfo->append(xxx);


    limitFull = createLimit(vecVrtx,"m"); // Here limitFull Wqual One Entity This
}

void VAOEntity::addingVAO(QVector<QTriangle3D> *mVecTriangles, QString name, QVector<QVector3D> vecVertexBase)
{
    QVector<QVector3D> vecVertex;
    QVector<QVector3D> vecNormals;
    QVector<QVector2D> vecTextureUV;
    for(int i=0; i<mVecTriangles->count(); ++i)
    {
        vecVertex.append(mVecTriangles->at(i).p1);
        vecVertex.append(mVecTriangles->at(i).p2);
        vecVertex.append(mVecTriangles->at(i).p3);

        vecNormals.append(mVecTriangles->at(i).p1Normal);
        vecNormals.append(mVecTriangles->at(i).p2Normal);
        vecNormals.append(mVecTriangles->at(i).p3Normal);

        vecTextureUV.append(mVecTriangles->at(i).p1UV);
        vecTextureUV.append(mVecTriangles->at(i).p2UV);
        vecTextureUV.append(mVecTriangles->at(i).p3UV);
    }

    ///================= Create VAO =================///

    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject;
    vao->setObjectName(name);
    vao->create();
    vao->bind();

    //Vertex
    QOpenGLBuffer *vboVertex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertex->create();
    vboVertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertex->bind();
    vboVertex->allocate(vecVertex.constData(), vecVertex.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("vertex",0);
    m_shader->enableAttributeArray(0);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // Normal
    QOpenGLBuffer *vboNormal = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormal->create();
    vboNormal->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormal->bind();
    vboNormal->allocate(vecNormals.constData(), vecNormals.count() *3* sizeof(GLfloat));
    m_shader->bindAttributeLocation("vertexNormal",4);
    m_shader->enableAttributeArray(4);
    m_shader->setAttributeBuffer(4,GL_FLOAT, 0, 3);
    // Texture
    QOpenGLBuffer *vboTexture = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexture->create();
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->bind();
    vboTexture->allocate(vecTextureUV.constData(), vecTextureUV.count() *2* sizeof(GLfloat));
    m_shader->bindAttributeLocation("a_texcoord", 2);
    m_shader->enableAttributeArray(2);
    m_shader->setAttributeBuffer(2, GL_FLOAT, 0, 2);

    vao->release();
    int numVertex = vecVertex.count();
//    m_listVecTriangles->append(mVecTriangles);

    ///================= Create InfoEntity =================///


    EntityInfo *xxx = new EntityInfo;
    xxx->name = name;
    xxx->numVertex = numVertex;
    xxx->vao = vao;
//    xxx->vecTringle = mVecTriangles; // No Used
//    qDebug()<<"INFO X:\n"<<xxx->name<<" | "<<xxx->numVertex;
    xxx->limited = createLimit(vecVertexBase, name);
//    xxx->partsLimited =new QVector<QCubeLimited*>(*getLimitedPart2());
    if(name == "PlaneErth")
    {
        qDebug()<<"PE: "<<xxx->limited->limitedTop<<" | "<<xxx->limited->limitedBottom;
        getUpsErth(vecVertexBase, xxx->limited, xxx);

        qDebug("PE");
    }
    else
    {
        qDebug()<<"NAME: "<<name;
        qDebug()<<"LIMIT: "<<xxx->limited->limitedTop<<" | "<<xxx->limited->limitedBottom;
        xxx->partsLimited = createLimitParts(vecVertexBase, xxx->limited);
    }
    listEntityInfo->append(xxx);
//    if(name == "PlaneErth")
//    {
//    }

}

QCubeLimited *VAOEntity::createLimit(QVector<QVector3D> vertex, QString n)
{
    // Warning: if value (top z < 0)
    // Solved: create blender object in center |OR| ..
    // Inter first value in limited[top&Bottom]..contenue check

    QCubeLimited *limitEntity = new QCubeLimited;

    for(int i=0; i<vertex.count(); ++i)
    {
        float vertexX=vertex.at(i).x();
        float vertexY=vertex.at(i).y();
        float vertexZ=vertex.at(i).z();

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

//    if(n != "PlaneErth")
//        createLimitParts(vertex, limitEntity);

    /// Save Limit Full Model
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

QVector<QCubeLimited *> *VAOEntity::createLimitParts(QVector<QVector3D> vertex, QCubeLimited *cube)
{
    QVector<QCubeLimited *> *vecPartsEntity = new QVector<QCubeLimited *>;
    QVector<QCubeLimited*> vecLimitedPartsX;
    QVector<QCubeLimited*> vecLimitedPartsY;
    QVector<QCubeLimited*> vecLimitedPartsZ;

    float rangSpace = 0.25f;

    int numPartTopX    = cube->limitedTop.x()    /rangSpace;
    int numPartBottomX = cube->limitedBottom.x() /rangSpace;
    int numPartX = numPartTopX+fabs(numPartBottomX) + 2;

    int numPartTopY    = cube->limitedTop.y()    /rangSpace;
    int numPartBottomY = cube->limitedBottom.y() /rangSpace;
    int numPartY = numPartTopY+fabs(numPartBottomY) + 2;

    int numPartTopZ    = cube->limitedTop.z()    /rangSpace;
    int numPartBottomZ = cube->limitedBottom.z() /rangSpace;
    int numPartZ = numPartTopZ+fabs(numPartBottomZ) + 2;

    // create parent parts limit X Y Z
    {
        for(int i=0; i<numPartX; ++i)
        {
            QCubeLimited *limiX = new QCubeLimited;
            limiX->limitedTop =    QVector3D(float(numPartTopX*rangSpace) - ((i)*rangSpace)   + rangSpace ,0,0);
            limiX->limitedBottom = QVector3D(float(numPartTopX*rangSpace) - ((i+1)*rangSpace) + rangSpace ,0,0);
            vecLimitedPartsX.append(limiX);
        }
        for(int i=0; i<numPartY; ++i)
        {
            QCubeLimited *limiY = new QCubeLimited;
            limiY->limitedTop =    QVector3D(0, float(numPartTopY*rangSpace) - ((i)*rangSpace)   + rangSpace, 0);
            limiY->limitedBottom = QVector3D(0, float(numPartTopY*rangSpace) - ((i+1)*rangSpace) + rangSpace, 0);
            vecLimitedPartsY.append(limiY);
        }
        for(int i=0; i<numPartZ; ++i)
        {
            QCubeLimited *limiZ = new QCubeLimited;
            limiZ->limitedTop =    QVector3D(0,0, float(numPartTopZ*rangSpace) - ((i)*rangSpace)   + rangSpace);
            limiZ->limitedBottom = QVector3D(0,0, float(numPartTopZ*rangSpace) - ((i+1)*rangSpace) + rangSpace);
            vecLimitedPartsZ.append(limiZ);
        }
    }

    QCubeLimited defultCube;
    defultCube.limitedTop = QVector3D(-999.9f,-999.9f,-999.9f);
    defultCube.limitedBottom = QVector3D(999.9f,999.9f,999.9f);
    QVector< QVector< QVector<QCubeLimited> > > arrayLimited(numPartX,
                                                        QVector< QVector<QCubeLimited> >(numPartY,
                                                        QVector<QCubeLimited>(numPartZ,defultCube) ) );


    // check vertex and edit with value parent limit in array
    qDebug()<<"numVertex: "<<vertex.count();
    for(int v=0; v<vertex.count(); ++v)
    {
        float vX = vertex.at(v).x();
        float vY = vertex.at(v).y();
        float vZ = vertex.at(v).z();

        // get index vertex in array
        int getXX=0, getYY=0, getZZ=0;
        {
            for(int i=0; i<vecLimitedPartsX.count(); ++i)
            {
                if(vX < vecLimitedPartsX.at(i)->limitedTop.x() &&
                   vX >= vecLimitedPartsX.at(i)->limitedBottom.x())
                {
                    getXX=i;
                    break;
                }
            }
            for(int i=0; i<vecLimitedPartsY.count(); ++i)
            {
                if(vY < vecLimitedPartsY.at(i)->limitedTop.y() &&
                   vY >= vecLimitedPartsY.at(i)->limitedBottom.y())
                {
                    getYY=i;
                    break;
                }
            }
            for(int i=0; i<vecLimitedPartsZ.count(); ++i)
            {
                if(vZ < vecLimitedPartsZ.at(i)->limitedTop.z() &&
                   vZ >= vecLimitedPartsZ.at(i)->limitedBottom.z())
                {
                    getZZ=i;
                    break;
                }
            }
        }

        // check and edit limit in array
        {
            QCubeLimited *cubeAry = &arrayLimited[getXX][getYY][getZZ];
            // X
            if(vX > cubeAry->limitedTop.x())
            {
                cubeAry->limitedTop.setX(vX);
            }
            if (vX < cubeAry->limitedBottom.x())
            {
                cubeAry->limitedBottom.setX(vX);
            }

            // Y
            if(vY > cubeAry->limitedTop.y())
            {
                cubeAry->limitedTop.setY(vY);
            }
            if (vY < cubeAry->limitedBottom.y())
            {
                cubeAry->limitedBottom.setY(vY);
            }

            // Z
            if(vZ > cubeAry->limitedTop.z())
            {
                cubeAry->limitedTop.setZ(vZ);
            }
            if (vZ < cubeAry->limitedBottom.z())
            {
                cubeAry->limitedBottom.setZ(vZ);
            }
        }

    }

    // Save part limit from array
    for(int x=0; x<numPartX; ++x)
    {
        for(int y=0; y<numPartY; ++y)
        {
            for(int z=0; z<numPartZ; ++z)
            {
                QCubeLimited *cc = new QCubeLimited(arrayLimited[x][y][z]);
                if(cc->limitedTop.x() == -999.9f && cc->limitedBottom.x() == 999.9f)
                {}
                else
                {
                    vecPartsEntity->append(cc);
                }
            }
        }
    }

    qDebug()<<"countAll: "<< numPartX*numPartY*numPartZ;
    qDebug()<<"countParts: "<<vecPartsEntity->count();
    return vecPartsEntity;
}

void VAOEntity::getUpsErth(QVector<QVector3D> vertex, QCubeLimited *cube, EntityInfo *info)
{
    float maxX = cube->limitedTop.x(); //100
    float minX = cube->limitedBottom.x(); //-100
    float maxZ = cube->limitedTop.z(); //100
    float minZ = cube->limitedBottom.z();//-100.30

    float spaceRng=10.0f;
    int countMaxX = floor( fabs(maxX) / spaceRng)+1; //11
    int countMinX = floor( fabs(minX) / spaceRng)+1; //11
    int countMaxZ = floor( fabs(maxZ) / spaceRng)+1; //11
    int countMinZ = floor( fabs(minZ) / spaceRng)+1; //11

    int sumPartX = countMaxX+countMinX;
    int sumPartZ = countMaxZ+countMinZ;
    {
        int nums = sumPartX*sumPartZ;
        qDebug()<<"Num Parts Erth: "<< nums;
    }

    QVector<QVector<float>> arrayErths(sumPartX, QVector<float>(sumPartZ,-66.6f));

    // fill Array
    for(int index=0; index<vertex.count(); ++index)
    {
        float numVerX = vertex.at(index).x();
        float numVerZ = vertex.at(index).z();
        float numVerY = vertex.at(index).y();

        int IDx=0;
        int IDz=0;

        if(numVerX >= 0)
        {
            int pX = floor(fabs(numVerX) / spaceRng);
            IDx = countMaxX-(pX+1);
        }
        else if(numVerX < 0)
        {
            int pX = ceil(fabs(numVerX) / spaceRng);
            IDx = countMaxX+(pX-1);
        }

        if(numVerZ >= 0)
        {
            int pZ = floor(fabs(numVerZ) / spaceRng);
            IDz = countMaxZ-(pZ+1);
        }
        else if(numVerZ < 0)
        {
            int pZ = ceil(fabs(numVerZ) / spaceRng);
            IDz = countMaxZ+(pZ-1);
        }

        arrayErths[IDx][IDz] = numVerY;
    }

    info->arry = arrayErths;
    info->numSpaceRng = spaceRng;
    info->countMaxX = countMaxX;
    info->countMaxZ = countMaxZ;

}

void VAOEntity::setTextureImg(QString texture)
{
    m_textureImg = new QOpenGLTexture(QImage(texture).mirrored());
//    m_textureImg->setWrapMode(QOpenGLTexture::MirroredRepeat);
    m_textureImg->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textureImg->setMagnificationFilter(QOpenGLTexture::Linear);
}
void VAOEntity::setTextureImgEdit(QImage texture)
{
    if(m_textureImg != nullptr)
    {
        m_textureImg->destroy();
        m_textureImg = nullptr;
    }
    m_textureImg = new QOpenGLTexture(texture);
//    m_textureImg->setWrapMode(QOpenGLTexture::MirroredRepeat);
    m_textureImg->setMinificationFilter(QOpenGLTexture::Nearest);
    m_textureImg->setMagnificationFilter(QOpenGLTexture::Linear);
}
QOpenGLTexture *VAOEntity::getTextureImg()
{
    return m_textureImg;
}

QList<EntityInfo*> *VAOEntity::getListInfo()
{
    return listEntityInfo;
}

QCubeLimited *VAOEntity::getLimited()
{
    return limitFull;
}






















