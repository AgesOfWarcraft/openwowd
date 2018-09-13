#ifndef _LuaCreatureScript_h
#define _LuaCreatureScript_h

#include "LuaScriptContainer.h"
#include "ScriptMgr.h"
#include "Creature.h"

class LuaCreatureScript : public CreatureScript {
public:
    LuaCreatureScript();

    // Called when a player opens a gossip dialog with the creature.
    bool OnGossipHello(Player* /*player*/, Creature* /*creature*/);

    // Called when a CreatureAI object is needed for the creature.
    CreatureAI* GetAI(Creature* /*creature*/) const;

private:
};


#endif //_LuaCreatureScript_h
