#ifndef GRID_H
#define GRID_H

#include "window.h"
#include "mesh.h"
#include "cameraManager.h"
#include "shader.h"
#include "entity.h"

class Grid
{
public:
    Grid(int size, Shader* shader);
    ~Grid();

    void DrawGrid(CameraManager* cameraManager);
    void InitGrid(int size);
private:
    GLuint VAO;
    GLuint VBO;
    GLuint* m_idArray;

    GLuint vertexDataSize;

    Shader* shader;

    const int MODEL_ID = 0;
    const int VIEW_ID = 1;
    const int PROJECTION_ID = 2;
};

#endif
