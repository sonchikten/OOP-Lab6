#pragma once
#include "npc.h"

struct Elf : public NPC {
    Elf(const std::string& name, int x, int y); 
    Elf(std::istream &is);
    
    bool accept(Visitor& visitor, std::shared_ptr<NPC> attacker) override; //возвращает true если атакующий победил, false, если эльф(защитник) победил, visitor по сути - арбитор, который смотрит и решает
    
    void print() override;
    void save(std::ostream &os) override;
    
    friend std::ostream &operator<<(std::ostream &os, Elf &elf);
};
