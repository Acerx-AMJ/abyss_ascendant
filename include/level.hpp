#ifndef AA_LEVEL_HPP
#define AA_LEVEL_HPP

#include "save.hpp"
#include "entities.hpp"
#include <memory>

// Enums for handling levels.
enum class goal {getCoins, someCoins, point, area, aboveX, aboveY, belowX, belowY, turns, survive, getHit};
enum class storyType {intro, outro, death};
enum class endState {won, lost, quit};

// Level class for playing and working with levels.
class Level
{
public:
   Level(const std::string& name, Vec2 size, int index, const std::vector<std::vector<int>>& map, const std::vector<std::vector<std::string>>& intro, const std::vector<std::vector<std::string>>& outro, const std::vector<std::vector<std::string>>& death, const std::vector<std::vector<int>>& condition, PlayerData& data);

   // Move all of the enemies.
   void moveEnemies(Vec2 playerPos);

   // Manipulate the level tiles.
   void setTile(Vec2 pos, tileType tile);
   tileType getTile(Vec2 pos) const;
   tileType moveTile(Vec2 pos, Vec2 target, tileType type);
   Vec2 findFirstTile(tileType tile) const;

   // Check if the player has won based on win conditions.
   bool won(Vec2 pos);

   // Display level info to the user.
   void story(storyType type) const;
   void printConditions(Vec2 pos);
   void printMap(Vec2 pos);

   // Increase a counter by one.
   void incrementCoins();
   void incrementTurn();

   // Clears the previous map from the terminal.
   void clear() const;

   // Check if the given position is inside bounds of the map.
   bool withinBounds(Vec2 pos) const;

   // Retrieve the level index.
   int getIndex() const;
   bool killPlayerAfterWinning() const;
   const std::vector<std::vector<int>>& getMap() const;

private:
   std::string name;
   Vec2 size;
   int index;

   std::vector<std::vector<int>> map;
   std::vector<std::vector<std::string>> intro, outro, death;
   std::vector<std::vector<int>> condition;

   PlayerData& data;

   int turn {};
   int coinCount {}, coins {};
   std::vector<std::unique_ptr<Enemy>> enemies {};

   // Replace all occurrences of "$PLR" to the player's name.
   std::string replace(const std::string& string) const;
};

// Play a level, either story or endless.
endState playLevel(Level& level);

#endif
