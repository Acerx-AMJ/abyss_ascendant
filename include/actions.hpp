#pragma once
#include "lua.hpp"

// Let the user create their own color or icon.
str createColor();
str createIcon();

// Let the user decide how they'll appear in game.
str selectColor(uint64 highScore);
str selectIcon(uint64 highScore);
str selectUsername(Save& save);

// Makes the user create a new save.
void createSave(Save& save, PlayerData& data);

// Lets the user select what to customize, name, icon, color or reset high score.
void customize(Save& save, PlayerData& data);

// Let the user decide what to do next.
Level mainMenu(Save& save, Lua& lua, PlayerData& data);
Level manageSave(Save& save, Lua& lua, PlayerData& data);