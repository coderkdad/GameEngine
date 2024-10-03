#ifndef ASSIMPLIB_H
#define ASSIMPLIB_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QVector>
#include <QMatrix4x4>

struct Vertex {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 uv = glm::vec2(0.0f);
    glm::vec4 boneID = glm::vec4(0);
    glm::vec4 boneWeight = glm::vec4(0.0f);
};

struct Bone {
    int id = 0;
    std::string name = "";
    glm::mat4 offset = glm::mat4(1.0f); // معكوس هرمية التحولات : مثل (INV)
    std::vector<Bone*> children = {};
};

struct Meshs {
    std::string name;
    std::vector<Vertex*> vertices;
    std::vector<uint> vecIDBones;
};

struct BoneTransformTrack { // 7:30 // مخصصة لكل عظم على حدة
    std::vector<float> posTimeStemp = {};
    std::vector<float> rotTimeStemp = {};
    std::vector<float> scalTimeStemp = {};

    std::vector<glm::vec3> position = {};
    std::vector<glm::quat> rotation = {};
    std::vector<glm::vec3> scaling = {};
};

struct Animations {
    std::string name = "";
    float duration = 0.0f;
    float ticks = 1.0f;
    std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

class AssimpLib
{
public:
    AssimpLib(const char *filePaths);

    std::vector<Meshs *> getMeshes();
    Bone* getBone();
    uint countBone();
    std::vector<Animations *> getAnimation();
    glm::mat4 getGlobalMatrixINV();
    

private:
    const char *filePath = "";
    std::vector<Meshs*> vecMeshs;
    Bone *skeleton;
    uint mCountBone = 0;
    std::vector<Animations*> vecAnimations;
    glm::mat4 globalInvertMatrix = glm::mat4(1.0f);
    
    void importers();

    void loadModel(const aiScene *scene);
    void processVertexFill(aiMesh *iMesh, std::vector<Vertex *> *vertices);
    void processVertexEqualOne(std::vector<Vertex *> *vertices);
    void processVertexWieght(aiBone *bone, uint *idBone, std::vector<Vertex *> *vertices, std::vector<uint> *vertexJW);
    void processVertexBone(aiBone *bone, uint *idBone, uint *mCounterBone, std::unordered_map<std::string, std::pair<int, glm::mat4> > *boneInfoAll);

    bool readSkeleton(Bone *boneOutput, aiNode *node,
                      std::unordered_map<std::string, std::pair<int, glm::mat4> > &boneInfoTable);

    void loadAnimation(const aiScene *scene);

    glm::mat4 assimpToMat(aiMatrix4x4 matrixIn);
    glm::vec3 assimpToVec3(aiVector3D vecIn);
    glm::quat assimpToQuat(aiQuaternion quatIn);
    QMatrix4x4 convertToQMatrix(glm::mat4 *matrix);
};


#endif // ASSIMPLIB_H
