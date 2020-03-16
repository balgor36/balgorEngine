#include "luaScript.h"

LuaScript::LuaScript()
{
}

LuaScript::LuaScript(std::string filename, LuaManager* luaManager)
{
    m_index = 0;
    this->filename = filename;
    this->luaManager = luaManager;
    AddToManager();
}

LuaScript::~LuaScript()
{
}

void LuaScript::AddToManager()
{
    luaManager->GetScripts().push_back(*this); // ошибка
    luaManager->SetIndices();
}

void LuaScript::Update()
{
    if(update.valid()){
        update();
    }
}

void LuaScript::SetIndex(int* index)
{
    //luaManager->GetLuaState()["m_index"] = *index;
}

int LuaScript::GetScriptIndex()
{
    return m_index;
}

void LuaScript::SetScriptIndex(int index)
{
    m_index = index;
}

sol::global_table LuaScript::GetGlobalTable(sol::environment& env){
    sol::state& luaState = luaManager->GetLuaState();
    //sol::load_result lr = luaState.load_file(filename);
    sol::load_result script = luaState.load_file(filename);
    //luaState.script_file(filename);

    if(!script.valid()){
        sol::error err = script;
        std::cerr << err.what() << std::endl;
    }

    sol::protected_function_result script_result = script(env);
    if(!script_result.valid()){
        sol::error err = script_result;
        std::cerr << err.what() << std::endl;
    }
    return luaState.globals();
}

bool LuaScript::Run(sol::environment& env)
{
    sol::state& luaState = luaManager->GetLuaState();
    //sol::load_result lr = luaState.load_file(filename);
    sol::load_result script = luaState.load_file(filename);
    //luaState.script_file(filename);

    if(!script.valid()){
        sol::error err = script;
        std::cerr << err.what() << std::endl;
        return false;
    }

    sol::protected_function_result script_result = script(env);
    if(!script_result.valid()){
        sol::error err = script_result;
        std::cerr << err.what() << std::endl;
        return false;
    }

    update = luaState["Update"];
    return true;
}

std::string LuaScript::GetFilename()
{
    return filename;
}

void LuaScript::SetFilename(std::string newFilename)
{
    filename = newFilename;
}

void LuaScript::Stop()
{
}
