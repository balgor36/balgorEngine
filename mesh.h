#ifndef MESH_H
#define MESH_H

#include <string>

#include "window.h"
#include <glm/gtx/string_cast.hpp>
#include "meshData.h"

#include <vector>

#define IND_VB 			0
#define POS_VB 			1
#define NORMAL_VB 		2
#define TEX_VB 			3
#define TANGENT_VB 		4
#define BITANGENT_VB 	5
#define COLOR_VB        6

struct PBRMaterial{
    glm::vec3 albedo;

    float metallic;
    float roughness;
    float occlusion;
};

class Mesh
{
public:
    Mesh();
    Mesh(std::vector<glm::vec3>* vertexData, std::vector<glm::vec3>* normalData, std::vector<glm::vec2>* uvData,
         std::vector<glm::vec3>* tangentData, std::vector<glm::vec3>* bitangentData, std::vector<glm::vec3>* colorData,
         std::vector<GLuint>* indicesData, std::string* path, std::string* name, PBRMaterial* material);
    ~Mesh();

    GLuint GetVAO();
    GLuint GetTexture();
    GLuint GetTextureID();
    GLuint GetIndicesSize();

    std::string& GetPath();
    std::string& GetName();

    void Render();

    void SetIndex(int index);
    int GetIndex();

    std::vector<Mesh*>& GetChildMeshes();

    std::vector<glm::vec3>& GetVertexData();
    std::vector<glm::vec3>& GetNormalData();
    std::vector<glm::vec2>& GetUVData();
    std::vector<glm::vec3>& GetTangentData();
    std::vector<glm::vec3>& GetBitangentData();
    std::vector<GLuint>& GetIndicesData();

    PBRMaterial GetMaterial();
private:
    std::vector<Mesh*> childMeshes;

    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec3> normalData;
    std::vector<glm::vec2> uvData;

    std::vector<glm::vec3> tangentData;
    std::vector<glm::vec3> bitangentData;
    std::vector<glm::vec3> colorData;

    std::vector<GLuint> indicesData;
    GLuint VAO;
    GLuint m_buffers[6];

    std::string m_path;
    std::string m_name;

    PBRMaterial m_material;

    GLuint indicesSize;

    int index = 0;

};

#endif
