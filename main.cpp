#include "../include/npc.h"
#include "../include/factory.h"
#include "../include/observer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

set_t fight(const set_t &array, size_t distance);  

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
    std::ifstream is(filename);
    if (is.good() && is.is_open()) {
        int count;
        is >> count;
        int loaded = 0;
        int skipped = 0;
        
        for (int i = 0; i < count; ++i) {
            auto npc = factory(is);
            if (npc) {
                if (!is_name_unique(result, npc->get_name())) {
                    std::cout << "Пропущен NPC с неуникальным именем: " 
                              << npc->get_name() << std::endl;
                    skipped++;
                    continue;
                }
                
                npc->subscribe(std::make_shared<TextObserver>());
                npc->subscribe(file_observer);
                result.insert(npc);
                loaded++;
            }
        }
        is.close();
        
        if (skipped > 0) {
            std::cout << "Загружено: " << loaded << ", пропущено (неуникальные имена): " << skipped << std::endl;
        }
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    return result;
}

int main() {
    std::srand(std::time(nullptr));
    set_t array;

    auto file_observer = std::make_shared<FileObserver>("log.txt");
    
    std::cout << "=== Редактор подземелья Balagur Fate 3 ===\n";
    
    int choice;
    do {
        std::cout << "\nМЕНЮ:\n";
        std::cout << "1. Добавить NPC\n";
        std::cout << "2. Сохранить в файл\n";
        std::cout << "3. Загрузить из файла\n";
        std::cout << "4. Показать всех NPC\n";
        std::cout << "5. Запустить бой\n";
        std::cout << "6. Сгенерировать тестовых NPC\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::string name;
                int type, x, y;
                
                std::cout << "\n--- Добавление нового NPC ---\n";
                std::cout << "Введите уникальное имя NPC: ";
                std::cin >> name;

                if (!is_name_unique(array, name)) {
                    std::cout << "Ошибка: NPC с именем '" << name << "' уже существует!\n";
                    break;
                }
                            
                std::cout << "Тип NPC:\n";
                std::cout << "  1 - Эльф (побеждает Рыцаря)\n";
                std::cout << "  2 - Рыцарь (побеждает Разбойника)\n";
                std::cout << "  3 - Разбойник (побеждает Эльфа)\n";
                std::cout << "Ваш выбор (1-3): ";
                std::cin >> type;
                
                if (type < 1 || type > 3) {
                    std::cout << "Ошибка: тип должен быть от 1 до 3!\n";
                    break;
                }
                
                std::cout << "Координата X (0 < X <= 500): ";
                std::cin >> x;
                std::cout << "Координата Y (0 < Y <= 500): ";
                std::cin >> y;

                if (x <= 0 || x > 500 || y <= 0 || y > 500) {
                    std::cout << "Ошибка: координаты должны быть в диапазоне (0, 500]!\n";
                    break;
                }

                auto npc = factory(static_cast<NpcType>(type), name, x, y);
                if (npc) {
                    npc->subscribe(std::make_shared<TextObserver>());
                    npc->subscribe(file_observer);
                    array.insert(npc);
                    std::cout << "✓ NPC '" << name << "' успешно добавлен!\n";
                }
                break;

            } case 2: {
                if (array.empty()) {
                    std::cout << "Нет NPC для сохранения!\n";
                    break;
                }
                
                std::string filename;
                std::cout << "Введите имя файла для сохранения: ";
                std::cin >> filename;
                
                save(array, filename);
                std::cout << "✓ Сохранено " << array.size() 
                         << " NPC в файл '" << filename << "'\n";
                break;

            } case 3: {
                std::string filename;
                std::cout << "Введите имя файла для загрузки: ";
                std::cin >> filename;
                
                std::ifstream test_file(filename);
                if (!test_file.is_open()) {
                    std::cout << "✗ Ошибка: файл '" << filename << "' не найден!\n";
                    break;
                }
                test_file.close();
                
                auto loaded_set = load_with_validation(filename, file_observer);
                
                set_t final_set = array; 
                
                int conflicts = 0;
                for (const auto& new_npc : loaded_set) {
                    if (!is_name_unique(final_set, new_npc->get_name())) {
                        std::cout << "Конфликт имен: '" << new_npc->get_name() 
                                << "' уже существует. NPC пропущен.\n";
                        conflicts++;
                    } else {
                        final_set.insert(new_npc);
                    }
                }
                
                array = final_set;
                
                std::cout << "✓ Загружено " << (loaded_set.size() - conflicts) 
                        << " NPC из файла '" << filename << "'\n";
                if (conflicts > 0) {
                    std::cout << " Пропущено из-за конфликтов имен: " << conflicts << std::endl;
                }
                break;

            } case 4: {
                std::cout << "\n=== Список всех NPC (" << array.size() << " шт.) ===\n";
                if (array.empty()) {
                    std::cout << "Нет NPC\n";
                } else {
                    std::cout << array;
                }
                break;

            } case 5: {
                if (array.empty()) {
                    std::cout << "Нет NPC для боя!\n";
                    break;
                }
                
                if (array.size() == 1) {
                    std::cout << "Только один NPC - не с кем драться!\n";
                    break;
                }
                
                int distance;
                std::cout << "Введите дальность боя (в метрах): ";
                std::cin >> distance;
                
                if (distance <= 0) {
                    std::cout << "Дальность должна быть положительной!\n";
                    break;
                }
                
                std::cout << "\n=== НАЧАЛО БОЯ ===\n";
                auto dead_list = fight(array, distance);
                std::cout << "\n=== БОЙ ЗАВЕРШЕН ===\n";
                
                std::cout << "Убито NPC: " << dead_list.size() << "\n";
                for (auto& dead : dead_list) {
                    array.erase(dead);
                }
                
                if (!dead_list.empty()) {
                    std::cout << "Лог битв записан в файл 'log.txt'\n";
                }
                break;

            } case 6: {
                std::cout << "Сколько тестовых NPC сгенерировать? ";
                int count;
                std::cin >> count;
                
                if (count <= 0) {
                    std::cout << "Число должно быть положительным!\n";
                    break;
                }
                
                std::string elf_names[] = {"Legolas", "Elrond", "Galadriel", "Thranduil", "Arwen"};
                std::string knight_names[] = {"Arthur", "Lancelot", "Gawain", "Percival", "Galahad"};
                std::string bandit_names[] = {"Robin", "John", "Will", "Alan", "Much"};
                
                int generated = 0;
                int attempts = 0;
                const int MAX_ATTEMPTS = count * 10;
                
                while (generated < count && attempts < MAX_ATTEMPTS) {
                    attempts++;
                    
                    NpcType type = static_cast<NpcType>(std::rand() % 3 + 1);
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
                    std::cout << " Сгенерировано только " << generated 
                              << " из " << count << " NPC (проблемы с уникальностью имен)\n";
                } else {
                    std::cout << "✓ Сгенерировано " << generated << " тестовых NPC\n";
                }
                break;

            } case 0: {
                std::cout << "Выход из программы...\n";
                break;

            } default: {
                std::cout << "Неверный выбор! Попробуйте снова.\n";
                break;
            }
        }

    } while (choice != 0);
    
    return 0;
}
