#include "../include/elf.h"

Elf::Elf(const std::string& name, int x, int y) : NPC(ElfType, name, x, y) {}

Elf::Elf(std::istream &is) : NPC(ElfType, is) {}

bool Elf::accept(Visitor& visitor, std::shared_ptr<NPC> attacker) {
    return visitor.visit(*this, attacker);
}

void Elf::print() {
    std::cout << *this;
}

void Elf::save(std::ostream &os) {
    os << ElfType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Elf &elf) {
    os << "Elf " << elf.name << " at (" << elf.x << ", " << elf.y << ")";
    return os;
}
