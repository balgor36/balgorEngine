#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "mesh.h"
#include "imageLoader.h"
#include "camera.h"
#include "shader.h"

class Cubemap
{
public:
    Cubemap(ImageLoader* imageLoader, Shader* shader, std::vector<std::string> faces);
    ~Cubemap();

    void Render(Camera& camera);
private:
    std::vector<glm::vec3> vecData;
    GLuint textureID;
    std::vector<std::string> faces;

    ImageLoader* imageLoader;
    Shader* shader;
    GLuint m_idArray[3];

    GLuint VBO;
    GLuint VAO;

    const unsigned int VIEW_ID = 0;
    const unsigned int PROJECTION_ID = 1;
    const unsigned int CUBEMAP_ID = 2;
};

#endif
