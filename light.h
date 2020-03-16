#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "meshData.h"

struct LightMaterial{
    glm::vec3 color;
};

struct Light
{
    glm::vec3 position;

    LightMaterial material;
    float range;
    float intensity;
};


class LightManager
{
public:
    LightManager();
    ~LightManager();

    std::vector<Light>& GetLights();
private:
    std::vector<Light> lights;
};

#endif
