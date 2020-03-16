#include "component.h"

Component::Component()
{
    this->environment = sol::environment(luaManager->GetLuaState(), sol::create, luaManager->GetLuaState().globals());
}

Component::Component(LuaScript* luaScript, std::string name, Game* game, LuaManager* luaManager)
{
    script = luaScript;
    this->name = name;
    this->game = game;
    this->luaManager = luaManager;
    this->environment = sol::environment(luaManager->GetLuaState(), sol::create, luaManager->GetLuaState().globals());
}

Component::Component(std::string scriptFilename, std::string name, Game* game, LuaManager* luaManager)
{
    this->script = &luaManager->GetScriptByFilename(scriptFilename);
    this->name = name;
    this->game = game;
    this->luaManager = luaManager;
    this->environment = sol::environment(luaManager->GetLuaState(), sol::create, luaManager->GetLuaState().globals());
}

Component::Component(int scriptIndex, std::string name, Game* game, LuaManager* luaManager)
{
    this->script = &luaManager->GetScriptByIndex(scriptIndex);
    this->name = name;
    this->game = game;
    this->luaManager = luaManager;
    this->environment = sol::environment(luaManager->GetLuaState(), sol::create, luaManager->GetLuaState().globals());
}

Component::~Component()
{

}

LuaScript* Component::GetLuaScript()
{
    return script;
}

void Component::RenderInEditor()
{
    if(ImGui::CollapsingHeader("Script")){
        static bool a = true;
        if(a){
            LoadVariables();
            a = false;
        }
        ImGui::Checkbox("Enable", &enable);
        ImGui::Text(name.c_str());
        for(int i = 0; i < numVariables.size(); i++){
            ImGui::InputDouble(numVariables[i].first.c_str(), &numVariables[i].second);
        }
        /*for(int i = 0; i < stringVariables.size(); i++){
            ImGui::InputText(numVariables[i].first.c_str(), &numVariables[i].second);
        }*/
        for(int i = 0; i < boolVariables.size(); i++){
            ImGui::Checkbox(boolVariables[i].first.c_str(), &boolVariables[i].second);
        }
    }
}

void Component::SetIndex(int* index)
{
    script->SetIndex(index);
}

bool Component::LoadVariables(){
    numVariables.clear();
    boolVariables.clear();
    stringVariables.clear();
    sol::global_table gt = script->GetGlobalTable(environment);

    for (const auto& key_value_pair : gt ) {
        sol::object key = key_value_pair.first;
        sol::object value = key_value_pair.second;

        if(value.get_type() == sol::type::number){
            std::cout << "number ";
            std::cout << key.as<std::string>() << " " << value.as<std::string>() << std::endl;
            numVariables.push_back(make_pair(key.as<std::string>(), value.as<double>()));
        }
        if(value.get_type() == sol::type::string){
            std::cout << "string ";
            std::cout << key.as<std::string>() << " " << value.as<std::string>() << std::endl;
            if(key.as<std::string>() != "_VERSION"){
                stringVariables.push_back(make_pair(key.as<std::string>(), value.as<std::string>()));
            }
        }
        if(value.get_type() == sol::type::boolean){
            std::cout << "boolean ";
            std::cout << key.as<std::string>() << " " << value.as<bool>() << std::endl;
            numVariables.push_back(make_pair(key.as<std::string>(), value.as<bool>()));
        }
        /*if(value.get_type() == sol::type::nil){
            //std::cout << "nil";
        }
        if(value.get_type() == sol::type::none){
            //std::cout << "none";
        }
        if(value.get_type() == sol::type::poly){
            //std::cout << "poly";
        }
        if(value.get_type() == sol::type::table){
            //std::cout << "table";
        }
        if(value.get_type() == sol::type::lua_nil){
            //std::cout << "lua_nil";
        }
        if(value.get_type() == sol::type::thread){
            //std::cout << "thread";
        }
        if(value.get_type() == sol::type::function){
            //std::cout << "function";
        }
        if(value.get_type() == sol::type::userdata){
            //std::cout << "userdata";
        }
        if(value.get_type() == sol::type::lightuserdata) {
            //std::cout << "lightuserdata";
        }*/
    }
}

bool Component::Run()
{
    //std::cout << "Name: " << name << std::endl;
    if(game->isPlay())
    {
        if(enable)
        {
            if(!isRan)
            {
                for(int i = 0; i < numVariables.size(); i++){
                    environment[numVariables[i].first] = numVariables[i].second;
                }
                environment["deltaX"] = 500;
                if(!script->Run(environment)){
                    game->Stop();
                    return false;
                }
                isRan = true;
            }
            script->Update();
        }
    }
    return true;
}

void Component::Stop()
{
    script->Stop();
    isRan = false;
}

std::string Component::GetName()
{
    return name;
}
