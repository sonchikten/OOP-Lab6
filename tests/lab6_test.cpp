#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/factory.h"
#include "../include/battle_visitor.h"
#include "../include/observer.h"
#include <sstream>
#include <memory>

TEST(NPCTest, ConstructorAndGetters) {
    auto elf = factory(ElfType, "Legolas", 100, 200);
    
    EXPECT_NE(elf, nullptr);
    EXPECT_EQ(elf->get_name(), "Legolas");
    EXPECT_EQ(elf->get_type(), ElfType);
    EXPECT_TRUE(elf->is_elf());
    EXPECT_FALSE(elf->is_knight());
    EXPECT_FALSE(elf->is_bandit());
}

TEST(NPCTest, IsCloseCalculation) {
    auto elf1 = factory(ElfType, "Legolas", 100, 200);
    auto elf2 = factory(ElfType, "Elrond", 110, 210);
    
    EXPECT_TRUE(elf1->is_close(elf2, 15));
    EXPECT_FALSE(elf1->is_close(elf2, 10));
}

TEST(FactoryTest, CreateNPCFromParameters) {
    auto elf = factory(ElfType, "Legolas", 100, 200);
    EXPECT_NE(elf, nullptr);
    EXPECT_EQ(elf->get_name(), "Legolas");
    EXPECT_EQ(elf->get_type(), ElfType);
    
    auto knight = factory(KnightType, "Arthur", 150, 250);
    EXPECT_NE(knight, nullptr);
    EXPECT_EQ(knight->get_name(), "Arthur");
    EXPECT_EQ(knight->get_type(), KnightType);
    
    auto bandit = factory(BanditType, "Robin", 200, 300);
    EXPECT_NE(bandit, nullptr);
    EXPECT_EQ(bandit->get_name(), "Robin");
    EXPECT_EQ(bandit->get_type(), BanditType);
}

TEST(FactoryTest, CreateNPCFromStream) {
    std::stringstream ss;
    ss << ElfType << "\nLegolas\n100\n200\n";
    
    auto npc = factory(ss);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_name(), "Legolas");
    EXPECT_EQ(npc->get_type(), ElfType);
}

TEST(FactoryTest, SaveAndLoad) {
    set_t original_set;
    original_set.insert(factory(ElfType, "Legolas", 100, 200));
    original_set.insert(factory(KnightType, "Arthur", 150, 250));
    
    save(original_set, "test_save.txt");
    auto loaded_set = load("test_save.txt");
    
    EXPECT_EQ(original_set.size(), loaded_set.size());
    std::remove("test_save.txt");
}

TEST(BattleVisitorTest, ElfVsKnight) {
    BattleVisitor visitor;
    auto elf = factory(ElfType, "Legolas", 100, 200);
    auto knight = factory(KnightType, "Arthur", 110, 210);
    
    bool result = elf->accept(visitor, knight);
    EXPECT_FALSE(result);
}

TEST(BattleVisitorTest, KnightVsBandit) {
    BattleVisitor visitor;
    auto knight = factory(KnightType, "Arthur", 100, 200);
    auto bandit = factory(BanditType, "Robin", 110, 210);
    
    bool result = knight->accept(visitor, bandit);
    EXPECT_FALSE(result);
}

TEST(BattleVisitorTest, BanditVsElf) {
    BattleVisitor visitor;
    auto bandit = factory(BanditType, "Robin", 100, 200);
    auto elf = factory(ElfType, "Legolas", 110, 210);
    
    bool result = bandit->accept(visitor, elf);
    EXPECT_FALSE(result);
}

TEST(BattleVisitorTest, SameTypeFight) {
    BattleVisitor visitor;
    auto elf1 = factory(ElfType, "Legolas", 100, 200);
    auto elf2 = factory(ElfType, "Elrond", 110, 210);
    
    bool result = elf1->accept(visitor, elf2);
    EXPECT_FALSE(result);
}

TEST(FightTest, BasicFight) {
    set_t npcs;
    auto elf = factory(ElfType, "Legolas", 100, 200);
    auto knight = factory(KnightType, "Arthur", 101, 201);
    
    npcs.insert(elf);
    npcs.insert(knight);
    
    class MockObserver : public IFightObserver {
        void on_fight(const std::shared_ptr<NPC> attacker, 
                     const std::shared_ptr<NPC> defender, 
                     bool win) override {}
    };
    auto mock_observer = std::make_shared<MockObserver>();
    elf->subscribe(mock_observer);
    knight->subscribe(mock_observer);
    
    auto dead = fight(npcs, 10);
    
    EXPECT_EQ(dead.size(), 1);
    EXPECT_TRUE(dead.count(knight));
}

TEST(FightTest, TooFarToFight) {
    set_t npcs;
    npcs.insert(factory(ElfType, "Legolas", 100, 200));
    npcs.insert(factory(KnightType, "Arthur", 400, 400));
    
    auto dead = fight(npcs, 50);
    
    EXPECT_EQ(dead.size(), 0);
}

TEST(ObserverTest, TextObserverOutput) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    TextObserver observer;
    auto elf = factory(ElfType, "Legolas", 100, 200);
    auto knight = factory(KnightType, "Arthur", 110, 210);
    
    observer.on_fight(elf, knight, true);
    
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Murder") != std::string::npos);
    EXPECT_TRUE(output.find("Legolas") != std::string::npos);
    EXPECT_TRUE(output.find("Arthur") != std::string::npos);
}

TEST(ObserverTest, FileObserverCreatesFile) {
    std::string test_filename = "test_log.txt";
    
    {
        FileObserver observer(test_filename);
        auto elf = factory(ElfType, "Legolas", 100, 200);
        auto knight = factory(KnightType, "Arthur", 110, 210);
        
        observer.on_fight(elf, knight, true);
    }
    
    std::ifstream file(test_filename);
    EXPECT_TRUE(file.is_open());
    
    std::string line;
    std::getline(file, line);
    EXPECT_TRUE(line.find("Legolas") != std::string::npos);
    EXPECT_TRUE(line.find("Arthur") != std::string::npos);
    
    file.close();
    std::remove(test_filename.c_str());
}
