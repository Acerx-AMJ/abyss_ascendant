#pragma once
#include "lua.hpp"

// Define story to get intro for every endless stage and daily for getting daily level layouts.
inline Lua story("endless.lua");
inline Lua daily("layouts.lua");

// Convert a map to string and vice versa for saving purposes.
std::string mapToString(const std::vector<std::vector<int>>& map);
std::vector<std::vector<int>> stringToMap(std::string_view map);

// Randomly choose an enemy based on score.
int getRandomEnemy(unsigned long score, const std::vector<std::pair<int, int>>& rates);

// Place coins and enemies around the map.
void dressLevel(std::vector<std::vector<int>>& map, Vec2 size, Vec2 player, int enemies, int coins, const std::vector<std::pair<int, int>>& rates, unsigned long score = 0ul);

// Generate a new level.
Level generateEndless(PlayerData& data, unsigned long score, bool pickedSave, std::string_view stringMap = "");
Level generateDaily(PlayerData& data);

// Play endless until the player dies or quits.
void endless(Save& save, PlayerData& data);