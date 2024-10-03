#include "assimplib.h"
#include <QDebug>

AssimpLib::AssimpLib(const char *filePaths) : filePath(filePaths)
{
    skeleton = new Bone {};

    importers();
}

std::vector<Meshs *> AssimpLib::getMeshes()
{
    return vecMeshs;
}

Bone *AssimpLib::getBone()
{
    return skeleton;
}

uint AssimpLib::countBone()
{
    return mCountBone;
}

std::vector<Animations *> AssimpLib::getAnimation()
{
    return vecAnimations;
}

glm::mat4 AssimpLib::getGlobalMatrixINV()
{
    return globalInvertMatrix;
}


void AssimpLib::importers()
{
    Assimp::Importer importer;

//    const char *filePaths = "/home/aloool14/PersonCube/testCubes_61.dae";

    const aiScene *scene = importer.ReadFile(filePath,
                                             aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        qDebug("Not Open File Assimp");
        return;
    }

    globalInvertMatrix = assimpToMat(scene->mRootNode->mTransformation);
    globalInvertMatrix = glm::inverse(globalInvertMatrix);

    loadModel(scene);
    loadAnimation(scene);
}

void AssimpLib::loadModel(const aiScene *scene)
{
    // Export from Meshs:
    // vertex + fill Wieght
    // tableBone
    // sort Child skeleton

    std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfoAll = {} ;
    uint mCounterBone = 0;

    vecMeshs.clear();
    for(uint m=0; m<scene->mNumMeshes; ++m)
    {
        aiMesh *iMesh = scene->mMeshes[m];

        std::vector<Vertex*> verticesMsh;
        std::vector<uint> vecIDBoneMesh;

        processVertexFill(iMesh, &verticesMsh);

        // fill wieght + tableBone
        std::vector<uint> boneJW;
        boneJW.resize(verticesMsh.size(), 0);

        for(uint i=0; i<iMesh->mNumBones; ++i)
        {
            aiBone *bone = iMesh->mBones[i];
            uint idBone = -1;

            processVertexBone(bone, &idBone, &mCounterBone, &boneInfoAll);

            vecIDBoneMesh.push_back(idBone);

            processVertexWieght(bone, &idBone, &verticesMsh, &boneJW);
        }

        processVertexEqualOne(&verticesMsh);

        Meshs *mMeshs = new Meshs;
        mMeshs->name = iMesh->mName.C_Str();
        mMeshs->vertices = verticesMsh;
        mMeshs->vecIDBones = vecIDBoneMesh;
        vecMeshs.push_back(mMeshs);

    }

    readSkeleton(skeleton, scene->mRootNode, boneInfoAll);

    mCountBone = mCounterBone;

}

void AssimpLib::processVertexFill(aiMesh *iMesh, std::vector<Vertex*> *vertices)
{
    for(uint i=0; i<iMesh->mNumVertices; ++i)
    {
        Vertex *mVertex = new Vertex{};

        mVertex->position = glm::vec3(iMesh->mVertices[i].x,
                                      iMesh->mVertices[i].y,
                                      iMesh->mVertices[i].z);

        mVertex->normal = glm::vec3(iMesh->mNormals[i].x,
                                    iMesh->mNormals[i].y,
                                    iMesh->mNormals[i].z);

        mVertex->uv = glm::vec2(iMesh->mTextureCoords[0][i].x,
                                iMesh->mTextureCoords[0][i].y);

        mVertex->boneID = glm::ivec4(0);
        mVertex->boneWeight = glm::vec4(0.0f);

        vertices->push_back(mVertex);
    }
}
void AssimpLib::processVertexEqualOne(std::vector<Vertex*> *vertices)
{
    for(uint i=0; i<vertices->size(); ++i)
    {
        glm::vec4 &boneW = vertices->at(i)->boneWeight;
        float totalW = boneW.x + boneW.y + boneW.z + boneW.w;

        if(totalW > 0.0f)
        {
            vertices->at(i)->boneWeight = glm::vec4 (
                        boneW.x / totalW,
                        boneW.y / totalW,
                        boneW.z / totalW,
                        boneW.w / totalW
                        );
        }
    }
}
void AssimpLib::processVertexWieght(aiBone *bone, uint *idBone, std::vector<Vertex*> *vertices, std::vector<uint> *vertexJW)
{
    for(uint j=0; j<bone->mNumWeights; ++j)
    {
        uint id =      bone->mWeights[j].mVertexId;
        float weight = bone->mWeights[j].mWeight;

        (*vertexJW)[id] = (*vertexJW)[id]+1;

        switch ((*vertexJW)[id]) {
        case 1:
            vertices->at(id)->boneID.x = *idBone;
            vertices->at(id)->boneWeight.x = weight;
            break;
        case 2:
            vertices->at(id)->boneID.y = *idBone;
            vertices->at(id)->boneWeight.y = weight;
            break;
        case 3:
            vertices->at(id)->boneID.z = *idBone;
            vertices->at(id)->boneWeight.z = weight;
            break;
        case 4:
            vertices->at(id)->boneID.w = *idBone;
            vertices->at(id)->boneWeight.w = weight;
            break;
        default:
            // no
            break;
        }
    }
}
void AssimpLib::processVertexBone(aiBone *bone, uint *idBone, uint *mCounterBone,
                                  std::unordered_map<std::string, std::pair<int, glm::mat4>> *boneInfoAll)
{
    glm::mat4 mOffset = assimpToMat(bone->mOffsetMatrix);
    std::string nameBone = bone->mName.C_Str();

    if(boneInfoAll->find(nameBone) == boneInfoAll->end())
    {
        qDebug()<<"Not Found: "<<nameBone.c_str();
        *idBone = *mCounterBone;
        (*boneInfoAll)[nameBone] = {*idBone,mOffset};
        ++(*mCounterBone);
    }
    else
    {
        qDebug()<<"Its Found: "<<nameBone.c_str();
        std::unordered_map<std::string, std::pair<int, glm::mat4>>::iterator itr;
        for(itr = boneInfoAll->begin(); itr != boneInfoAll->end(); itr++)
        {
            if(itr->first == nameBone)
            {
                *idBone = itr->second.first;
            }
        }
    }
}

bool AssimpLib::readSkeleton(Bone *boneOutput, aiNode *node,
                             std::unordered_map<std::string, std::pair<int, glm::mat4> > &boneInfoTable)
{
    // 45:00 Tree skeleton
    if(boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end())
    {
        boneOutput->name = node->mName.C_Str();
        boneOutput->id = boneInfoTable[boneOutput->name].first;
        boneOutput->offset = boneInfoTable[boneOutput->name].second;

        for(uint i=0; i<node->mNumChildren; ++i)
        {
            Bone *child = new Bone;
            readSkeleton(child, node->mChildren[i], boneInfoTable);
            boneOutput->children.push_back(child);
        }
        return true;
    }
    else
    {
        // غي حال كان العقدة غير العظام كالكاميرا وغيرها
        for(uint i=0; i<node->mNumChildren; ++i)
        {
            if(readSkeleton(boneOutput, node->mChildren[i], boneInfoTable))
            {
                return true;
            }
        }
    }
    return false;
}

void AssimpLib::loadAnimation(const aiScene *scene)
{
    qDebug()<<"Num Anim: "<<scene->mNumAnimations;
    for(uint i=0; i<scene->mNumAnimations; ++i)
    {
        Animations *mAnims = new Animations{};
        vecAnimations.push_back(mAnims);

        aiAnimation *iAnim = scene->mAnimations[i];
        mAnims->name = iAnim->mName.C_Str();

        if(iAnim->mTicksPerSecond != 0.0f)
        {
            mAnims->ticks = iAnim->mTicksPerSecond;
        }
        else
        {
            mAnims->ticks = 1;
        }

        mAnims->duration = iAnim->mDuration * iAnim->mTicksPerSecond;

        // load pose 50:00
        for(uint i=0; i<iAnim->mNumChannels; ++i)
        {
            // لكل عظم قناة والقناة تحمل جميع حركات العظم
            aiNodeAnim *channel = iAnim->mChannels[i];
            BoneTransformTrack track = {};

            for(uint j=0; j<channel->mNumPositionKeys; ++j)
            {
                track.posTimeStemp.push_back(channel->mPositionKeys[j].mTime);
                track.position.push_back(assimpToVec3(channel->mPositionKeys[j].mValue));
            }
            for(uint j=0; j<channel->mNumRotationKeys; ++j)
            {
                track.rotTimeStemp.push_back(channel->mRotationKeys[j].mTime);
                track.rotation.push_back(assimpToQuat(channel->mRotationKeys[j].mValue));
            }
            for(uint j=0; j<channel->mNumScalingKeys; ++j)
            {
                track.scalTimeStemp.push_back(channel->mScalingKeys[j].mTime);
                track.scaling.push_back(assimpToVec3(channel->mScalingKeys[j].mValue));
            }

            mAnims->boneTransforms[channel->mNodeName.C_Str()] = track;
        }

    }
}


glm::mat4 AssimpLib::assimpToMat(aiMatrix4x4 matrixIn)
{
    glm::mat4 mtx;

    for(int y=0; y<4; ++y)
    {
        for(int x=0; x<4; ++x)
        {
            mtx[x][y] = matrixIn[y][x];
        }
    }

    return mtx;
}
glm::vec3 AssimpLib::assimpToVec3(aiVector3D vecIn)
{
    return glm::vec3(vecIn.x, vecIn.y, vecIn.z);
}
glm::quat AssimpLib::assimpToQuat(aiQuaternion quatIn)
{
    glm::quat q;
    q.x = quatIn.x;
    q.y = quatIn.y;
    q.z = quatIn.z;
    q.w = quatIn.w;

    return q;
}
QMatrix4x4 AssimpLib::convertToQMatrix(glm::mat4 *matrix)
{
    float *a = glm::value_ptr(*matrix);

    QMatrix4x4 q_mtrx(a[0], a[1], a[2], a[3],
                      a[4], a[5], a[6], a[7],
                      a[8], a[9], a[10], a[11],
                      a[12], a[13], a[14], a[15]);

    return q_mtrx;
}



