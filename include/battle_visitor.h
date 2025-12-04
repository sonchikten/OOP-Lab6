#pragma once
#include "npc.h"

class BattleVisitor : public Visitor {
public:
    bool visit(Elf& defender, std::shared_ptr<NPC> attacker) override;
    bool visit(Knight& defender, std::shared_ptr<NPC> attacker) override;
    bool visit(Bandit& defender, std::shared_ptr<NPC> attacker) override;
};
