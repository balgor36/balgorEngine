#ifndef ENTITY_H
#define ENTITY_H

#include <algorithm>
#include <glm/gtx/string_cast.hpp>
#include "mesh.h"
#include "sceneManager.h"
#include "window.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "cameraManager.h"
#include "game.h"
#include "component.h"
#include "projectManager.h"
#include "meshManager.h"
#include "luaManager.h"
#include "renderToTexture.h"

class Component;

class Entity
{
public:
    enum EntityType
    {
        MESH,
        CAMERA,
        LIGHT_DIR,
        LIGHT_POINT
    };

    Entity();
    Entity(EntityType m_entityType, Mesh* mesh, Camera* camera, Light* light,
           EntityManager* entityManager, Shader* shader, Shader* depthShader, CameraManager* cameraManager,
           LightManager* lightManager, Game* game, GLuint shadowMap);
    ~Entity();

    void SetVariables(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
                      glm::vec3 gpos, glm::vec3 grot, glm::vec3 gscale,
                      glm::mat4 matrix, glm::vec3 color, int parentIndex,
                      int index, bool hasParent, std::string name);

    void SetMesh(Mesh* mesh);
    Mesh& GetMesh();

    EntityType GetType();

    void SetPosition(glm::vec3 targetPosition);
    void IncreasePosition(glm::vec3 targetPosition);

    void SetRotation(glm::vec3 targetRotation);
    void IncreaseRotation(glm::vec3 targetRotation);

    void SetScale(glm::vec3 targetScale);
    void IncreaseScale(glm::vec3 targetScale);

    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();

    glm::vec3 GetGlobalPosition();
    glm::vec3 GetGlobalRotation();
    glm::vec3 GetGlobalScale();

    glm::mat4 GetTransformationMatrix();

    float* GetPositionPtr();
    float* GetRotationPtr();
    float* GetScalePtr();

    float* GetGlobalPositionPtr();
    float* GetGlobalRotationPtr();
    float* GetGlobalScalePtr();

    Light* GetLight();
    Camera* GetCamera();

    LightMaterial* GetLightMaterialPtr();
    LightMaterial GetLighMaterial();

    PBRMaterial* GetPBRMaterialPtr();
    PBRMaterial GetPBRMaterial();

    GLuint GetAlbedo();
    void SetAlbedo(GLuint m_texture);

    GLuint GetNormalMap();
    void SetNormalMap(GLuint m_texture);

    void Select();
    void UnSelect();

    int GetIndex();
    void SetIndex(int index);
    void SetLightIndex(int index);

    void Draw(Camera* camera);
    void Draw();
    void Lighting(bool isDepthShader);
    void UpdateCamera();

    std::string GetName();
    std::string& GetNamePtr();
    void SetName(char* newName);

    std::vector<Entity>& GetChildEntities();
    Entity& GetParent();
    int GetParentIndex();
    int GetLightIndex();
    void SetParent(int entityIndex);

    void StartComponent();
    void StopComponent();

    bool hasParent();

    std::vector<Component>& GetComponents();
private:
    EntityType m_type;

    Mesh* mesh;
    Light* m_light;
    Camera* m_camera;

    EntityManager* entityManager;

    Shader* shader;
    Shader* depthShader;
    CameraManager* cameraManager;
    LightManager* lightManager;
    Game* game;

    PBRMaterial m_pbrMaterial;

    int parentIndex;
    std::vector<Entity> childEntities;
    std::vector<Component> components;
    glm::mat4 transformationMatrix;

    glm::vec3 localPosition;
    glm::vec3 localRotation;
    glm::vec3 localScale;

    glm::vec3 globalPosition;
    glm::vec3 globalRotation;
    glm::vec3 globalScale;

    glm::mat4 spaceMatrix;

    float near_plane, far_plane;
    glm::mat4 lightProjection;
    glm::mat4 lightView;

    GLuint m_albedo;
    GLuint m_normalMap;
    GLuint m_shadowMap;

    std::string name;

    GLuint m_idArray[18];

    bool isSelected;
    bool m_hasParent;
    int m_index;
    int m_lightIndex;

    static const int MODEL = 0;
    static const int VIEW = 1;
    static const int PROJECTION = 2;
    static const int INPUT_COLOR = 3;
    static const int IS_ALBEDO = 4;
    static const int IS_NORMAL_MAP = 5;
    static const int IS_SELECTED = 6;
    static const int LIGHT_POS = 7;
    static const int LIGHT_COLOR = 8;
    static const int LIGHT_RANGE = 9;
    static const int LIGHT_INTENSITY = 10;
    static const int LIGHT_MAT_COLOR = 11;
    static const int CAMERA_POS = 12;
    static const int LIGHT_SPACE_MAT = 13;
    static const int PBR_MAT_ALBEDO = 14;
    static const int PBR_MAT_METALLIC = 15;
    static const int PBR_MAT_ROUGHNESS = 16;
    static const int PBR_MAT_OCCLUSION = 17;

    GLuint depthLight;
    GLuint depthModel;
};

#endif
