#pragma once
#include "lua.hpp"

// Define story to get intro for every endless stage and daily for getting daily level layouts.
inline Lua story("endless.lua"s);
inline Lua daily("layouts.lua"s);

// Convert a map to string and vice versa for saving purposes.
str mapToString(const m_uint& map);
m_uint stringToMap(vstr map);

// Randomly choose an enemy based on score.
uint16 getRandomEnemy(uint64 score, const std::vector<std::pair<uint16, uint8>>& rates);

// Place coins and enemies around the map.
void dressLevel(m_uint& map, const Vec2& size, const Vec2& player, uint16 enemies, uint16 coins, const std::vector<std::pair<uint16, uint8>>& rates, uint64 score = 0ul);

// Generate a new level.
Level generateEndless(PlayerData& data, uint64 score, bool pickedSave, vstr stringMap = "");
Level generateDaily(PlayerData& data);

// Play endless until the player dies or quits.
void endless(Save& save, PlayerData& data);