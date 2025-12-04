#pragma once
#include "npc.h"

struct Knight : public NPC {
    Knight(const std::string& name, int x, int y);
    Knight(std::istream &is);
    
    bool accept(Visitor& visitor, std::shared_ptr<NPC> attacker) override;
    
    void print() override;
    void save(std::ostream &os) override;
    
    friend std::ostream &operator<<(std::ostream &os, Knight &knight);
};
