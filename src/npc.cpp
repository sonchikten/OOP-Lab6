#include "npc.h"

NPC::NPC(NpcType t, const std::string& n, int _x, int _y) : type(t), name(n), x(_x), y(_y) {
    if (x <= 0 || x > 500) x = 250;
    if (y <= 0 || y > 500) y = 250;
}

NPC::NPC(NpcType t, std::istream &is) : type(t) {
    is >> name;
    is >> x;  
    is >> y;   

    if (x <= 0 || x > 500) x = 250;
    if (y <= 0 || y > 500) y = 250;
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer) {
   observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win) {
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win); //shared_from_this() создает shared_ptr на текущий объект this
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    int dx = x - other->x;
    int dy = y - other->y;
    return (dx*dx + dy*dy) <= static_cast<int>(distance*distance);
}

bool NPC::is_elf() const { 
    return type == ElfType;
}

bool NPC::is_knight() const { 
    return type == KnightType;
}

bool NPC::is_bandit() const { 
    return type == BanditType;
}

void NPC::save(std::ostream &os) {
    os << name << std::endl; 
    os << x << std::endl;   
    os << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc) {
    switch (npc.type) {
        case ElfType: os << "Elf "; break;
        case KnightType: os << "Knight "; break;
        case BanditType: os << "Bandit "; break;
        default: os << "Unknown ";
    }
    os << npc.name << " at (" << npc.x << ", " << npc.y << ")";
    return os;
}
