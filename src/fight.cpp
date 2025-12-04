#include "../include/npc.h"
#include "../include/battle_visitor.h"
#include <memory>
#include <set>
#include <vector>

set_t fight(const set_t &array, size_t distance) {
    set_t dead_list;
    BattleVisitor visitor;
    
    std::vector<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> battles;
    
    for (const auto &attacker : array) {
        for (const auto &defender : array) {
            if (attacker == defender) continue;
            
            if (attacker->is_close(defender, distance)) {
                bool already_exists = false;
                for (const auto& [n1, n2] : battles) {
                    if ((n1 == attacker && n2 == defender) || 
                        (n1 == defender && n2 == attacker)) {
                        already_exists = true;
                        break;
                    }
                }
                
                if (!already_exists) {
                    battles.push_back({attacker, defender});
                }
            }
        }
    }
    
    for (const auto &[attacker, defender] : battles) {
        if (dead_list.count(attacker) || dead_list.count(defender)) {
            continue;
        }
        
        bool attacker_won = defender->accept(visitor, attacker);
        
        if (attacker->get_type() == defender->get_type()) {
            dead_list.insert(attacker);
            dead_list.insert(defender);
            
            attacker->fight_notify(defender, false);
        }
        else if (attacker_won) {
            dead_list.insert(defender);
            attacker->fight_notify(defender, true);
        }
        else {
            dead_list.insert(attacker);
            defender->fight_notify(attacker, true);
        }
    }
    
    return dead_list;
}
