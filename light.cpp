#include "light.h"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

std::vector<Light>& LightManager::GetLights()
{
    return lights;
}
