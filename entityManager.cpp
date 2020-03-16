#include "entityManager.h"

EntityManager::EntityManager(CameraManager* cameraManager, LightManager* lightManager, Game* game)
{
    m_size = 0;

    this->cameraManager = cameraManager;
    this->lightManager = lightManager;
    this->game = game;
}

EntityManager::~EntityManager()
{

}

std::vector<Entity>& EntityManager::GetEntities()
{
    return entities;
}

Entity& EntityManager::RecursiveSearch(int& index, Entity& targetEntity)
{
    for(GLuint i = 0; i < targetEntity.GetChildEntities().size(); i++)
    {
        if(targetEntity.GetChildEntities()[i].GetIndex() == index)
        {
            returnedEntity = &targetEntity.GetChildEntities()[i];
            return *returnedEntity;
        }
    }
    if(targetEntity.GetChildEntities().size() != 0)
    {
        for(GLuint i = 0; i < targetEntity.GetChildEntities().size(); i++)
        {
            return RecursiveSearch(index, targetEntity.GetChildEntities()[i]);
        }
    }
    return *returnedEntity;
}

Entity& EntityManager::SearchByIndex(int index)
{
    returnedEntity = NULL;
    for(GLuint i = 0; i < entities.size(); i++)
    {
        if(entities[i].GetIndex() == index)
        {
            return entities[i];
        }
    }
    for(GLuint i = 0; i < entities.size(); i++)
    {
        if(!isFounded && entities[i].GetChildEntities().size() != 0)
        {
            RecursiveSearch(index, entities[i]);
        }
    }
    //assert(returnedEntity != NULL);
    return *returnedEntity;
}

int EntityManager::GetIndexByEntityIndex(int index)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i].GetIndex() == index)
        {
            std::cout << "Founded index is " << i << std::endl;
            return i;
        }
    }
    return 0;
}

void EntityManager::RenderEntities(Camera* camera, bool isDepthShader)
{
    for(GLuint i = 0; i < entities.size(); i++)
    {
        switch(entities[i].GetType()){
            case Entity::EntityType::MESH:
                //if(!isDepthShader)
                    entities[i].Draw(camera);
                //else
                //    entities[i].Draw();
                break;
            case Entity::EntityType::CAMERA:
                entities[i].UpdateCamera();
                break;
            case Entity::EntityType::LIGHT_DIR:
                entities[i].Lighting(isDepthShader);
                //entities[i].Lighting(false);
                break;
            case Entity::EntityType::LIGHT_POINT:
                if(!isDepthShader)
                    entities[i].Lighting(false);
                break;
        }
    }
}

void EntityManager::RunComponents()
{
    for(GLuint i = 0; i < entities.size(); i++)
    {
        entities[i].StartComponent();
    }
}

void EntityManager::StopComponents()
{
    for(GLuint i = 0; i < entities.size(); i++)
    {
        entities[i].StopComponent();
    }
}
