#pragma once
#include "npc.h"      
#include <memory>     
#include <iostream>    
#include <fstream>    

class TextObserver : public IFightObserver {
public:
    TextObserver() = default;
    
    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override { 
        if (win) {
            std::cout << std::endl << "Murder --------" << std::endl;
            std::cout << "Attacker: ";
            attacker->print();
            std::cout << " Defender: ";
            defender->print();
        } else {
            std::cout << std::endl << "Mutual Destruction --------" << std::endl;
            std::cout << "Both died: ";
            attacker->print();
            std::cout << " and ";
            defender->print();
        }
    }
};

class FileObserver : public IFightObserver {
    std::ofstream log_file;
    
public:
    FileObserver(const std::string& filename) {
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Cannot open log file: " << filename << std::endl;
        }
    }

    ~FileObserver() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        if (log_file.is_open()) {
            if (win) {
                log_file << attacker->get_name() << " killed " 
                         << defender->get_name() << std::endl;
            } else {
                log_file << "Mutual destruction: " << attacker->get_name() 
                         << " and " << defender->get_name() << " both died" << std::endl;
            }
        }
    }
};
