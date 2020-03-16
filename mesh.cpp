#include "mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<glm::vec3>* vertexData, std::vector<glm::vec3>* normalData, std::vector<glm::vec2>* uvData,
           std::vector<glm::vec3>* tangentData, std::vector<glm::vec3>* bitangentData, std::vector<glm::vec3>* colorData,
           std::vector<GLuint>* indicesData, std::string* path, std::string* name, PBRMaterial* material)
{
    this->vertexData = *vertexData;
    this->normalData = *normalData;
    this->uvData = *uvData;
    this->tangentData = *tangentData;
    this->bitangentData = *bitangentData;
    this->colorData = *colorData;
    this->indicesData = *indicesData;
    this->m_name = *name;
    this->m_path = *path;
    this->m_material = *material;

    //texture = 0; // Когда модель загружана, текстура отсутсвует.

    indicesSize = this->indicesData.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(6, m_buffers);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexData.size() * sizeof(glm::vec3), &this->vertexData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->normalData.size() * sizeof(glm::vec3), &this->normalData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEX_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->uvData.size() * sizeof(glm::vec2), &this->uvData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TANGENT_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->tangentData.size() * sizeof(glm::vec3), &this->tangentData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BITANGENT_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->bitangentData.size() * sizeof(glm::vec3), &this->bitangentData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[COLOR_VB]);
    glBufferData(GL_ARRAY_BUFFER, this->colorData.size() * sizeof(glm::vec3), &this->colorData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[IND_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesData.size() * sizeof(GLuint), &this->indicesData[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(6, m_buffers);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::Render()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<Mesh*>& Mesh::GetChildMeshes()
{
    return childMeshes;
}

std::string& Mesh::GetPath()
{
    return m_path;
}

std::string& Mesh::GetName()
{
    return m_name;
}

std::vector<glm::vec3>& Mesh::GetVertexData()
{
    return vertexData;
}

std::vector<glm::vec3>& Mesh::GetNormalData()
{
    return normalData;
}

std::vector<glm::vec2>& Mesh::GetUVData()
{
    return uvData;
}

std::vector<glm::vec3>& Mesh::GetTangentData()
{
    return tangentData;
}

std::vector<glm::vec3>& Mesh::GetBitangentData()
{
    return bitangentData;
}

std::vector<GLuint>& Mesh::GetIndicesData()
{
    return indicesData;
}

PBRMaterial Mesh::GetMaterial()
{
    return m_material;
}

int Mesh::GetIndex()
{
    return index;
}

void Mesh::SetIndex(int index)
{
    this->index = index;
}

GLuint Mesh::GetVAO()
{
    return VAO;
}

GLuint Mesh::GetIndicesSize()
{
    return indicesSize;
}
