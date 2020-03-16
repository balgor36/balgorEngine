#include "input.h"
#include "shader.h"
#include "entity.h"
#include "imageLoader.h"
#include "meshManager.h"
#include "entityManager.h"
#include "guiManager.h"
#include "light.h"
#include "luaManager.h"
#include "grid.h"
#include "renderToTexture.h"
#include "GUIDManager.h"
#include "cameraManager.h"
#include "sceneManager.h"
#include "cubemap.h"

int main()
{
    Window* window = new Window(1680, 973, "balgorEngine");
    Shader shader;
    Shader depthShader;
    Input* input = new Input(window);
    ImageLoader* imageLoader = new ImageLoader();
    MeshManager* meshManager = new MeshManager();
    LightManager* lightManager = new LightManager();
    ProjectManager* projectManager = new ProjectManager();
    Time* time = new Time();
    RenderToTexture renderToTexture(imageLoader);
    RenderToTexture renderToTextureGame(imageLoader);

    renderToTexture.CreateFBO(window->GetWidth(), window->GetHeight());
    renderToTextureGame.CreateFBO(window->GetWidth(), window->GetHeight());

    CameraManager* cameraManager = new CameraManager();
    Game* game = new Game();
    EntityManager* entityManager = new EntityManager(cameraManager, lightManager, game);

    game->SetEntityManager(entityManager);

    if(!window->GetResult())
    {
        return -1;
    }

    LuaManager* luaManager = new LuaManager(entityManager);

    SceneManager* sceneManager = new SceneManager(entityManager, meshManager, imageLoader, game,
            luaManager, &shader, cameraManager, lightManager);


    GUIManager* guiManager = new GUIManager(window, input, entityManager, lightManager, meshManager, imageLoader,
                                            &shader, &depthShader, projectManager, luaManager, &renderToTexture, &renderToTextureGame,
                                            game, cameraManager, sceneManager, renderToTexture.GetShadowMap());

    guiManager->InitGUI();
    input->SetIO();

    cameraManager->GetCameras().push_back(new Camera(window, input, time, false));  // камера вьюпорта
    cameraManager->GetCameras().push_back(new Camera(window, input, time, true));   // игровая камера
    //cameraManager->GetCameras()[1]->SetPosition(glm::vec3(0, 0, 0));

    shader.InitShader("vertex.glsl", "fragment.glsl", false);
    shader.RegisterID("model");
    shader.RegisterID("view");
    shader.RegisterID("projection");
    shader.RegisterID("inputColor");

    shader.RegisterID("isAlbedo");
    shader.RegisterID("isNormalMap");
    shader.RegisterID("isSelected");

    shader.RegisterID("light.position");
    shader.RegisterID("light.color");
    shader.RegisterID("light.range");
    shader.RegisterID("light.intensity");
    shader.RegisterID("light.material.color");

    shader.RegisterID("cameraPos");

    shader.RegisterID("pbrMaterial.albedo");
    shader.RegisterID("pbrMaterial.metallic");
    shader.RegisterID("pbrMaterial.roughness");
    shader.RegisterID("pbrMaterial.occlusion");

    shader.RegisterID("lightSpaceMatrix");

    depthShader.InitShader("depthShaderVS.glsl", "depthShaderFS.glsl", false);
    depthShader.RegisterID("lightSpaceMatrix");
    depthShader.RegisterID("model");

    LuaScript testScript("script.lua", luaManager);

    luaManager->Init();
    luaManager->GetLuaState()["entityManager"] = entityManager;
    luaManager->GetLuaState()["input"] = input;
    luaManager->GetLuaState()["time"] = time;
    luaManager->GetLuaState()["gameCamera"] = cameraManager->GetCameras()[1];

    Shader gridShader;
    gridShader.InitShader("gridVS.glsl", "gridFS.glsl", false);
    gridShader.RegisterID("model");
    gridShader.RegisterID("view");
    gridShader.RegisterID("projection");

    Grid grid = Grid(16, &gridShader);

    Camera& mainCamera = *cameraManager->GetCameras()[0];
    Camera& gameCamera = *cameraManager->GetCameras()[1];

    Shader cubemapShader;
    cubemapShader.InitShader("skyboxVS.glsl", "skyboxFG.glsl", false);
    cubemapShader.RegisterID("view");
    cubemapShader.RegisterID("projection");
    cubemapShader.RegisterID("cubemap");

    std::vector<std::string> faces = {"skyboxes/hw_morning/morning_rt.png",
                                      "skyboxes/hw_morning/morning_lf.png",
                                      "skyboxes/hw_morning/morning_up.png",
                                      "skyboxes/hw_morning/morning_dn.png",
                                      "skyboxes/hw_morning/morning_bk.png",
                                      "skyboxes/hw_morning/morning_ft.png", };
    Cubemap cubemap(imageLoader, &cubemapShader, faces);

    Entity entityGameCamera(Entity::EntityType::CAMERA, nullptr, &gameCamera, nullptr,
                      entityManager, &shader, &depthShader, cameraManager,
                      lightManager, game, 0);
    entityManager->GetEntities().push_back(entityGameCamera);

    Entity& cameraEntity = entityManager->GetEntities()[0];
    cameraEntity.SetIndex(0);

    while(window->isClosed() != 1)
    {
        time->Reset();
        /////////////// DESIGN RENDERING ///////////////
        if(game->isPlay()){
            renderToTextureGame.StartRender();
            window->Clear();
            cubemap.Render(gameCamera);
            //gameCamera.InputMove();
            if(game->isPlay())
            {
                entityManager->RunComponents();
            }

            entityManager->RenderEntities(cameraEntity.GetCamera(), false);
            renderToTextureGame.StopRender();
        }

        else{
            renderToTexture.StartRender();
            window->Clear();
            cubemap.Render(mainCamera);
            mainCamera.InputMove();
            grid.DrawGrid(cameraManager);
            entityManager->RenderEntities(&mainCamera, false);
            renderToTexture.StopRender();
        }

        /////////////// GAME RENDERING /////////////////
        /*renderToTextureGame->StartRender();
        window->Clear();
        cubemap.Render(gameCamera);
        gameCamera.InputMove();
        if(game->isPlay())
        {
            entityManager->RunComponents();
        }
        entityManager->RenderEntities(true);
        renderToTextureGame->StopRender();*/

        /////////////// GUI RENDERING //////////////////
        window->Clear();
        guiManager->Render();
        window->Update();
    }

    delete sceneManager;
    delete luaManager;
    delete entityManager;
    delete game;
    delete cameraManager;
    delete time;
    delete projectManager;
    delete lightManager;
    delete meshManager;
    delete imageLoader;
    delete input;
    window->Release();
    delete window;
    delete guiManager;
    return 0;
}
