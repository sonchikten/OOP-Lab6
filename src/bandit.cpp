#include "../include/bandit.h"

Bandit::Bandit(const std::string& name, int x, int y) : NPC(BanditType, name, x, y) {}

Bandit::Bandit(std::istream &is) : NPC(BanditType, is) {}

bool Bandit::accept(Visitor& visitor, std::shared_ptr<NPC> attacker) {
    return visitor.visit(*this, attacker);
}

void Bandit::print() {
    std::cout << *this;
}

void Bandit::save(std::ostream &os) {
    os << BanditType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Bandit &bandit) {
    os << "Bandit " << bandit.name << " at (" << bandit.x << ", " << bandit.y << ")";
    return os;
}
