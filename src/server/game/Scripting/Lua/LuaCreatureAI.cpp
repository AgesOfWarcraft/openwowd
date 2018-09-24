#include "LuaCreatureAI.h"
#include "Creature.h"
#include "Map.h"
#include "ObjectMgr.h"

LuaCreatureAI::LuaCreatureAI(Creature *creature) : CreatureAI(creature), me(creature) {
    script = creature->GetMap()->GetLuaContainer()->CreateOrGetCreatureAIScriptObject(creature);
}

LuaCreatureAI::~LuaCreatureAI() {

}

bool LuaCreatureAI::CanAIAttack(const Unit *target) const {
    return true;
}

void LuaCreatureAI::EnterCombat(Unit *) {

}

void LuaCreatureAI::ExecuteEvent(uint32) {

}

void LuaCreatureAI::JustDied(Unit *) {

}

void LuaCreatureAI::JustReachedHome() {

}

void LuaCreatureAI::JustSummoned(Creature *summon) {

}

void LuaCreatureAI::ScheduleTasks() {

}

void LuaCreatureAI::SummonedCreatureDespawn(Creature *summon) {
    if(script.valid()) {
        sol::protected_function func = script["SummonedCreatureDespawn"];
        if(func.valid()) {
            auto result = func(script, summon);
            if (!result.valid()) {
                sol::error error = result;
                std::string msg = error.what();
                TC_LOG_ERROR("lua", "An exception was thrown while executing a lua script.");
                TC_LOG_ERROR("lua", "%s", msg.c_str());
            }
        }
    }

    CreatureAI::SummonedCreatureDespawn(summon);
}

void LuaCreatureAI::UpdateAI(uint32 diff) {
    if(script.valid()) {
        sol::protected_function func = script["UpdateAI"];
        if(func.valid()) {
            auto result = func(script, diff);
            if (!result.valid()) {
                sol::error error = result;
                std::string msg = error.what();
                TC_LOG_ERROR("lua", "An exception was thrown while executing a lua script.");
                TC_LOG_ERROR("lua", "%s", msg.c_str());
            }
        }
    }

    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    DoMeleeAttackIfReady();
}

void LuaCreatureAI::Reset() {
    if(script.valid()) {
        sol::protected_function func = script["Reset"];
        if(func.valid()) {
            auto result = func(script);
            if (!result.valid()) {
                sol::error error = result;
                std::string msg = error.what();
                TC_LOG_ERROR("lua", "An exception was thrown while executing a lua script.");
                TC_LOG_ERROR("lua", "%s", msg.c_str());
            }
        }
    }

    CreatureAI::Reset();
}