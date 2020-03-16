#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#define MODEL_FILES 0
#define IMAGE_FILES 1
#define LUA_SCRIPTS 2

#include <string>
#include <nfd.h>

#include "entity.h"
#include "input.h"
#include "cameraManager.h"
#include "light.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class GUIManager
{
public:
    GUIManager(Window* window, Input* input, EntityManager* entityManager, LightManager* lightManager, MeshManager* meshManager,
               ImageLoader* imageLoader, Shader* shader, Shader* depthShader, ProjectManager* projectManager, LuaManager* luaManager,
               RenderToTexture* renderToTexture, RenderToTexture* renderToTextureGame, Game* game,
               CameraManager* cameraManager, SceneManager* sceneManager, GLuint shadowMap);
    ~GUIManager();

    void InitGUI();
    void Render();

    ImVec2 GetViewportSize();
private:
    Entity tmp;

    bool FileBrowsing(int mode);
    void ImportMesh();
    void ImportScript();

    void AddEntity(Entity::EntityType m_type);
    void SetAlbedo();
    void SetNormalMap();
    void SetComponent();

    void ShowProjectStructure();
    void ImportImage(GLint type);

    void AddIndexRecursive(Entity& entity);
    void ShowEntityStructure(Entity& entity);
    void ShowStatusBar(const char* text);
    void ShowGameSettings();

    void RenderTexture();

    void About();

    int selectableEntity;
    //int selectableMesh;
    //int selectableCamera;
    int localTmpIndex = -1;
    int lastSelectableEntity;

    ImVec2 lastViewport;

    nfdchar_t *outPath = NULL;
    nfdresult_t result;

    bool m_addEntity, m_setAlbedo, m_about, m_setNormalMap, m_addComponent, m_gameSettings;

    bool m_Scene;
    bool m_first;
    bool isNullEntity;
    bool isSearching;

    std::string statusMsg = "Ready";

    ImVec2 viewportSize;
    std::vector<Entity>* entities;

    Entity::EntityType m_currType;

    GLuint shadowMap;

    Window* window;
    Input* input;
    EntityManager* entityManager;
    LightManager* lightManager;
    MeshManager* meshManager;
    ImageLoader* imageLoader;
    Shader* shader;
    Shader* depthShader;
    ProjectManager* projectManager;
    LuaManager* luaManager;
    RenderToTexture* renderToTexture;
    RenderToTexture* renderToTextureGame;
    Game* game;
    CameraManager* cameraManager;
    SceneManager* sceneManager;
};

#endif
