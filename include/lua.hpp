#ifndef AA_LUA_HPP
#define AA_LUA_HPP

#include "level.hpp"
#include <lua-5.1/lua.hpp>

// Lua class for opening and executing scripts.
class Lua
{
public:
   Lua(const str& file);
   ~Lua();

   // Reload lua scripts.
   void reload();

   // Retrieve a specific type table from lua.
   m_uint getNumberTable();
   m_str getStringTable();

   // Load a single level based on index and return it. Index is lua based.
   Level loadLevel(uint16 index, PlayerData& data);
   Vec2 loadChapter(uint16 index);
   v_str getLevelNames(uint16 from, uint16 to);
   v_str getChapters();
   std::vector<std::pair<str, uint16>> getLeftOverLevels();
   uint16 getLevelCount();

   // Return dialogue at the given score.
   m_str getStory(uint64 score);

   // Work with the daily layouts, like retrieving count, layout and size.
   uint16 getDailyLayoutSize();
   std::pair<m_uint, Vec2> getDailyLayout(uint16 index);
   std::pair<std::vector<std::pair<uint16, uint8>>, uint16> getDailyRates(uint16 index);

private:
   str file {};
   lua_State* lua {};
};

#endif // AA_LUA_HPP
