#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <sol/sol.hpp>

#include "luaScript.h"
#include "time_bg.h"
#include "camera.h"
#include "entityManager.h"
#include "window.h"

class LuaScript;
class EntityManager;

class LuaManager
{
public:
    LuaManager(EntityManager* entityManager);
    ~LuaManager();

    void Init();
    void UpdateFunc();
    void SetIndices();
    sol::state& GetLuaState();

    std::vector<LuaScript>& GetScripts();
    LuaScript& GetScriptByFilename(std::string filename);
    LuaScript& GetScriptByIndex(int index);
private:
    sol::state luaState;

    EntityManager* m_entityManager;

    int lastSize = 0;

    std::vector<LuaScript> luaScripts;
};

#endif
