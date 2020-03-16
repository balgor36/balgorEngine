#include "luaManager.h"

LuaManager::LuaManager(EntityManager* entityManager)
{
    m_entityManager = entityManager;
}

LuaManager::~LuaManager()
{

}

sol::state& LuaManager::GetLuaState()
{
    return luaState;
}

void LuaManager::Init()
{
    luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
    sol::usertype<glm::vec3> vec3_type = luaState.new_usertype<glm::vec3>("Vector3", sol::constructors<glm::vec3(), glm::vec3(float, float, float), glm::vec3(float)>());
    vec3_type["x"] = &glm::vec3::x;
    vec3_type["y"] = &glm::vec3::y;
    vec3_type["z"] = &glm::vec3::z;

    sol::usertype<Entity> entity_type = luaState.new_usertype<Entity>("Entity", sol::constructors<Entity()>());
    entity_type["GetPosition"] = &Entity::GetPosition;
    entity_type["SetPosition"] = &Entity::SetPosition;

    entity_type["GetRotation"] = &Entity::GetRotation;
    entity_type["SetRotation"] = &Entity::SetRotation;

    entity_type["GetScale"] = &Entity::GetScale;
    entity_type["SetScale"] = &Entity::SetScale;

    entity_type["IncreasePosition"] = &Entity::IncreasePosition;
    entity_type["IncreaseRotation"] = &Entity::IncreaseRotation;
    entity_type["IncreaseScale"] = &Entity::IncreaseScale;

    sol::usertype<EntityManager> entityManager_type = luaState.new_usertype<EntityManager>("EntityManager", sol::constructors<EntityManager(CameraManager*, LightManager*, Game*)>());
    entityManager_type["SearchByIndex"] = &EntityManager::SearchByIndex;

    sol::usertype<Input> input_type = luaState.new_usertype<Input>("Input", sol::constructors<Input()>());
    input_type["GetKeyDown"] = &Input::GetKeyDown;
    input_type["GetDeltaX"] = &Input::GetDeltaX;
    input_type["GetDeltaY"] = &Input::GetDeltaY;
    input_type["HideCursor"] = &Input::HideCursor;
    input_type["ShowCursor"] = &Input::ShowCursor;

    sol::usertype<Camera> camera_type = luaState.new_usertype<Camera>("Camera", sol::constructors<Camera(Window*, Input*, Time*, bool)>());
    camera_type["GetPosition"] = &Camera::GetPosition;
    camera_type["SetPosition"] = &Camera::SetPosition;
    camera_type["Front"] = &Camera::Front;
    camera_type["Back"] = &Camera::Back;
    camera_type["Right"] = &Camera::Right;
    camera_type["Left"] = &Camera::Left;
    camera_type["Yaw"] = &Camera::Yaw;
    camera_type["Pitch"] = &Camera::Pitch;

    sol::usertype<Time> time_type = luaState.new_usertype<Time>("Time", sol::constructors<Time()>());
    time_type["DeltaTime"] = &Time::GetDeltaTime;
}

void LuaManager::SetIndices()
{
    for(int i = lastSize; i < luaScripts.size(); i++)
    {
        luaScripts[i].SetScriptIndex(i);
    }
    lastSize = luaScripts.size();
}

std::vector<LuaScript>& LuaManager::GetScripts()
{
    return luaScripts;
}

LuaScript& LuaManager::GetScriptByFilename(std::string filename)
{
    for(int i = 0; i < luaScripts.size(); i++)
    {
        if(luaScripts[i].GetFilename() == filename)
            return luaScripts[i];
    }
}

LuaScript& LuaManager::GetScriptByIndex(int index)
{
    for(int i = 0; i < luaScripts.size(); i++)
    {
        if(luaScripts[i].GetScriptIndex() == index)
            return luaScripts[i];
    }
}
