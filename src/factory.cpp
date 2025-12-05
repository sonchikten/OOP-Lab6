#include "../include/factory.h"
#include "../include/elf.h"
#include "../include/knight.h"
#include "../include/bandit.h"
#include "../include/observer.h"
#include <fstream>
#include <memory>
#include <iostream> 

std::shared_ptr<NPC> factory(std::istream &is) {
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type) {
        switch (type) {
        case ElfType:
            result = std::make_shared<Elf>(is); //выделяет память, вызывает конструктор и создает shared_ptr, который управляет этим объектом
            break;
        case KnightType:
            result = std::make_shared<Knight>(is);
            break;
        case BanditType:
            result = std::make_shared<Bandit>(is);
            break;
        default:
            std::cerr << "unexpected NPC type:" << type << std::endl;
        }
    } else {
        std::cerr << "error reading NPC type" << std::endl;
    }
    return result;
}

std::shared_ptr<NPC> factory(NpcType type, const std::string& name, int x, int y) {
    std::shared_ptr<NPC> result;
    switch (type) {
    case ElfType:
        result = std::make_shared<Elf>(name, x, y);
        break;
    case KnightType:
        result = std::make_shared<Knight>(name, x, y);
        break;
    case BanditType:
        result = std::make_shared<Bandit>(name, x, y);
        break;
    default:
        std::cerr << "unknown NPC type: " << type << std::endl;
    }
    return result;
}

void save(const set_t &array, const std::string &filename) {
    std::ofstream fs(filename);
    if (!fs.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }
    
    fs << array.size() << std::endl; 
    for (auto &n : array) {
        n->save(fs); 
    }
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename) {
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open()) { //если поток в хорошем состоянии и файл открыт
        int count;
        is >> count;  
        for (int i = 0; i < count; i++) {
            auto npc = factory(is);
            if (npc) {
                result.insert(npc);
            }
        }
        is.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array) {
    for (auto &n : array) {
        n->print();
        os << std::endl;
    }
    return os;
}
