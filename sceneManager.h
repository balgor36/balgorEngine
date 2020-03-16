#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "component.h"
#include "meshManager.h"
#include "imageLoader.h"

#include <fstream>

class Component;
class LuaManager;
class Game;
class EntityManager;
class Shader;
class CameraManager;
class LightManager;
class Entity;

class SceneManager
{
public:
    SceneManager(EntityManager* entityManager, MeshManager* meshManager, ImageLoader* imageLoader, Game* game,
                 LuaManager* luaManager, Shader* shader, CameraManager* cameraManager, LightManager* lightManager);
    ~SceneManager();

    bool Save(const char* filename);
    bool Load(const char* filename);
private:
    EntityManager* entityManager;
    MeshManager* meshManager;
    ImageLoader* imageLoader;
    Game* game;
    LuaManager* luaManager;
    Shader* shader;
    CameraManager* cameraManager;
    LightManager* lightManager;

    void Serialize(std::ofstream& out, Entity& m_entity);
    void Serialize(std::ofstream& out, Mesh& m_mesh);
    void Serialize(std::ofstream& out, Component& component);

    void Serialize(std::ofstream& out, std::vector<Mesh*>& m_meshes);
    void Serialize(std::ofstream& out, std::vector<Entity>& m_entities);
    void Serialize(std::ofstream& out, std::vector<Component>& m_components);

    void Deserialize(std::ifstream& in, std::vector<Mesh*>& outMeshes);
    void Deserialize(std::ifstream& in, std::vector<Entity>& outEntities);
    void Deserialize(std::ifstream& in, std::vector<Component>& m_components);

    void Deserialize(std::ifstream& in, Mesh& outMesh);
    void Deserialize(std::ifstream& in, Entity& outEntity);
    void Deserialize(std::ifstream& in, Component& outComponent);
};

#endif
