#pragma once
#include "npc.h"

struct Bandit : public NPC {
    Bandit(const std::string& name, int x, int y);
    Bandit(std::istream &is);
    
    bool accept(Visitor& visitor, std::shared_ptr<NPC> attacker) override;
    
    void print() override;
    void save(std::ostream &os) override;
    
    friend std::ostream &operator<<(std::ostream &os, Bandit &bandit);
};
