#include "LuaCreatureScript.h"
#include "LuaCreatureAI.h"
#include "ObjectMgr.h"

LuaCreatureScript::LuaCreatureScript() : CreatureScript("LuaCreature") {

}

bool LuaCreatureScript::OnGossipHello(Player* player, Creature* creature) {

    LuaScriptContainer* container = creature->GetMap()->GetLuaContainer();
    sol::table script = container->CreateOrGetCreatureGossipScriptObject(creature);
    if(!script.valid())
        return false;

    sol::protected_function func = script["OnGossipHello"];
    if(!func.valid())
        return false;

    auto result = func(script, player, creature);
    if(result.valid()) {
        bool ret = result;
        if(ret)
            return true;
    } else {
        sol::error error = result;
        std::string msg = error.what();
        TC_LOG_ERROR("lua", "An exception was thrown while executing a Lua script.");
        TC_LOG_ERROR("lua", "%s", msg.c_str());
    }

    return false;
}

CreatureAI* LuaCreatureScript::GetAI(Creature* creature) const {
    return new LuaCreatureAI(creature);
}