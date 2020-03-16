#ifndef COMPONENT_H
#define COMPONENT_H

#include "luaScript.h"
#include "game.h"

class LuaManager;
class LuaScript;
class Game;

class Component
{
public:
    Component();
    Component(LuaScript* luaScript, std::string name, Game* game, LuaManager* luaManager);
    Component(std::string scriptFilename, std::string name, Game* game, LuaManager* luaManager);
    Component(int scriptIndex, std::string name, Game* game, LuaManager* luaManager);
    ~Component();

    LuaScript* GetLuaScript();

    bool Run();
    void Stop();
    void RenderInEditor();
    void SetIndex(int* index);

    std::string GetName();
private:
    LuaScript* script;
    Game* game;
    LuaManager* luaManager;

    sol::environment environment;

    bool isRan = false;
    bool enable = true;

    bool LoadVariables();

    std::string name;

    std::vector<std::pair<std::string, std::string>> stringVariables;
    std::vector<std::pair<std::string, double>> numVariables;
    std::vector<std::pair<std::string, bool>> boolVariables;
};

#endif
