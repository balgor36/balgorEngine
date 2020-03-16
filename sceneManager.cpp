#include "sceneManager.h"

SceneManager::SceneManager(EntityManager* entityManager, MeshManager* meshManager, ImageLoader* imageLoader, Game* game, LuaManager* luaManager, Shader* shader, CameraManager* cameraManager, LightManager* lightManager)
{
    this->entityManager = entityManager;
    this->meshManager = meshManager;
    this->imageLoader = imageLoader;
    this->game = game;
    this->luaManager = luaManager;
    this->shader = shader;
    this->cameraManager = cameraManager;
    this->lightManager = lightManager;
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Save(const char* filename)
{
    std::ofstream out(filename, std::ios::binary);
    if(!out.is_open())
    {
        std::cout << "Failed to save the file: " << filename << std::endl;
        return false;
    }

//    Serialize(out, meshManager->GetMeshes());
    Serialize(out, entityManager->GetEntities());
    out.close();
    return true;
}

bool SceneManager::Load(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open())
    {
        std::cout << "Failed to load the file: " << filename << std::endl;
        return false;
    }

//    Deserialize(in, meshManager->GetMeshes());
    Deserialize(in, entityManager->GetEntities());
    in.close();
    return true;
}

void SceneManager::Serialize(std::ofstream& out, Mesh& mesh)
{
    size_t nameSize = mesh.GetName().size();
    size_t pathSize = mesh.GetPath().size();
    size_t vertexSize = mesh.GetVertexData().size();
    size_t normalSize = mesh.GetNormalData().size();
    size_t uvSize = mesh.GetUVData().size();
    size_t tangentSize = mesh.GetTangentData().size();
    size_t bitangentSize = mesh.GetBitangentData().size();
    size_t indicesSize = mesh.GetIndicesData().size();

    out.write((const char*)&nameSize, sizeof(nameSize));
    out.write((const char*)&mesh.GetName()[0], nameSize);

    out.write((const char*)&pathSize, sizeof(pathSize));
    out.write((const char*)&mesh.GetPath()[0], pathSize);

    out.write((const char*)&vertexSize, sizeof(vertexSize));
    out.write((const char*)&mesh.GetVertexData()[0], vertexSize * sizeof(glm::vec3));

    out.write((const char*)&normalSize, sizeof(normalSize));
    out.write((const char*)&mesh.GetNormalData()[0], normalSize * sizeof(glm::vec3));

    out.write((const char*)&uvSize, sizeof(uvSize));
    out.write((const char*)&mesh.GetUVData()[0], uvSize * sizeof(glm::vec2));

    out.write((const char*)&tangentSize, sizeof(tangentSize));
    out.write((const char*)&mesh.GetTangentData()[0], tangentSize * sizeof(glm::vec3));

    out.write((const char*)&bitangentSize, sizeof(bitangentSize));
    out.write((const char*)&mesh.GetBitangentData()[0], bitangentSize * sizeof(glm::vec3));

    out.write((const char*)&indicesSize, sizeof(indicesSize));
    out.write((const char*)&mesh.GetIndicesData()[0], indicesSize * sizeof(GLuint));

    size_t subsetSize = mesh.GetChildMeshes().size();
    out.write((const char*)&subsetSize, sizeof(subsetSize));

    for(int i = 0; i < mesh.GetChildMeshes().size(); i++)
    {
        Serialize(out, *mesh.GetChildMeshes()[i]);
    }
}

void SceneManager::Serialize(std::ofstream& out, std::vector<Mesh*>& m_meshes)
{
    size_t meshesSize = m_meshes.size();
    out.write((const char*)&meshesSize, sizeof(meshesSize));
    for(size_t i = 0; i < m_meshes.size(); i++)
    {
        Serialize(out, *m_meshes[i]);
    }
}

void SceneManager::Deserialize(std::ifstream& in, std::vector<Mesh*>& outMeshes)
{
    size_t meshesSize;
    in.read((char*)&meshesSize, sizeof(meshesSize));
    outMeshes.resize(meshesSize);

    for(size_t i = 0; i < meshesSize; i++)
    {
        Deserialize(in, *outMeshes[i]);
    }
}

void SceneManager::Deserialize(std::ifstream& in, Mesh& outMesh)
{
    size_t nameSize;
    size_t pathSize;
    size_t vertexSize;
    size_t normalSize;
    size_t uvSize;
    size_t tangentSize;
    size_t bitangentSize;
    size_t indicesSize;

    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec3> normalData;
    std::vector<glm::vec2> uvData;

    std::vector<glm::vec3> tangentData;
    std::vector<glm::vec3> bitangentData;
    std::vector<GLuint> indicesData;

    std::string name;
    std::string path;

    in.read((char*)&nameSize, sizeof(nameSize));
    name.resize(nameSize);
    in.read((char*)&name[0], nameSize);

    in.read((char*)&pathSize, sizeof(pathSize));
    path.resize(pathSize);
    in.read((char*)&path[0], pathSize);
    in.read((char*)&vertexSize, sizeof(vertexSize));
    vertexData.resize(vertexSize);
    in.read((char*)&vertexData[0], vertexSize * sizeof(glm::vec3));
    in.read((char*)&normalSize, sizeof(normalSize));
    normalData.resize(normalSize);
    in.read((char*)&normalData[0], normalSize * sizeof(glm::vec3));

    in.read((char*)&uvSize, sizeof(uvSize));
    uvData.resize(uvSize);
    in.read((char*)&uvData[0], uvSize * sizeof(glm::vec2));

    in.read((char*)&tangentSize, sizeof(tangentSize));
    tangentData.resize(tangentSize);
    in.read((char*)&tangentData[0], tangentSize * sizeof(glm::vec3));

    in.read((char*)&bitangentSize, sizeof(bitangentSize));
    bitangentData.resize(bitangentSize);
    in.read((char*)&bitangentData[0], bitangentSize * sizeof(glm::vec3));

    in.read((char*)&indicesSize, sizeof(indicesSize));
    indicesData.resize(indicesSize);
    in.read((char*)&indicesData[0], indicesSize * sizeof(GLuint));

    outMesh = Mesh(&vertexData, &normalData, &uvData, &tangentData, &bitangentData, NULL, &indicesData, &path, &name, NULL);

    size_t subsetSize;
    in.read((char*)&subsetSize, sizeof(subsetSize));
    outMesh.GetChildMeshes().resize(subsetSize);


    for(size_t i = 0; i < subsetSize; i++)
    {
        Deserialize(in, *outMesh.GetChildMeshes()[i]);
    }
}

void SceneManager::Serialize(std::ofstream& out, Component& m_component)
{
    std::string name = m_component.GetName();
    int scriptIndex = m_component.GetLuaScript()->GetScriptIndex();

    size_t nameSize = name.size();
    out.write((const char*)&nameSize, sizeof(size_t));
    out.write((const char*)&name[0], nameSize);

    out.write((const char*)&scriptIndex, sizeof(int));
}

void SceneManager::Serialize(std::ofstream& out, std::vector<Component>& m_components)
{
    size_t componentsSize = m_components.size();
    out.write((const char*)&componentsSize, sizeof(size_t));

    for(size_t i = 0; i < componentsSize; i++)
    {
        Serialize(out, m_components[i]);
    }
}

void SceneManager::Deserialize(std::ifstream& in, std::vector<Entity>& outEntity)
{
    size_t entitiesSize;
    in.read((char*)&entitiesSize, sizeof(size_t));
    outEntity.resize(entitiesSize);

    for(size_t i = 0; i < entitiesSize; i++)
    {
        Deserialize(in, outEntity[i]);
    }
}

void SceneManager::Deserialize(std::ifstream& in, Entity& outEntity)
{
    int parentIndex, index;
    bool hasParent;
    std::string name;

    glm::vec3 pos, rot, scale, gpos, grot, gscale, color;
    glm::mat4 matrix;

    in.read((char*)&parentIndex, sizeof(int));
    in.read((char*)&index, sizeof(int));

    in.read((char*)&hasParent, sizeof(bool));

    size_t nameSize;
    in.read((char*)&nameSize, sizeof(size_t));
    name.resize(nameSize);
    in.read((char*)&name[0], nameSize);

    in.read((char*)&matrix, sizeof(glm::mat4));

    in.read((char*)&pos, sizeof(glm::vec3));
    in.read((char*)&rot, sizeof(glm::vec3));
    in.read((char*)&scale, sizeof(glm::vec3));

    in.read((char*)&gpos, sizeof(glm::vec3));
    in.read((char*)&grot, sizeof(glm::vec3));
    in.read((char*)&gscale, sizeof(glm::vec3));

    in.read((char*)&color, sizeof(glm::vec3));

    int meshIndex;
    in.read((char*)&meshIndex, sizeof(int));

    std::vector<Component> tmpComponents;
    Deserialize(in, tmpComponents);

    std::vector<Entity> childEntities;
    size_t subsets;
    in.read((char*)&subsets, sizeof(size_t));
    if(subsets > 0)
    {
        childEntities.resize(subsets);
        for(size_t i = 0; i < subsets; i++)
        {
            Deserialize(in, childEntities[i]);
        }
    }
    Mesh* meshByIdx = meshManager->GetMeshByIndex(meshIndex);
    assert(meshByIdx != NULL);

    //outEntity = Entity(Entity::EntityType::MESH, meshByIdx, NULL, NULL, entityManager, shader, depthShader, cameraManager, lightManager, game);
    outEntity.SetVariables(pos, rot, scale, gpos, grot, gscale, matrix, color, parentIndex, index, hasParent, name);

    outEntity.GetComponents() = tmpComponents;
    if(subsets > 0)
        outEntity.GetChildEntities() = childEntities;
}

void SceneManager::Deserialize(std::ifstream& in, std::vector<Component>& outComponents)
{
    size_t componentsSize;
    in.read((char*)&componentsSize, sizeof(size_t));
    outComponents.resize(componentsSize);

    for(size_t i = 0; i < componentsSize; i++)
    {
        Deserialize(in, outComponents[i]);
    }
}

void SceneManager::Deserialize(std::ifstream& in, Component& outComponent)
{
    std::string name;
    int scriptIndex;
    size_t nameSize;

    in.read((char*)&nameSize, sizeof(size_t));
    in.read((char*)&name[0], nameSize);
    in.read((char*)&scriptIndex, sizeof(int));

    outComponent = Component(scriptIndex, name, game, luaManager);
}

void SceneManager::Serialize(std::ofstream& out, std::vector<Entity>& m_entities)
{
    size_t entitiesSize = m_entities.size();
    out.write((const char*)&entitiesSize, sizeof(size_t));

    for(size_t i = 0; i < entitiesSize; i++)
    {
        Serialize(out, m_entities[i]);
    }
}

void SceneManager::Serialize(std::ofstream& out, Entity& m_entity)
{
    int parentIndex, index;
    bool hasParent;
    std::string name;

    name = m_entity.GetName();
    parentIndex = m_entity.GetParentIndex();
    index = m_entity.GetIndex();
    hasParent = m_entity.hasParent();

    glm::vec3 pos, rot, scale, gpos, grot, gscale, color;
    glm::mat4 matrix;

    pos = m_entity.GetPosition();
    rot = m_entity.GetRotation();
    scale = m_entity.GetScale();

    gpos = m_entity.GetGlobalPosition();
    grot = m_entity.GetGlobalRotation();
    gscale = m_entity.GetGlobalScale();

    matrix = m_entity.GetTransformationMatrix();

    out.write((const char*)&parentIndex, sizeof(int));
    out.write((const char*)&index, sizeof(int));

    out.write((const char*)&hasParent, sizeof(bool));

    size_t nameSize = name.size();
    out.write((const char*)&nameSize, sizeof(size_t));
    out.write((const char*)&name[0], nameSize);

    out.write((const char*)&matrix, sizeof(glm::mat4));

    out.write((const char*)&pos, sizeof(glm::vec3));
    out.write((const char*)&rot, sizeof(glm::vec3));
    out.write((const char*)&scale, sizeof(glm::vec3));

    out.write((const char*)&gpos, sizeof(glm::vec3));
    out.write((const char*)&grot, sizeof(glm::vec3));
    out.write((const char*)&gscale, sizeof(glm::vec3));

    out.write((const char*)&color, sizeof(glm::vec3));

    int meshIndex = m_entity.GetMesh().GetIndex();
    out.write((const char*)&meshIndex, sizeof(int));

    Serialize(out, m_entity.GetComponents());

    std::vector<Entity>& childEntities = m_entity.GetChildEntities();

    size_t subsets = childEntities.size();
    out.write((const char*)&subsets, sizeof(size_t));

    for(size_t i = 0; i < subsets; i++)
    {
        Serialize(out, childEntities[i]);
    }
}
