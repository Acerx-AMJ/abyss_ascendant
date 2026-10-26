#ifndef AA_LEVEL_HPP
#define AA_LEVEL_HPP

#include "save.hpp"
#include "entities.hpp"
#include <memory>

// Enums for handling levels.
enum class goal      : uint8 {getCoins, someCoins, point, area, aboveX, aboveY, bellowX, bellowY, turns, survive, getHit};
enum class storyType : uint8 {intro, outro, death};
enum class endState  : uint8 {won, lost, quit};

// Level class for playing and working with levels.
class Level
{
public:
   Level(const str& name, const Vec2& size, uint16 index, const m_uint& map, const m_str& intro, const m_str& outro, const m_str& death, const m_uint& condition, PlayerData& data);

   // Move all of the enemies.
   void moveEnemies(const Vec2& playerPos);

   // Manipulate the level tiles.
   void setTile(const Vec2& pos, tileType tile);
   tileType getTile(const Vec2& pos) const;
   tileType moveTile(const Vec2& pos, const Vec2& target, tileType type);
   Vec2 findFirstTile(tileType tile) const;

   // Check if the player has won based on win conditions.
   bool won(const Vec2& pos);

   // Display level info to the user.
   void story(storyType type) const;
   void printConditions(const Vec2& pos);
   void printMap(const Vec2& pos);

   // Increase a counter by one.
   void incrementCoins();
   void incrementTurn();

   // Clears the previous map from the terminal.
   void clear() const;

   // Check if the given position is inside bounds of the map.
   bool withinBounds(const Vec2& pos) const;

   // Retrieve the level index.
   uint16 getIndex() const;
   bool killPlayerAfterWinning() const;
   const m_uint& getMap() const;

private:
   str name;
   Vec2 size;
   uint16 index;

   m_uint map;
   m_str intro, outro, death;
   m_uint condition;

   PlayerData& data;

   uint32 turn {};
   uint16 coinCount {}, coins {};
   std::vector<std::unique_ptr<Enemy>> enemies {};

   // Replace all occurrences of "$PLR" to the player's name.
   str replace(const str& string) const;
};

// Play a level, either story or endless.
endState playLevel(Level& level);

#endif
