#include "guiManager.h"

GUIManager::GUIManager(Window* window, Input* input, EntityManager* entityManager, LightManager* lightManager,
                       MeshManager* meshManager, ImageLoader* imageLoader, Shader* shader, Shader* depthShader,
                       ProjectManager* projectManager, LuaManager* luaManager, RenderToTexture* renderToTexture,
                       RenderToTexture* renderToTextureGame, Game* game, CameraManager* cameraManager, SceneManager* sceneManager,
                       GLuint shadowMap)
{
    selectableEntity = lastSelectableEntity = localTmpIndex = 0;
    isNullEntity = 0;
    m_addEntity = m_setAlbedo = m_setNormalMap = m_addComponent = m_gameSettings = m_about = false;
    m_first = true;
    isSearching = false;
    m_Scene = false;

    this->window = window;
    this->input = input;
    this->entityManager = entityManager;
    this->lightManager = lightManager;
    this->meshManager = meshManager;
    this->imageLoader = imageLoader;
    this->shader = shader;
    this->depthShader = depthShader;
    this->projectManager = projectManager;
    this->luaManager = luaManager;
    this->renderToTexture = renderToTexture;
    this->renderToTextureGame = renderToTextureGame;
    this->game = game;
    this->cameraManager = cameraManager;
    this->sceneManager = sceneManager;
    this->shadowMap = shadowMap;

    entities = &entityManager->GetEntities();
}

GUIManager::~GUIManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if(outPath != NULL)
        free(outPath);
}

void GUIManager::InitGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void GUIManager::RenderTexture()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin("Design View");

    ImVec2 size = ImGui::GetContentRegionAvail();
    viewportSize = size;
    if(size.x != lastViewport.x || size.y != lastViewport.y)
    {
        if(!game->isPlay())
            renderToTexture->CreateFBO((int)size.x, (int)size.y);
        else
            renderToTextureGame->CreateFBO((int)size.x, (int)size.y);
        lastViewport = size;
    }
    if(game->isPlay()){
        ImGui::Image((void*)renderToTextureGame->GetRenderedTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
    }
    else
        ImGui::Image((void*)renderToTexture->GetRenderedTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}

void GUIManager::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static ImGuiID dockspaceID = ImGui::GetID("Dockspace");

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MyDockspaceWindow", &m_Scene, window_flags);
    ImGui::DockSpace(dockspaceID);
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::ShowDemoWindow((bool*)true);

    if(m_addEntity)
    {
        AddEntity(m_currType);
    }

    if(m_setAlbedo)
    {
        SetAlbedo();
    }
    if(m_setNormalMap)
    {
        SetNormalMap();
    }
    if(m_addComponent)
    {
        SetComponent();
    }
    if(m_gameSettings){
        ShowGameSettings();
    }

    if(m_about)
    {
        About();
    }

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Save scene"))
            {
                if(sceneManager->Save("scene.bg"))
                    std::cout << "Scene was saved successfully." << std::endl;
            }
            if(ImGui::MenuItem("Load scene"))
            {
                if(sceneManager->Load("scene.bg"))
                    std::cout << "Scene was loaded successfully." << std::endl;
            }
            ImGui::Separator();

            if(ImGui::MenuItem("Import mesh"))
            {
                // Открытые файла (импорт меша)
                if(FileBrowsing(MODEL_FILES))
                {
                    ImportMesh();
                }
            }

            if(ImGui::MenuItem("Import diffuse texture"))
            {
                ImportImage(DIFFUSE);
            }
            if(ImGui::MenuItem("Import normal map"))
            {
                ImportImage(NORMAL);
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Edit"))
        {
            if(ImGui::MenuItem("Game Setings"))
            {
                m_gameSettings = true;
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Entity"))
        {
            if(ImGui::MenuItem("Add entity"))
            {
                m_addEntity = true;
                m_currType = Entity::EntityType::MESH;
            }
            ImGui::Separator();
            if(ImGui::BeginMenu("Light"))
            {
                if(ImGui::MenuItem("Directional Light"))
                {
                    m_addEntity = true;
                    m_currType = Entity::EntityType::LIGHT_DIR;
                }
                if(ImGui::MenuItem("Point Light"))
                {
                    m_addEntity = true;
                    m_currType = Entity::EntityType::LIGHT_POINT;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Scripting")){
            if(ImGui::MenuItem("Import a script")){
                if(FileBrowsing(LUA_SCRIPTS))
                {
                    ImportScript();
                }
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Game"))
        {
            if(ImGui::MenuItem("Play"))
            {
                game->Start();
                statusMsg = "PLAYED";
            }
            if(ImGui::MenuItem("Stop"))
            {
                game->Stop();
                statusMsg = "STOPED";
                input->ShowCursor();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("About"))
            {
                m_about = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Scene");
    for(GLuint i = 0; i < entities->size(); i++)
        ShowEntityStructure(entityManager->GetEntities()[i]);

    // Проверка нажатия правой кнопки мыши
    if(ImGui::GetIO().MouseClicked[1])
    {
        ImGui::OpenPopup("Menu");
    }

    if(ImGui::BeginPopupContextItem("Menu"))
    {
        if(ImGui::Button("Add mesh"))
        {
            // Открытые файла (импорт меша)
            if(FileBrowsing(MODEL_FILES))
            {
                ImportMesh();
            }
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    RenderTexture();

    ImGui::Begin("Properties");
    if(entities->size() > 0 && isSearching)
    {
        Entity& searchedEntity = entityManager->SearchByIndex(selectableEntity);
        assert(&searchedEntity != NULL);
        ImGui::Text(searchedEntity.GetName().c_str());

        std::string index_text = "Index: " + std::to_string(selectableEntity);
        if(searchedEntity.GetType() == Entity::EntityType::LIGHT_DIR || searchedEntity.GetType() == Entity::EntityType::LIGHT_POINT)
        {
            std::string light_index = "Light index: " + std::to_string(searchedEntity.GetLightIndex());
            ImGui::Text(light_index.c_str());
        }

        ImGui::Text(index_text.c_str());
        if(ImGui::Button("Delete"))
        {
            if(searchedEntity.GetType() == Entity::EntityType::LIGHT_DIR || searchedEntity.GetType() == Entity::EntityType::LIGHT_POINT)
                lightManager->GetLights().erase(lightManager->GetLights().begin() + searchedEntity.GetLightIndex());
            entities->erase(entityManager->GetEntities().begin() + entityManager->GetIndexByEntityIndex(selectableEntity));
            isSearching = false;
            m_first = true;
        }

        if (ImGui::CollapsingHeader("Transform"))
        {
            switch(searchedEntity.GetType())
            {
                case Entity::EntityType::MESH:
                {
                    ImGui::DragFloat3("Position", searchedEntity.GetPositionPtr(), 0.01f);
                    ImGui::DragFloat3("Rotation", searchedEntity.GetRotationPtr(), 0.01f);
                    ImGui::DragFloat3("Scale", searchedEntity.GetScalePtr(), 0.01f);
                    break;
                }
                case Entity::EntityType::CAMERA:
                {
                    ImGui::DragFloat3("Position", searchedEntity.GetPositionPtr(), 0.01f);
                    ImGui::DragFloat3("Rotation", searchedEntity.GetRotationPtr(), 0.01f);
                    break;
                }
                case Entity::EntityType::LIGHT_DIR:
                {
                    ImGui::DragFloat3("Direction", searchedEntity.GetPositionPtr(), 0.01f);
                    break;
                }
                case Entity::EntityType::LIGHT_POINT:
                {
                    ImGui::DragFloat3("Position", searchedEntity.GetPositionPtr(), 0.01f);
                    break;
                }
            }
        }
        switch(searchedEntity.GetType())
        {
            case Entity::EntityType::MESH:
            {
                if(ImGui::CollapsingHeader("Material"))
                {
                    PBRMaterial* tmpMaterial = searchedEntity.GetPBRMaterialPtr();
                    /*ImGui::ColorEdit3("Ambient", (float*)searchedEntity.GetAmbientPtr(), 0);
                    ImGui::ColorEdit3("Diffuse", (float*)searchedEntity.GetDiffusePtr(), 0);
                    ImGui::ColorEdit3("Specular", (float*)searchedEntity.GetSpeculatPtr(), 0);
                    ImGui::DragFloat("Shininess", searchedEntity.GetShininess(), 0.01f);*/
                    ImGui::ColorEdit3("Albedo", glm::value_ptr(tmpMaterial->albedo), 0);
                    ImGui::DragFloat("Metallic", &tmpMaterial->metallic, 0.001f, 0.0f, 1.0f);
                    ImGui::DragFloat("Roughness", &tmpMaterial->roughness, 0.001f, 0.0f, 1.0f);
                    ImGui::DragFloat("AO", &tmpMaterial->occlusion, 0.001f, 0.0f, 1.0f);

                    ImGui::Text("Texturing");
                    if(ImGui::Button("Set albedo texture"))
                    {
                        m_setAlbedo = true;
                    }
                    if(ImGui::Button("Set normal map"))
                    {
                        m_setNormalMap = true;
                    }
                }
                break;
            }
            case Entity::EntityType::LIGHT_DIR:
            {
                if(ImGui::CollapsingHeader("Material"))
                {
                    LightMaterial* tmpMaterial = searchedEntity.GetLightMaterialPtr();
                    ImGui::ColorEdit3("Color", glm::value_ptr(tmpMaterial->color), 0);
                    ImGui::DragFloat("Intensity", &searchedEntity.GetLight()->intensity, 0.01f, 0.0f, FLT_MAX);
                }
                break;
            }
            case Entity::EntityType::LIGHT_POINT:
            {
                if(ImGui::CollapsingHeader("Material"))
                {
                    LightMaterial* tmpMaterial = searchedEntity.GetLightMaterialPtr();
                    ImGui::ColorEdit3("Color", glm::value_ptr(tmpMaterial->color), 0);
                    ImGui::DragFloat("Range", &searchedEntity.GetLight()->range, 0.01f, 0.0f, FLT_MAX);
                    ImGui::DragFloat("Intensity", &searchedEntity.GetLight()->intensity, 0.01f, 0.0f, FLT_MAX);
                }
                break;
            }
        }
        if(ImGui::Button("Add component"))
        {
            m_addComponent = true;
        }
        for(GLuint i = 0; i < searchedEntity.GetComponents().size(); i++)
        {
            searchedEntity.GetComponents()[i].RenderInEditor();
        }
    }
    ImGui::End();
    ImGui::Begin("Game Explorer");
    ShowProjectStructure();
    ImGui::End();

    ShowStatusBar(statusMsg.c_str());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUIManager::FileBrowsing(int mode)
{
    if(mode == 0)
        result = NFD_OpenDialog("fbx,dae,gltf,glb,blend,3ds,ase,obj,ifc,xgl,zgl,ply,dxf,lwo,lws,lxo,stl,x,ac,ms3d,cob,scn,bhv,csm,xml,irrmesh", NULL, &outPath );
    else if(mode == 1)
        result = NFD_OpenDialog("jpg,jpeg,png,bmp", NULL, &outPath);
    else if(mode == 2)
        result = NFD_OpenDialog("lua", NULL, &outPath);
    else
        return false;
    if (result == NFD_OKAY )
    {
        return true;
    }
    else if ( result == NFD_CANCEL )
    {
        return false;
    }
    else
    {
        return false;
    }
    return true;
}

void GUIManager::ShowProjectStructure()
{
    boost::filesystem::path path = boost::filesystem::current_path();
    path += "/project/meshes";
    for (const auto & entry : boost::filesystem::directory_iterator(path))
    {
        if(ImGui::TreeNode(entry.path().leaf().c_str())){
            ImGui::TreePop();
        }
    }
}

void GUIManager::ImportMesh()
{
    const char* path = outPath;
    meshManager->ImportMesh(path);

    if(!projectManager->Refresh(meshManager))
        std::cout << "Failed to refresh the project directory. (meshes)" << std::endl;
}

void GUIManager::ImportScript()
{
    const char* path = outPath;
    LuaScript script(path, luaManager);
}

void GUIManager::AddEntity(Entity::EntityType m_type)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    switch(m_type)
    {
    case Entity::EntityType::MESH:
    {
        ImGui::Begin("Adding entity", &m_addEntity);
        if(meshManager->GetMeshes().size() == 0)
        {
            ImGui::Text("Meshes are not availed.");
        }
        else
        {
            std::vector<std::string> meshList;

            std::string tmpString;

            for(GLuint i = 0; i < meshManager->GetMeshes().size(); i++)
            {
                tmpString = "Mesh ";
                tmpString += std::to_string(i);
                meshList.push_back(tmpString);
            }

            static const char* item_current = meshList[0].c_str();
            static GLuint tmpSelect;
            if(ImGui::BeginCombo("Select the mesh", item_current))
            {
                for(GLuint i = 0; i < meshList.size(); i++)
                {
                    bool is_selected = (item_current == meshList[i].c_str());

                    if (ImGui::Selectable(meshList[i].c_str(), is_selected))
                    {
                        tmpSelect = i;
                        item_current = meshList[i].c_str();
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if(ImGui::Button("Add"))
            {
                entities->push_back(Entity(Entity::EntityType::MESH, meshManager->GetMeshes()[tmpSelect], nullptr, nullptr, entityManager, shader, depthShader, cameraManager, lightManager, game, shadowMap)); // Если вызвать push_back, то вызовется деструктор.
                AddIndexRecursive(entityManager->GetEntities()[ entities->size()-1] );
                selectableEntity = lastSelectableEntity = 0;
            }
        }
        ImGui::End();
        break;
    }
        case Entity::EntityType::LIGHT_DIR:
        {
            lightManager->GetLights().push_back(Light{glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(1.0f, 1.0f, 1.0f),
                                                      1.0f,
                                                      1.0f,
                                                });    //position
            Light* light_ptr = &lightManager->GetLights()[lightManager->GetLights().size()-1];
            entities->push_back(Entity(Entity::EntityType::LIGHT_DIR, nullptr, nullptr, light_ptr, entityManager, shader, depthShader, cameraManager, lightManager, game, shadowMap));
            size_t entSize = entities->size();
            entities->at(entSize-1).SetLightIndex(lightManager->GetLights().size()-1);
            AddIndexRecursive(entityManager->GetEntities()[ entities->size()-1] );
            m_addEntity = false;
            selectableEntity = localTmpIndex;
            break;
        }
        case Entity::EntityType::LIGHT_POINT:
        {
            lightManager->GetLights().push_back(Light{glm::vec3(0.0f, 0.0f, 0.0f),
                                                       glm::vec3(1.0f, 1.0f, 1.0f),
                                                       1.0f,
                                                       1.0f,
                                                 });    //position
            Light* light_ptr = &lightManager->GetLights()[lightManager->GetLights().size()-1];
            entities->push_back(Entity(Entity::EntityType::LIGHT_POINT, nullptr, nullptr, light_ptr, entityManager, shader, depthShader, cameraManager, lightManager, game, shadowMap));
            size_t entSize = entities->size();
            entities->at(entSize-1).SetLightIndex(lightManager->GetLights().size()-1);
            AddIndexRecursive(entityManager->GetEntities()[ entities->size()-1] );
            m_addEntity = false;
            selectableEntity = localTmpIndex;
            break;
        }
    }
}

void GUIManager::ShowStatusBar(const char* text)
{
    ImGui::Begin("Status bar", NULL, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoNav|ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::Text(text);
    ImGui::End();
}

void GUIManager::AddIndexRecursive(Entity& entity)
{
    localTmpIndex++;
    entity.SetIndex(localTmpIndex);

    for(GLuint j = 0; j < entity.GetChildEntities().size(); j++)
    {
        AddIndexRecursive(entity.GetChildEntities()[j]);
    }
}

void GUIManager::ImportImage(GLint type)
{
    if(FileBrowsing(IMAGE_FILES))
    {
        imageLoader->LoadImage(outPath, type);
        projectManager->Refresh(imageLoader);
    }
}

void GUIManager::SetNormalMap()
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin("Select the normal map", &m_setNormalMap);

    if(imageLoader->GetNormalMaps().size() <= 0)
        ImGui::Text("Normal maps are not availed.");
    else
    {
        std::vector<std::string> imageList;

        std::string tmpString;
        for(GLuint i = 0; i < imageLoader->GetNormalMaps().size(); i++)
        {
            tmpString = "Normal map ";
            tmpString += std::to_string(i);
            imageList.push_back(tmpString);
        }

        static const char* item_current = imageList[0].c_str();
        static GLuint tmpSelect;
        if(ImGui::BeginCombo("Select the normal map", item_current))
        {
            for(GLuint i = 0; i < imageList.size(); i++)
            {
                bool is_selected = (item_current == imageList[i].c_str());

                if (ImGui::Selectable(imageList[i].c_str(), is_selected))
                {
                    tmpSelect = i;
                    item_current = imageList[i].c_str();
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if(ImGui::Button("Add normal map"))
        {
            entityManager->SearchByIndex(selectableEntity).SetNormalMap(imageLoader->GetNormalMaps()[tmpSelect].texture);
        }
        if(ImGui::Button("Set null"))
        {
            entityManager->SearchByIndex(selectableEntity).SetNormalMap(0);
        }
    }
    ImGui::End();
}

void GUIManager::SetAlbedo()
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin("Select the texture", &m_setAlbedo);

    if(imageLoader->GetDiffuseTextures().size() == 0)
        ImGui::Text("Textures are not availed.");
    else
    {
        std::vector<std::string> imageList;

        std::string tmpString;
        for(GLuint i = 0; i < imageLoader->GetDiffuseTextures().size(); i++)
        {
            tmpString = "Image ";
            tmpString += std::to_string(i);
            imageList.push_back(tmpString);
        }

        static const char* item_current = imageList[0].c_str();
        static GLuint tmpSelect;
        if(ImGui::BeginCombo("Select the texture", item_current))
        {
            for(GLuint i = 0; i < imageList.size(); i++)
            {
                bool is_selected = (item_current == imageList[i].c_str());

                if (ImGui::Selectable(imageList[i].c_str(), is_selected))
                {
                    tmpSelect = i;
                    item_current = imageList[i].c_str();
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if(ImGui::Button("Add albedo texture"))
        {
            entityManager->SearchByIndex(selectableEntity).SetAlbedo(imageLoader->GetDiffuseTextures()[tmpSelect].texture);
        }
        if(ImGui::Button("Set null"))
        {
            entityManager->SearchByIndex(selectableEntity).SetAlbedo(0);
        }
    }
    ImGui::End();
}

void GUIManager::SetComponent()
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin("Select the script", &m_addComponent);

    if(luaManager->GetScripts().size() == 0)
        ImGui::Text("Scripts are not availed.");
    else
    {
        std::vector<std::string> scriptsList;

        std::string tmpString;
        for(GLuint i = 0; i < luaManager->GetScripts().size(); i++)
        {
            tmpString = "Script ";
            tmpString += std::to_string(i);
            scriptsList.push_back(tmpString);
        }

        static const char* item_current = scriptsList[0].c_str();
        static GLuint tmpSelect;
        if(ImGui::BeginCombo("Select the script", item_current))
        {
            for(GLuint i = 0; i < scriptsList.size(); i++)
            {
                bool is_selected = (item_current == scriptsList[i].c_str());

                if (ImGui::Selectable(scriptsList[i].c_str(), is_selected))
                {
                    tmpSelect = i;
                    item_current = scriptsList[i].c_str();
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if(ImGui::Button("Add component"))
        {
            entityManager->SearchByIndex(selectableEntity).GetComponents().push_back(Component(&luaManager->GetScripts()[tmpSelect], tmpString, game, luaManager));
        }
    }
    ImGui::End();
}

void GUIManager::ShowEntityStructure(Entity& entity)
{
    static ImGuiTreeNodeFlags node_flags;

    if(entity.GetChildEntities().size() == 0)
        node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    else
        node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    bool tree = ImGui::TreeNodeEx((void*)(intptr_t)entity.GetIndex(), node_flags, entity.GetName().c_str(), entity.GetIndex());

    if(ImGui::IsItemClicked())
    {
        lastSelectableEntity = selectableEntity;
        selectableEntity = entity.GetIndex();
        if(!m_first)
        {
            entityManager->SearchByIndex(lastSelectableEntity).UnSelect();
        }
        m_first = false;

        entity.Select();
        node_flags |= ImGuiTreeNodeFlags_Selected;
        isSearching = true;
    }
    if(entity.GetChildEntities().size() != 0)
    {
        if(tree)
        {
            for(GLuint j = 0; j < entity.GetChildEntities().size(); j++)
            {
                ShowEntityStructure(entity.GetChildEntities()[j]);
            }
            ImGui::TreePop();
        }
    }
}

void GUIManager::ShowGameSettings(){
    ImGui::Begin("Game Settings", &m_gameSettings);
    ImGui::Columns(2, NULL, true);
    ImGui::Text("Skybox settings");

    ImGui::NextColumn();
    ImGui::Text("123");
    ImGui::Text("456");
    ImGui::Columns(1);

    ImGui::End();
}

ImVec2 GUIManager::GetViewportSize()
{
    return viewportSize;
}

void GUIManager::About()
{
    ImGui::Begin("About", &m_about);
    ImGui::Text("balgorEngine by balgor36.\n\nSubscribe to my YouTube channel - balgor36.\nHelp in the development of the project - https://donatepay.ru/don/315094");
    ImGui::End();
}

