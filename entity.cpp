#include "entity.h"

Entity::Entity()
{
    m_albedo = m_normalMap = m_index = m_lightIndex = 0;
    localPosition = localRotation = globalPosition = globalRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    localScale = globalScale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_hasParent = false;
}

Entity::Entity(EntityType m_entityType, Mesh* mesh, Camera* camera, Light* light,
               EntityManager* entityManager, Shader* shader, Shader* depthShader, CameraManager* cameraManager,
               LightManager* lightManager, Game* game, GLuint shadowMap)
{
    m_albedo = m_normalMap = m_lightIndex = 0;
    m_shadowMap = shadowMap;
    localPosition = localRotation = globalPosition = globalRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    localScale = globalScale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_hasParent = false;

    isSelected = false;
    near_plane = 1.0f;
    far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                  glm::vec3( 0.0f, 0.0f,  0.0f),
                                  glm::vec3( 0.0f, 1.0f,  0.0f));

    m_type = m_entityType;
    this->mesh = mesh;
    this->m_camera = camera;
    this->m_light = light;
    switch(m_type)
    {
    case EntityType::MESH:        
        /*m_material = mesh->GetMaterial();
        if(m_material.ambient == glm::vec3(0.0f, 0.0f, 0.0f)){
            m_material.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        }*/
        m_pbrMaterial = mesh->GetMaterial();
        //m_pbrMaterial.albedo = glm::vec3(0.5f, 0.5f, 0.5f);
        this->name = mesh->GetName();
        break;
    case EntityType::CAMERA:
        this->name = "Camera";
        break;
    case EntityType::LIGHT_DIR:
        this->name = "Directional light";
        m_light->material = light->material;
        m_light->range = -1.0f;
        break;
    case EntityType::LIGHT_POINT:
        this->name = "Point light";
        m_light->material = light->material;
        localPosition = light->position;
        m_light->range = 1.0f;
        break;
    }

    this->shader = shader;
    this->depthShader = depthShader;
    this->cameraManager = cameraManager;
    this->lightManager = lightManager;
    this->game = game;

    this->entityManager = entityManager;

    m_idArray[MODEL] = shader->GetID("model");
    m_idArray[VIEW] = shader->GetID("view");
    m_idArray[PROJECTION] = shader->GetID("projection");
    m_idArray[INPUT_COLOR] = shader->GetID("inputColor");

    m_idArray[IS_ALBEDO] = shader->GetID("isAlbedo");
    m_idArray[IS_NORMAL_MAP] = shader->GetID("isNormalMap");
    m_idArray[IS_SELECTED] = shader->GetID("isSelected");

    m_idArray[LIGHT_POS] = shader->GetID("light.position");
    m_idArray[LIGHT_COLOR] = shader->GetID("light.color");
    m_idArray[LIGHT_RANGE] = shader->GetID("light.range");
    m_idArray[LIGHT_INTENSITY] = shader->GetID("light.intensity");
    m_idArray[LIGHT_MAT_COLOR] = shader->GetID("light.material.color");

    m_idArray[CAMERA_POS] = shader->GetID("cameraPos");

    m_idArray[LIGHT_SPACE_MAT] = shader->GetID("lightSpaceMatrix");

    m_idArray[PBR_MAT_ALBEDO] = shader->GetID("pbrMaterial.albedo");
    m_idArray[PBR_MAT_METALLIC] = shader->GetID("pbrMaterial.metallic");
    m_idArray[PBR_MAT_ROUGHNESS] = shader->GetID("pbrMaterial.roughness");
    m_idArray[PBR_MAT_OCCLUSION] = shader->GetID("pbrMaterial.occlusion");

    m_idArray[LIGHT_SPACE_MAT] = shader->GetID("lightSpaceMatrix");

    depthLight = shader->GetID("lightSpaceMatrix");
    depthModel = shader->GetID("model");

    if(m_type == EntityType::MESH)
    {
        for(GLuint i = 0; i < mesh->GetChildMeshes().size(); i++)
        {
            childEntities.emplace_back(EntityType::MESH, mesh->GetChildMeshes()[i], nullptr, nullptr, entityManager, shader, depthShader, cameraManager, lightManager, game, shadowMap);
        }
    }
}

Entity::~Entity()
{
    if(m_type == EntityType::LIGHT_DIR || m_type == EntityType::LIGHT_POINT){
        glUseProgram(shader->GetProgramID());
        glUniform3f(m_idArray[LIGHT_POS], 0, 0, 0);
        glUniform3f(m_idArray[LIGHT_COLOR], 0, 0, 0);
        glUniform1f(m_idArray[LIGHT_RANGE], 0);
        glUniform1f(m_idArray[LIGHT_INTENSITY], 0);
        glUniformMatrix4fv(m_idArray[LIGHT_SPACE_MAT], 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    }

    // Нужно удалять камеру напрямую из CameraManager.
    /*if(m_camera != nullptr)
        delete m_camera;*/
}

void Entity::SetVariables(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
                          glm::vec3 gpos, glm::vec3 grot, glm::vec3 gscale,
                          glm::mat4 matrix, glm::vec3 color, int parentIndex,
                          int index, bool hasParent, std::string name)
{
    localPosition = pos;
    localRotation = rot;
    localScale = scale;

    globalPosition = gpos;
    globalRotation = grot;
    globalScale = gscale;

    transformationMatrix = matrix;
    this->parentIndex = parentIndex;
    this->m_hasParent = hasParent;
    this->m_index = index;
    this->name = name;
}

void Entity::SetMesh(Mesh* mesh)
{
    this->mesh = mesh;
    this->name = mesh->GetName();
}

Mesh& Entity::GetMesh()
{
    return *mesh;
}

void Entity::SetPosition(glm::vec3 targetPosition)
{
    localPosition = targetPosition;
}

void Entity::IncreasePosition(glm::vec3 targetPosition)
{
    localPosition += targetPosition;
}

void Entity::SetRotation(glm::vec3 targetRotation)
{
    localRotation += targetRotation;
}

void Entity::IncreaseRotation(glm::vec3 targetRotation)
{
    localRotation += targetRotation;
}

void Entity::SetScale(glm::vec3 targetScale)
{
    localScale = targetScale;
}

void Entity::IncreaseScale(glm::vec3 targetScale)
{
    localScale += targetScale;
}

glm::vec3 Entity::GetPosition()
{
    return localPosition;
}

glm::vec3 Entity::GetRotation()
{
    return localRotation;
}

glm::vec3 Entity::GetScale()
{
    return localScale;
}

glm::vec3 Entity::GetGlobalPosition()
{
    return globalPosition;
}

glm::vec3 Entity::GetGlobalRotation()
{
    return globalRotation;
}

glm::vec3 Entity::GetGlobalScale()
{
    return globalScale;
}

glm::mat4 Entity::GetTransformationMatrix()
{
    return transformationMatrix;
}

bool Entity::hasParent()
{
    return m_hasParent;
}

float* Entity::GetPositionPtr()
{
    return glm::value_ptr(localPosition);
}

float* Entity::GetRotationPtr()
{
    return glm::value_ptr(localRotation);
}

float* Entity::GetScalePtr()
{
    return glm::value_ptr(localScale);
}

float* Entity::GetGlobalPositionPtr()
{
    return glm::value_ptr(globalPosition);
}

float* Entity::GetGlobalRotationPtr()
{
    return glm::value_ptr(globalRotation);
}

float* Entity::GetGlobalScalePtr()
{
    return glm::value_ptr(globalScale);
}

Light *Entity::GetLight()
{
    return m_light;
}

Camera* Entity::GetCamera(){
    return m_camera;
}

GLuint Entity::GetAlbedo()
{
    return m_albedo;
}

GLuint Entity::GetNormalMap()
{
    return m_normalMap;
}

void Entity::SetAlbedo(GLuint m_albedo)
{
    this->m_albedo = m_albedo;
}

void Entity::SetNormalMap(GLuint m_normalMap)
{
    this->m_normalMap = m_normalMap;
}

std::string Entity::GetName()
{
    return name;
}

std::string &Entity::GetNamePtr()
{
    return name;
}

void Entity::SetName(char* newName)
{
    name = newName;
}

std::vector<Entity>& Entity::GetChildEntities()
{
    return childEntities;
}

Entity& Entity::GetParent()
{
    return entityManager->SearchByIndex(parentIndex);
}

void Entity::SetParent(int entityIndex)
{
    parentIndex = entityIndex;
    m_hasParent = true;
}

int Entity::GetParentIndex()
{
    return parentIndex;
}

int Entity::GetIndex()
{
    return m_index;
}

void Entity::SetIndex(int index)
{
    m_index = index;
    for(GLuint i = 0; i < childEntities.size(); i++)
    {
        childEntities[i].SetParent(m_index);
    }
}

void Entity::SetLightIndex(int index){
    this->m_lightIndex = index;
}

std::vector<Component>& Entity::GetComponents()
{
    return components;
}

void Entity::StartComponent()
{
    for(GLuint i = 0; i < components.size(); i++)
    {
        components[i].SetIndex(&m_index);
        components[i].Run();

        for(GLuint i = 0; i < childEntities.size(); i++)
        {
            childEntities[i].StartComponent();
        }
    }
}

void Entity::StopComponent()
{
    for(GLuint i = 0; i < components.size(); i++)
    {
        components[i].Stop();

        for(GLuint i = 0; i < childEntities.size(); i++)
        {
            childEntities[i].StopComponent();
        }
    }
}

void Entity::Select()
{
    isSelected = true;
}

void Entity::UnSelect()
{
    isSelected = false;
}

int Entity::GetLightIndex(){
    return m_lightIndex;
}

LightMaterial* Entity::GetLightMaterialPtr(){
    return &m_light->material;
}

LightMaterial Entity::GetLighMaterial(){
    return m_light->material;
}

PBRMaterial* Entity::GetPBRMaterialPtr(){
    return &m_pbrMaterial;
}

PBRMaterial Entity::GetPBRMaterial(){
    return m_pbrMaterial;
}

void Entity::UpdateCamera(){
    if(m_hasParent)
    {
        globalPosition = localPosition + entityManager->SearchByIndex(parentIndex).GetGlobalPosition();
        globalRotation = localRotation + entityManager->SearchByIndex(parentIndex).GetGlobalRotation();
    }
    else
    {
        globalPosition = localPosition;
        globalRotation = localRotation;
    }
    //std::cout << glm::to_string(m_camera->GetPosition()) << std::endl;
    m_camera->SetPosition(globalPosition);
    m_camera->InputMove();
}

void Entity::Lighting(bool isDepthShader){
    if(!isDepthShader){
        glUseProgram(shader->GetProgramID());
        glUniform3f(m_idArray[LIGHT_POS], localPosition.x, localPosition.y, localPosition.z);
        glUniform1f(m_idArray[LIGHT_RANGE], m_light->range);
        glUniform1f(m_idArray[LIGHT_INTENSITY], m_light->intensity);
        glUniform3f(m_idArray[LIGHT_MAT_COLOR], m_light->material.color.r,  m_light->material.color.g, m_light->material.color.b);
        /*glUniform3f(m_idArray[DIR_DIFFUSE], m_lightMaterial.diffuse.x, m_lightMaterial.diffuse.y, m_lightMaterial.diffuse.z);
        glUniform3f(m_idArray[DIR_SPECULAR], m_lightMaterial.specular.x, m_lightMaterial.specular.y, m_lightMaterial.specular.z);
        glUniform1f(m_idArray[DIR_CONSTANT], constant);*/
        spaceMatrix = lightProjection * lightView;
        glUniformMatrix4fv(m_idArray[LIGHT_SPACE_MAT], 1, GL_FALSE, glm::value_ptr(spaceMatrix));
        /*if(m_type == EntityType::LIGHT_POINT){
            glUniform1f(m_idArray[DIR_LINEAR], linear);
            glUniform1f(m_idArray[DIR_QUADRATIC], quadratic);
            m_light->linear = linear;
            m_light->quadratic = quadratic;
        }
        //m_light->position = localPosition;
        m_light->specular = m_lightMaterial.specular;
        m_light->diffuse = m_lightMaterial.diffuse;
        m_light->constant = constant;*/
    }
    else{
        glUseProgram(depthShader->GetProgramID());
        spaceMatrix = lightProjection * lightView;
        glUniformMatrix4fv(depthLight, 1, GL_FALSE, glm::value_ptr(spaceMatrix));
    }
}

void Entity::Draw(){
    glUseProgram(depthShader->GetProgramID());

    if(m_hasParent)
    {
        globalPosition = localPosition + entityManager->SearchByIndex(parentIndex).GetGlobalPosition();
        globalRotation = localRotation + entityManager->SearchByIndex(parentIndex).GetGlobalRotation();
        globalScale = localScale * entityManager->SearchByIndex(parentIndex).GetGlobalScale();
    }
    else
    {
        globalPosition = localPosition;
        globalRotation = localRotation;
        globalScale = localScale;
    }

    transformationMatrix = glm::translate(glm::mat4(1.0f), globalPosition)
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.x), glm::vec3(1, 0, 0))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.y), glm::vec3(0, 1, 0))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.z), glm::vec3(0, 0, 1))
                           * glm::scale(glm::mat4(1.0f), globalScale);

    glUniformMatrix4fv(depthModel, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

    mesh->Render();

    for(GLuint i = 0; i < childEntities.size(); i++)
    {
        if(childEntities[i].GetType() == EntityType::MESH)
            childEntities[i].Draw();
    }
}

void Entity::Draw(Camera* camera)
{
    glUseProgram(shader->GetProgramID());

    glUniform3f(m_idArray[PBR_MAT_ALBEDO], m_pbrMaterial.albedo.x, m_pbrMaterial.albedo.y, m_pbrMaterial.albedo.z);
    glUniform1f(m_idArray[PBR_MAT_METALLIC], m_pbrMaterial.metallic);
    glUniform1f(m_idArray[PBR_MAT_ROUGHNESS], m_pbrMaterial.roughness);
    glUniform1f(m_idArray[PBR_MAT_OCCLUSION], m_pbrMaterial.occlusion);

    glUniform3f(m_idArray[CAMERA_POS], camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);

    if(m_albedo != 0)
    {
        glUniform1i(m_idArray[IS_ALBEDO], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_albedo);
    }

    else
    {
        glUniform1i(m_idArray[IS_ALBEDO], 0);
    }

    if(m_normalMap != 0)
    {
        glUniform1i(m_idArray[IS_NORMAL_MAP], 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_normalMap);
    }
    else
    {
        glUniform1i(m_idArray[IS_NORMAL_MAP], 0);
    }

    if(isSelected)
    {
        glUniform1i(m_idArray[IS_SELECTED], 1);
    }
    else
    {
        glUniform1i(m_idArray[IS_SELECTED], 0);
    }

    if(m_hasParent)
    {
        globalPosition = localPosition + entityManager->SearchByIndex(parentIndex).GetGlobalPosition();
        globalRotation = localRotation + entityManager->SearchByIndex(parentIndex).GetGlobalRotation();
        globalScale = localScale * entityManager->SearchByIndex(parentIndex).GetGlobalScale();
    }
    else
    {
        globalPosition = localPosition;
        globalRotation = localRotation;
        globalScale = localScale;
    }

    transformationMatrix = glm::translate(glm::mat4(1.0f), globalPosition)
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.x), glm::vec3(1, 0, 0))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.y), glm::vec3(0, 1, 0))
                           * glm::rotate(glm::mat4(1.0f), glm::radians(globalRotation.z), glm::vec3(0, 0, 1))
                           * glm::scale(glm::mat4(1.0f), globalScale);

    glUniformMatrix4fv(m_idArray[MODEL], 1, GL_FALSE, glm::value_ptr(transformationMatrix));

    // Камера, которая находится в векторе (масиве) с индексом 0 является камерой самого движка (которая отвечает за рендер сцены в окне "Design View")

    glUniformMatrix4fv(m_idArray[VIEW], 1, GL_FALSE, glm::value_ptr(camera->GetView()));
    glUniformMatrix4fv(m_idArray[PROJECTION], 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));

    mesh->Render();

    for(GLuint i = 0; i < childEntities.size(); i++)
    {
        if(childEntities[i].GetType() == EntityType::MESH)
            childEntities[i].Draw(camera);
    }
}

Entity::EntityType Entity::GetType()
{
    return m_type;
}
