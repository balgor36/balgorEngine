#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "luaManager.h"

class LuaManager;

class LuaScript
{
public:
    LuaScript();
    LuaScript(std::string filename, LuaManager* luaManager);
    ~LuaScript();

    sol::global_table GetGlobalTable(sol::environment& env);
    bool Run(sol::environment& env);
    void Stop();
    void Update();
    void AddToManager();
    void SetIndex(int* index);

    std::string GetFilename();
    void SetFilename(std::string newFilename);

    int GetScriptIndex();
    void SetScriptIndex(int index);
private:
    LuaManager* luaManager;
    sol::protected_function update;

    std::string filename;
    int m_index;
    static int index;
};

#endif
