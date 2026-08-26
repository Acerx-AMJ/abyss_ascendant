#include "lua.hpp"
#include "io.hpp"

// Create a new lua class in the given script.
Lua::Lua(const str& file)
   : file(file)
{
   // Create a new lua state and open libraries.
   lua = luaL_newstate();
   luaL_openlibs(lua);

   // Load and try to execute the lua file. Exits the program if it's not possible.
   if (luaL_loadfile(lua, ("script/"s + file).c_str()) || lua_pcall(lua, 0, 0, 0)) 
   {
      println("Fatal error: Could not execute script 'script/"s + file + "'."s, RED);
      lua_close(lua);
      exit(-1);
   }
}

// Close lua after the program is done.
Lua::~Lua()
{
   lua_close(lua);
}

// Reload lua file.
void Lua::reload()
{
   // Close lua first.
   lua_close(lua);

   // Create a new lua state.
   lua = luaL_newstate();
   luaL_openlibs(lua);

   // Load and try to execute the lua file again.
   if (luaL_loadfile(lua, ("script/"s + file).c_str()) || lua_pcall(lua, 0, 0, 0)) 
   {
      println("Fatal error: Could not reload script 'script/"s + file + "'."s, RED);
      lua_close(lua);
      exit(-1);
   }
}

// Return a number table from a lua script.
m_uint Lua::getNumberTable()
{
   m_uint result;
   lua_pushnil(lua);

   while (lua_next(lua, -2) != 0) 
   {
      v_uint row;
      lua_pushnil(lua);

      while (lua_next(lua, -2) != 0) 
      {
         row.push_back(lua_tointeger(lua, -1));
         lua_pop(lua, 1);
      }

      result.push_back(row);
      lua_pop(lua, 1);
   }

   return result;
}

// Retrieve a string table from a lua script.
m_str Lua::getStringTable()
{
   m_str result;
   lua_pushnil(lua);

   while (lua_next(lua, -2) != 0) 
   {
      v_str row;
      lua_pushnil(lua);

      while (lua_next(lua, -2) != 0) 
      {
         row.push_back(lua_tostring(lua, -1));
         lua_pop(lua, 1);
      }

      result.push_back(row);
      lua_pop(lua, 1);
   }
   return result;
}

// Load a level from a lua script.
Level Lua::loadLevel(uint16 index, PlayerData& data)
{
   // Get the selected levels table.
   lua_getglobal(lua, "Levels");
   lua_pushinteger(lua, index);
   lua_gettable(lua, -2);

   // Get level name.
   lua_getfield(lua, -1, "name");
   const str name {lua_tostring(lua, -1)};
   lua_pop(lua, 1);

   // Get level layout.
   lua_getfield(lua, -1, "layout");
   const m_uint map {getNumberTable()};
   lua_pop(lua, 1);

   // Get dialogue tables.
   lua_getfield(lua, -1, "intro");
   const m_str intro {getStringTable()};
   lua_pop(lua, 1);

   lua_getfield(lua, -1, "outro");
   const m_str outro {getStringTable()};
   lua_pop(lua, 1);

   lua_getfield(lua, -1, "death");
   const m_str death {getStringTable()};
   lua_pop(lua, 1);

   // Get level conditions.
   lua_getfield(lua, -1, "goal");
   const m_uint goal {getNumberTable()};
   lua_pop(lua, 1);

   // Clean up and return the level.
   lua_pop(lua, 1);
   lua_pop(lua, 1);

   return Level(name, Vec2(map.at(0).size(), map.size()), index, map, intro, outro, death, goal, data);
}

// Load the given chapter.
Vec2 Lua::loadChapter(uint16 index)
{
   // Get the selected chapters table.
   lua_getglobal(lua, "Chapters");
   lua_pushinteger(lua, index);
   lua_gettable(lua, -2);

   // Get the from and to variables.
   lua_getfield(lua, -1, "from");
   const uint16 from = lua_tonumber(lua, -1);
   lua_pop(lua, 1);

   lua_getfield(lua, -1, "to");
   const uint16 to = lua_tonumber(lua, -1);
   lua_pop(lua, 1);

   // Clean up and return the from and to variables.
   lua_pop(lua, 1);
   lua_pop(lua, 1);

   return Vec2(from, to);
}

// Get all of the level names.
v_str Lua::getLevelNames(uint16 from, uint16 to)
{
   v_str names;
   const uint16 levelCount {getLevelCount()};

   lua_getglobal(lua, "Levels");

   // Loop through levels and retrieve all of the names.
   for (uint16 i = from - 1; i < to && i < levelCount; i++) 
   {
      // Select the Levels table.
      lua_pushinteger(lua, i + 1);
      lua_gettable(lua, -2);

      // Get level name.
      lua_getfield(lua, -1, "name");
      names.push_back(lua_tostring(lua, -1));
      
      lua_pop(lua, 1);
      lua_pop(lua, 1);
   }

   lua_pop(lua, 1);
   return names;
}

// Get all of the chapter names.
v_str Lua::getChapters()
{
   v_str chapters;

   lua_getglobal(lua, "Chapters");
   const uint16 count = lua_objlen(lua, -1);

   // Loop through levels and retrieve all of the names.
   for (uint16 i {}; i < count; i++) 
   {
      // Select the Levels table.
      lua_pushinteger(lua, i + 1);
      lua_gettable(lua, -2);

      // Get level name.
      lua_getfield(lua, -1, "name");
      chapters.push_back(lua_tostring(lua, -1));
      
      lua_pop(lua, 1);
      lua_pop(lua, 1);
   }

   lua_pop(lua, 1);
   return chapters;
}

// Get the number of levels from lua script.
uint16 Lua::getLevelCount()
{
   lua_getglobal(lua, "Levels");
   const uint16 count = lua_objlen(lua, -1);
   lua_pop(lua, 1);

   return count;
}

// Get the left over levels that are not in any chapters.
std::vector<std::pair<str, uint16>> Lua::getLeftOverLevels()
{
   const uint16 levelCount {getLevelCount()};

   // Get the chapter size.
   lua_getglobal(lua, "Chapters");
   const uint16 chapterCount = lua_objlen(lua, -1);
   lua_pop(lua, 1);

   std::vector<Vec2> chapters;

   for (uint16 i {1}; i <= chapterCount; i++)
      chapters.push_back(loadChapter(i));

   std::vector<std::pair<str, uint16>> levels;

   // Find the left over levels that aren't in any chapters.
   for (uint16 i {1}; i <= levelCount; i++)
   {
      bool covered {false};

      // Match every chapter.
      for (const auto& chapter : chapters)
      {
         if (i < chapter.x || i > chapter.y) continue;
         covered = true;
         break;
      }

      if (!covered)
         levels.push_back({getLevelNames(i, i).at(0), i});
   }

   return levels;
}

// Return endless dialogue at the given score.
m_str Lua::getStory(uint64 score)
{
   lua_getglobal(lua, "Story");
   const uint16 size = lua_objlen(lua, -1);

   // Loop through endless dialogue and return the first story to match the score.
   for (uint16 i {}; i < size; i++)
   {
      lua_pushinteger(lua, i + 1);
      lua_gettable(lua, -2);

      // Get score and see if it matches.
      lua_getfield(lua, -1, "score");

      if (lua_tonumber(lua, -1) == score) 
      {
         lua_pop(lua, 1);
         lua_getfield(lua, -1, "intro");

         const m_str story {getStringTable()};
         lua_pop(lua, 2);
         return story;
      }
      
      lua_pop(lua, 2);
   }

   lua_pop(lua, 1);
   return {{"SKIP"}};
}

// Get the number of daily level layouts.
uint16 Lua::getDailyLayoutSize()
{
   lua_getglobal(lua, "Levels");
   const uint16 count = lua_objlen(lua, -1);
   lua_pop(lua, 1);

   return count;
}

// Retrieve the daily level layout.
std::pair<m_uint, Vec2> Lua::getDailyLayout(uint16 index)
{
   // Get the selected levels table.
   lua_getglobal(lua, "Levels");
   lua_pushinteger(lua, index);
   lua_gettable(lua, -2);

   // Get level layout.
   lua_getfield(lua, -1, "layout");
   m_uint map {getNumberTable()};
   lua_pop(lua, 1);

   // Clean up and return the level.
   lua_pop(lua, 1);
   lua_pop(lua, 1);

   return {map, Vec2(map.at(0).size(), map.size())};
}

// Retrieve the daily layout spawn rates.
std::pair<std::vector<std::pair<uint16, uint8>>, uint16> Lua::getDailyRates(uint16 index)
{
   // Get the selected levels table.
   lua_getglobal(lua, "Levels");
   lua_pushinteger(lua, index);
   lua_gettable(lua, -2);

   // Get the enemy count.
   lua_getfield(lua, -1, "enemyCount");
   const uint16 count = lua_tointeger(lua, -1);
   lua_pop(lua, 1);

   // Get the spawn rates.
   lua_getfield(lua, -1, "enemyRates");
   std::vector<std::pair<uint16, uint8>> rates {};
   lua_pushnil(lua);

   // Retrieve the spawn rates.
   while (lua_next(lua, -2) != 0) 
   {
      // Get the enemy type.
      lua_pushinteger(lua, 1);
      lua_gettable(lua, -2);
      const uint16 first = lua_tointeger(lua, -1);
      lua_pop(lua, 1);

      // Get the spawn rate.
      lua_pushinteger(lua, 2);
      lua_gettable(lua, -2);
      const uint8 second = lua_tointeger(lua, -1);
      lua_pop(lua, 1);

      rates.push_back({first, second});
      lua_pop(lua, 1);
   }

   // Clean up and return the level.
   lua_pop(lua, 1);
   lua_pop(lua, 1);
   lua_pop(lua, 1);

   return {rates, count};
}
