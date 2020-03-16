#include "grid.h"

Grid::Grid(int size, Shader* shader)
{
    this->shader = shader;

    m_idArray = new GLuint[3];
    m_idArray[MODEL_ID] = shader->GetID("model");
    m_idArray[VIEW_ID] = shader->GetID("view");
    m_idArray[PROJECTION_ID] = shader->GetID("projection");

    InitGrid(size);

}

Grid::~Grid()
{
    delete[] m_idArray;
}

void Grid::InitGrid(int size)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    std::vector<glm::vec3> vertexData;
    /*for(int i = -1; i <= 9; i++){
        vertexData.push_back(glm::vec3(i+1, 0, 5));
        vertexData.push_back(glm::vec3(i+1, 0, -5));
    }
    for(int i = -4; i <= 9; i++){
        vertexData.push_back(glm::vec3(10, 0, i-1));
        vertexData.push_back(glm::vec3(0, 0, i-1));
    }*/

    for(int x = 0; x <= size; x++)
    {
        vertexData.push_back(glm::vec3(x-size/2, 0, -size/2));
        vertexData.push_back(glm::vec3(x-size/2, 0, size/2));
    }
    for(int z = 0; z <= size; z++)
    {
        vertexData.push_back(glm::vec3(-size/2, 0, z-size/2));
        vertexData.push_back(glm::vec3(size/2, 0, z-size/2));
    }

    vertexDataSize = vertexData.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}

void Grid::DrawGrid(CameraManager* cameraManager)
{
    glUseProgram(shader->GetProgramID());

    glm::mat4 matrix(1.0);

    glUniformMatrix4fv((GLuint const)m_idArray[MODEL_ID], 1, GL_FALSE, glm::value_ptr(matrix));
    glUniformMatrix4fv((GLuint const)m_idArray[VIEW_ID], 1, GL_FALSE, glm::value_ptr(cameraManager->GetCameras()[0]->GetView()));
    glUniformMatrix4fv((GLuint const)m_idArray[PROJECTION_ID], 1, GL_FALSE, glm::value_ptr(cameraManager->GetCameras()[0]->GetProjection()));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexDataSize);
    glBindVertexArray(0);
}
