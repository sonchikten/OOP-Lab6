#include "battle_visitor.h"

bool BattleVisitor::visit(Elf& defender, std::shared_ptr<NPC> attacker) { //true если атакующий нападает на эльфа
    if (attacker->is_knight()) { // эльф убивает рыцаря → атакующий рыцарь проигрывает
        return false;
    } else if (attacker->is_bandit()) { // разбойник убивает эльфа → атакующий разбойник побеждает
        return true;
    } else {
        return false; //оба погибают
    }
}

bool BattleVisitor::visit(Knight& defender, std::shared_ptr<NPC> attacker) {
    if (attacker->is_elf()) {
        return true;
    } else if (attacker->is_bandit()) {
        return false;
    } else {
        return false;
    }
}

bool BattleVisitor::visit(Bandit& defender, std::shared_ptr<NPC> attacker) {
    if (attacker->is_knight()) {
        return true;
    } else if (attacker->is_elf()) {
        return false;
    } else {
        return false;
    }
}
