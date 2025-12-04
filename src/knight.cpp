#include "../include/knight.h"

Knight::Knight(const std::string& name, int x, int y) : NPC(KnightType, name, x, y) {}

Knight::Knight(std::istream &is) : NPC(KnightType, is) {}

bool Knight::accept(Visitor& visitor, std::shared_ptr<NPC> attacker) {
    return visitor.visit(*this, attacker);
}

void Knight::print() {
    std::cout << *this;
}

void Knight::save(std::ostream &os) {
    os << KnightType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Knight &knight) {
    os << "Knight " << knight.name << " at (" << knight.x << ", " << knight.y << ")";
    return os;
}
