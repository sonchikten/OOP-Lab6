#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <set>
#include <vector>

struct NPC;
struct Elf;
struct Knight;
struct Bandit;
struct Visitor;
using set_t = std::set<std::shared_ptr<NPC>>; //множество умных указателей на NPC (хранит уникальные элементы)

enum NpcType {
    Unknown = 0,
    ElfType = 1,   
    KnightType = 2,  
    BanditType = 3 
};

struct IFightObserver {
    virtual void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) = 0; //win - true - атаккующий победил
    virtual ~IFightObserver() = default;
};

struct Visitor {
    virtual bool visit(Elf& elf, std::shared_ptr<NPC> attacker) = 0; //true если аттакующий победил
    virtual bool visit(Knight& knight, std::shared_ptr<NPC> attacker) = 0;
    virtual bool visit(Bandit& bandit, std::shared_ptr<NPC> attacker) = 0;
    virtual ~Visitor() = default;
};

struct NPC : public std::enable_shared_from_this<NPC> { //позволяет создавать shared_ptr на самого себя - для fight_notify()
    NpcType type; 
    std::string name;
    int x{0};         
    int y{0};       
    std::vector<std::shared_ptr<IFightObserver>> observers;
    
    NPC(NpcType t, const std::string& n, int _x, int _y);
    NPC(NpcType t, std::istream &is);
    
    virtual ~NPC() = default;
    
    void subscribe(std::shared_ptr<IFightObserver> observer); //добавляем нового наблюдателя
    void fight_notify(const std::shared_ptr<NPC> defender, bool win);
    
    virtual bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;
    
    virtual bool is_elf() const;
    virtual bool is_knight() const;
    virtual bool is_bandit() const;
    
    virtual bool accept(Visitor& visitor, std::shared_ptr<NPC> attacker) = 0;
    
    virtual void print() = 0;
    virtual void save(std::ostream &os);

    int get_x() const { return x; }
    int get_y() const { return y; }

    const std::string& get_name() const { return name; }
    NpcType get_type() const { return type; }
    
    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};
