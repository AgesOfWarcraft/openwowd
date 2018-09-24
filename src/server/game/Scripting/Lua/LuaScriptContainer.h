#ifndef _LuaScriptMgr_h
#define _LuaScriptMgr_h

#include "Common.h"
#include <sol.hpp>

class Object;
class Creature;

typedef std::unordered_map<std::string, sol::function> ScriptNameMap;
typedef std::unordered_map<ObjectGuid, sol::table> ScriptEntityMap;

class TC_GAME_API LuaScriptContainer {
public:
    LuaScriptContainer();
    ~LuaScriptContainer();

    void RegisterTypes();
    void LoadScripts();

    void UnloadScriptsFor(Object*);

    void RegisterCreatureGossipScript(std::string, sol::function);
    void RegisterCreatureAIScript(std::string, sol::function);

    sol::function GetCreatureGossipScript(std::string);
    sol::table CreateOrGetCreatureGossipScriptObject(Creature*);

    sol::function GetCreatureAIScript(std::string);
    sol::table CreateOrGetCreatureAIScriptObject(Creature*);


private:
    sol::state lua;

    ScriptNameMap m_creatureGossipScriptNameMap;
    ScriptEntityMap m_creatureGossipScriptMap;

    ScriptNameMap m_creatureAiScriptNameMap;
    ScriptEntityMap m_creatureAiScriptMap;
};

void AddLua_WrapperScripts();

#endif //_LuaScriptMgr_h
