#pragma once
#include "npc.h"
#include <memory>
#include <string>

std::shared_ptr<NPC> factory(std::istream &is);
std::shared_ptr<NPC> factory(NpcType type, const std::string& name, int x, int y);
void save(const set_t &array, const std::string &filename);
set_t load(const std::string &filename);
set_t fight(const set_t &array, size_t distance);
std::ostream &operator<<(std::ostream &os, const set_t &array);
