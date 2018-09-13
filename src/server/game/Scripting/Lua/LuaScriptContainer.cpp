#include "LuaScriptContainer.h"
#include "ScriptMgr.h"
#include "LuaCreatureScript.h"
#include "boost/filesystem.hpp"
#include "World.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Creature.h"
#include "ThreatManager.h"
#include <lua.h>

LuaScriptContainer::LuaScriptContainer() {
    RegisterTypes();
    LoadScripts();
}

LuaScriptContainer::~LuaScriptContainer() {

}

void LuaScriptContainer::RegisterTypes() {
    lua.new_usertype<LuaScriptContainer>("LuaScriptContainer",
        "RegisterCreatureGossipScript", &LuaScriptContainer::RegisterCreatureGossipScript,
        "RegisterCreatureAIScript", &LuaScriptContainer::RegisterCreatureAIScript
    );

    lua.new_usertype<ThreatManager>("ThreatManager",
            "AddThreat", &ThreatManager::doAddThreat,
            "GetThreat", sol::resolve<float(Unit*)>(&ThreatManager::getThreat),
            "GetThreatListSize", &ThreatManager::getThreatListSize
    );

    lua.new_usertype<Object>("Object",
        "GetGUID", &Object::GetGUID,
        "GetEntry", &Object::GetEntry,
        "GetUInt32Value", &Object::GetUInt32Value,
        "GetFloatValue", &Object::GetFloatValue
    );

    lua.new_usertype<WorldObject>("WorldObject",
        "FindNearestCreature", sol::overload(
                sol::resolve<Creature*(uint32, float, bool) const>(&WorldObject::FindNearestCreature),
                sol::resolve<Creature*(uint32, float) const>(&WorldObject::FindNearestCreature)
            )
    );

    lua.new_usertype<Unit>("Unit",
        "GetThreatManager", &Unit::getThreatManager,
        "IsInCombat", &Unit::IsInCombat,
        "IsAlive", &Unit::IsAlive,
        "IsDead", &Unit::isDead,
        "GetVictim", &Unit::GetVictim,
        "GetMaxHealth", &Unit::GetMaxHealth,
        "GetHealth", &Unit::GetHealth,
        "GetHealthPct", &Unit::GetHealthPct,
        "SetHealth", &Unit::SetHealth,
        "SetFullHealth", &Unit::SetFullHealth,
        "SetMaxHealth", &Unit::SetMaxHealth,
        "CombatStart", sol::overload(
                sol::resolve<void(Unit*)>(&Unit::CombatStart)
        ),
        "MonsterSay", sol::overload(
                sol::resolve<void(std::string const&)>(&Unit::Say),
                sol::resolve<void(std::string const&, Language)>(&Unit::Say),
                sol::resolve<void(std::string const&, Language, WorldObject const*)>(&Unit::Say),
                sol::resolve<void(uint32, WorldObject const*)>(&Unit::Say),
                sol::resolve<void(uint32)>(&Unit::Say)
        ),

        sol::base_classes, sol::bases<WorldObject, Object>()
    );

    lua.new_usertype<Creature>("Creature",
            sol::base_classes, sol::bases<Unit, WorldObject, Object>()
    );
}

void LuaScriptContainer::LoadScripts() {
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string);

    std::string package_path = sWorld->GetLuaScriptsPath() + "/lib/?.lua";
    lua["package"]["path"] = package_path;

    lua["scripts"] = this;

    boost::filesystem::path p(sWorld->GetLuaScriptsPath() + "/src");

    boost::filesystem::recursive_directory_iterator end_itr;

    for(boost::filesystem::recursive_directory_iterator itr(p); itr != end_itr; ++itr) {
        const boost::filesystem::path file = *itr;
        if(boost::filesystem::is_regular_file(file))
            lua.safe_script_file(file.string());
    }
}

void LuaScriptContainer::RegisterCreatureGossipScript(std::string name, sol::function function) {
    TC_LOG_ERROR("scripts.lua", "Registered gossip script %s", name.c_str());
    m_creatureGossipScriptNameMap.insert(std::make_pair(name, function));
}

void LuaScriptContainer::RegisterCreatureAIScript(std::string name, sol::function function) {
    TC_LOG_ERROR("scripts.lua", "Registered creature script %s", name.c_str());
    m_creatureAiScriptNameMap.insert(std::make_pair(name, function));
}

sol::table LuaScriptContainer::CreateOrGetCreatureGossipScriptObject(Creature* creature) {
    std::string scriptName = creature->GetCreatureTemplate()->ScriptData;
    sol::function func = GetCreatureGossipScript(scriptName);

    if(!func.valid())
        return sol::table();

    auto search = m_creatureGossipScriptMap.find(creature->GetGUID());
    if(search == m_creatureGossipScriptMap.end()) {
        sol::table object = func();
        m_creatureGossipScriptMap.insert(std::make_pair(creature->GetGUID(), object));
        return object;
    } else {
        return search->second;
    }
}

sol::table LuaScriptContainer::CreateOrGetCreatureAIScriptObject(Creature* creature) {
    std::string scriptName = creature->GetCreatureTemplate()->ScriptData;
    sol::function func = GetCreatureAIScript(scriptName);

    if(!func.valid())
        return sol::table();

    auto search = m_creatureAiScriptMap.find(creature->GetGUID());
    if(search == m_creatureAiScriptMap.end()) {
        sol::table object = func(creature);
        m_creatureAiScriptMap.insert(std::make_pair(creature->GetGUID(), object));
        return object;
    } else {
        return search->second;
    }
}

void LuaScriptContainer::UnloadScriptsFor(Object* object) {
    if(Creature* creature = object->ToCreature()) {
        m_creatureGossipScriptMap.erase(object->GetGUID());
        m_creatureAiScriptMap.erase(object->GetGUID());
    }
}

sol::function LuaScriptContainer::GetCreatureGossipScript(std::string name) {
    return m_creatureGossipScriptNameMap[name];
}

sol::function LuaScriptContainer::GetCreatureAIScript(std::string name) {
    return m_creatureAiScriptNameMap[name];
}

void AddLua_WrapperScripts() {
    new LuaCreatureScript;
}
