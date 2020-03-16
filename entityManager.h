#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include "shader.h"
#include "entity.h"
#include "cameraManager.h"
#include "light.h"

class Game;
class Entity;

class EntityManager
{
public:
    EntityManager(CameraManager* cameraManager, LightManager* lightManager, Game* game);
    ~EntityManager();

    void RenderEntities(Camera* camera, bool isDepthShader);
    void RunComponents();
    void StopComponents();
    std::vector<Entity>& GetEntities();

    Entity& SearchByIndex(int index);
    int GetIndexByEntityIndex(int index);
private:
    std::vector<Entity> entities;
    bool isFounded = false;
    Entity& RecursiveSearch(int& index, Entity& targetEntity);

    GLuint m_size;

    Entity* returnedEntity;

    CameraManager* cameraManager;
    LightManager* lightManager;
    Game* game;
};

#endif
