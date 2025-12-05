#include "../include/npc.h"
#include "../include/factory.h"
#include "../include/observer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

set_t fight(const set_t& array, size_t distance);  

bool is_name_unique(const set_t& array, const std::string& name) {
    for (const auto& npc : array) {
        if (npc->get_name() == name) {
            return false;
        }
    }
    return true;
}

set_t load_with_validation(const std::string& filename, std::shared_ptr<FileObserver> file_observer) {
    set_t result;
    std::ifstream is(filename); //открываем поток для чтения файла
    if (is.good() && is.is_open()) {
        int count; 
        is >> count; //количество npc
        int loaded = 0;
        int skipped = 0;
        
        for (int i = 0; i < count; i++) {
            auto npc = factory(is);

            if (!npc) {
                skipped++;      
                continue;
            }

            if (npc->get_x() <= 0 || npc->get_x() > 500 || 
                npc->get_y() <= 0 || npc->get_y() > 500) {
                std::cout << "Skipped " << npc->get_name() << ": coordinates (" << npc->get_x() << "," << npc->get_y() << ") are out of range (0,500]\n";
                skipped++;
                continue;
            }

            if (!is_name_unique(result, npc->get_name())) {
                std::cout << "Skipped NPC with non-unique name: " << npc->get_name() << std::endl;
                skipped++;
                continue;
            }
                
            npc->subscribe(std::make_shared<TextObserver>());
            npc->subscribe(file_observer);
            result.insert(npc);
            loaded++;
        }

        is.close();
        
        if (skipped > 0) {
            std::cout << "Loaded: " << loaded << ", skipped: " << skipped << std::endl;
        }
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    return result;
}

int main() {
    std::srand(std::time(nullptr)); //чтобы не выдавалось одно и то же число
    set_t array;

    auto file_observer = std::make_shared<FileObserver>("log.txt");
    
    std::cout << "=== Balagur Fate 3 Dungeon Editor ===\n";
    
    int choice;
    do {
        std::cout << "\nMENU:\n";
        std::cout << "1. Add NPC\n";
        std::cout << "2. Save to file\n";
        std::cout << "3. Load from file\n";
        std::cout << "4. Show all NPCs\n";
        std::cout << "5. Start battle\n";
        std::cout << "6. Generate test NPCs\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::string name;
                int type, x, y;
                
                std::cout << "\n--- Add new NPC ---\n";
                std::cout << "Enter unique NPC name: ";
                std::cin >> name;

                if (!is_name_unique(array, name)) {
                    std::cout << "Error: NPC with name " << name << " already exists!\n";
                    break;
                }
                            
                std::cout << "NPC type:\n";
                std::cout << "  1 - Elf (defeats Knight)\n";
                std::cout << "  2 - Knight (defeats Bandit)\n";
                std::cout << "  3 - Bandit (defeats Elf)\n";
                std::cout << "Your choice (1-3): ";
                std::cin >> type;
                
                if (type < 1 || type > 3) {
                    std::cout << "Error: type must be between 1 and 3!\n";
                    break;
                }
                
                std::cout << "Coordinate X (0 < X <= 500): ";
                std::cin >> x;
                std::cout << "Coordinate Y (0 < Y <= 500): ";
                std::cin >> y;

                if (x <= 0 || x > 500 || y <= 0 || y > 500) {
                    std::cout << "Error: coordinates must be in range (0, 500]!\n";
                    break;
                }

                auto npc = factory(static_cast<NpcType>(type), name, x, y);
                if (npc) {
                    npc->subscribe(std::make_shared<TextObserver>());
                    npc->subscribe(file_observer);
                    array.insert(npc);
                    std::cout << "NPC " << name << " successfully added!\n";
                }
                break;

            } case 2: {
                if (array.empty()) {
                    std::cout << "No NPCs to save!\n";
                    break;
                }
                
                std::string filename;
                std::cout << "Enter filename to save: ";
                std::cin >> filename;
                
                save(array, filename);
                std::cout << "Saved " << array.size() << " NPCs to file " << filename << "\n";
                break;

            } case 3: {
                std::string filename;
                std::cout << "Enter filename to load: ";
                std::cin >> filename;
                
                std::ifstream test_file(filename);
                if (!test_file.is_open()) {
                    std::cout << "Error: file " << filename << " not found!\n";
                    break;
                }
                test_file.close();
                
                auto loaded_set = load_with_validation(filename, file_observer);
                
                set_t final_set = array; 
                
                int conflicts = 0;
                for (const auto& new_npc : loaded_set) {
                    if (!is_name_unique(final_set, new_npc->get_name())) {
                        std::cout << "Name conflict: " << new_npc->get_name() << " already exists. NPC skipped.\n";
                        conflicts++;
                    } else {
                        final_set.insert(new_npc);
                    }
                }
                
                array = final_set;
                
                std::cout << "Loaded " << (loaded_set.size() - conflicts) << " NPCs from file " << filename << "\n";
                if (conflicts > 0) {
                    std::cout << "Skipped due to name conflicts: " << conflicts << std::endl;
                }
                break;

            } case 4: {
                std::cout << "\n=== List of all NPCs (" << array.size() << " total) ===\n";
                if (array.empty()) {
                    std::cout << "No NPCs\n";
                } else {
                    std::cout << array;
                }
                break;

            } case 5: {
                if (array.empty()) {
                    std::cout << "No NPCs for battle!\n";
                    break;
                }
                
                if (array.size() == 1) {
                    std::cout << "Only one NPC - no one to fight with!\n";
                    break;
                }
                
                int distance;
                std::cout << "Enter battle distance (in meters): ";
                std::cin >> distance;
                
                if (distance <= 0) {
                    std::cout << "Distance must be positive!\n";
                    break;
                }
                
                std::cout << "\n=== BATTLE START ===\n";
                auto dead_list = fight(array, distance);
                std::cout << "\n=== BATTLE ENDED ===\n";
                
                std::cout << "Killed NPCs: " << dead_list.size() << "\n";
                for (auto& dead : dead_list) {
                    array.erase(dead);
                }
                
                if (!dead_list.empty()) {
                    std::cout << "Battle log written to file 'log.txt'\n";
                }
                break;

            } case 6: {
                std::cout << "How many test NPCs to generate? ";
                int count;
                std::cin >> count;
                
                if (count <= 0) {
                    std::cout << "Number must be positive!\n";
                    break;
                }
                
                std::string elf_names[] = {"Henry", "Edward", "Gorge", "Theodor", "Arthur"};
                std::string knight_names[] = {"Archi", "Lancel", "Gero", "Pedro", "Arthur"};
                std::string bandit_names[] = {"Robin", "John", "Will", "Alan", "Millie"};
                
                int generated = 0;
                int attempts = 0;
                const int MAX_ATTEMPTS = count * 100; //сколько попвток сделали, чтобы защитится от бесконечного цикла
                
                while (generated < count && attempts < MAX_ATTEMPTS) {
                    attempts++;
                    
                    NpcType type = static_cast<NpcType>(std::rand() % 3 + 1); //рандомный выбор типа и каст в enum
                    std::string name;
                    int x = std::rand() % 500 + 1;
                    int y = std::rand() % 500 + 1;
                    
                    std::string base_name;
                    switch(type) {
                        case ElfType:
                            base_name = elf_names[std::rand() % 5];
                            break;
                        case KnightType:
                            base_name = knight_names[std::rand() % 5];
                            break;
                        case BanditType:
                            base_name = bandit_names[std::rand() % 5];
                            break;
                    }
                    
                    name = base_name + "_" + std::to_string(std::rand() % 1000000);
                    
                    if (!is_name_unique(array, name)) {
                        continue;
                    }

                    auto npc = factory(type, name, x, y);
                    if (npc) {
                        npc->subscribe(std::make_shared<TextObserver>());
                        npc->subscribe(file_observer);
                        array.insert(npc);
                        generated++;
                    }
                }
                
                if (generated < count) {
                    std::cout << "Generated only " << generated << " out of " << count << " NPCs (name uniqueness issues)\n";
                } else {
                    std::cout << "Generated " << generated << " test NPCs\n";
                }
                break;

            } case 0: {
                std::cout << "Exiting program...\n";
                break;

            } default: {
                std::cout << "Invalid choice! Try again.\n";
                break;
            }
        }

    } while (choice != 0);
    
    return 0;
}
