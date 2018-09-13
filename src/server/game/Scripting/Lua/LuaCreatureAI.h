#ifndef _Lua_CreatureAI_h
#define _Lua_CreatureAI_h

#include "CreatureAI.h"
#include "LuaScriptContainer.h"

class LuaCreatureAI : public CreatureAI {
public:
    LuaCreatureAI(Creature* creature);
    ~LuaCreatureAI();


    void JustSummoned(Creature* summon) override;
    void SummonedCreatureDespawn(Creature* summon) override;

    virtual void UpdateAI(uint32 diff) override;

    virtual void ExecuteEvent(uint32);

    virtual void ScheduleTasks();

    void Reset() override;
    void EnterCombat(Unit*) override;
    void JustDied(Unit*) override;
    void JustReachedHome() override;

    bool CanAIAttack(Unit const* target) const override;

private:
    Creature* me;
    sol::table script;
};


#endif //_Lua_CreatureAI_h
