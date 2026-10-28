#ifndef AA_LUA_HPP
#define AA_LUA_HPP

#include "level.hpp"
#include <lua-5.1/lua.hpp>

// Lua class for opening and executing scripts.
class Lua
{
public:
   Lua(const std::string& file);
   ~Lua();

   // Reload lua scripts.
   void reload();

   // Retrieve a specific type table from lua.
   std::vector<std::vector<int>> getNumberTable();
   std::vector<std::vector<std::string>> getStringTable();

   // Load a single level based on index and return it. Index is lua based.
   Level loadLevel(int index, PlayerData& data);
   Vec2 loadChapter(int index);
   std::vector<std::string> getLevelNames(int from, int to);
   std::vector<std::string> getChapters();
   std::vector<std::pair<std::string, int>> getLeftOverLevels();
   int getLevelCount();

   // Return dialogue at the given score.
   std::vector<std::vector<std::string>> getStory(unsigned long score);

   // Work with the daily layouts, like retrieving count, layout and size.
   int getDailyLayoutSize();
   std::pair<std::vector<std::vector<int>>, Vec2> getDailyLayout(int index);
   std::pair<std::vector<std::pair<int, int>>, int> getDailyRates(int index);

private:
   std::string file {};
   lua_State* lua {};
};

#endif // AA_LUA_HPP
