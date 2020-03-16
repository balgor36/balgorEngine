#ifndef MESHDATA_H
#define MESHDATA_H

#include "window.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
};

#endif
