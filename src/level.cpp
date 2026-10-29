#include "level.hpp"
#include "io.hpp"
#include "audio.hpp"
#include <map>

// User input map.
const std::map<int, Vec2> inputMap {{
   {'s', Vec2()}, {'w', Vec2::up}, {'e', Vec2(1, -1)}, {'d', Vec2::right}, {'c', Vec2(1)}, 
   {'x', Vec2::down}, {'z', Vec2(-1, 1)}, {'a', Vec2::left}, {'q', Vec2(-1)},
}};

// Create a new level for the user to play.
Level::Level(
   const std::string& name, 
   Vec2 size, 
   int index,
   const std::vector<std::vector<int>>& map,
   const std::vector<std::vector<std::string>>& intro, 
   const std::vector<std::vector<std::string>>& outro, 
   const std::vector<std::vector<std::string>>& death, 
   const std::vector<std::vector<int>>& condition, 
   PlayerData& data)
   : name(name), size(size), index(index), map(map), intro(intro), outro(outro), death(death), condition(condition), data(data)
{
   int playerCount {};

   // Count coin and player tiles and setup the enemies.
   for (int y {}; y < size.y; y++) 
   {
      for (int x {}; x < size.x; x++) 
      {
         switch (getTile(Vec2(x, y))) 
         {
         case tileType::coin:     coinCount++; break;
         case tileType::player:   playerCount++; break;
         case tileType::vBat:     enemies.push_back(std::make_unique<VBat>(*this, Vec2(x, y)));     break;
         case tileType::hBat:     enemies.push_back(std::make_unique<HBat>(*this, Vec2(x, y)));     break;
         case tileType::glimmer:  enemies.push_back(std::make_unique<Glimmer>(*this, Vec2(x, y)));  break;
         case tileType::seeker:   enemies.push_back(std::make_unique<Seeker>(*this, Vec2(x, y)));   break;
         case tileType::lurker:   enemies.push_back(std::make_unique<Lurker>(*this, Vec2(x, y)));   break;
         case tileType::wanderer: enemies.push_back(std::make_unique<Wanderer>(*this, Vec2(x, y))); break;
         case tileType::chaser:   enemies.push_back(std::make_unique<Chaser>(*this, Vec2(x, y)));   break;
         case tileType::observer: enemies.push_back(std::make_unique<Observer>(*this, Vec2(x, y))); break;
         case tileType::hunter:   enemies.push_back(std::make_unique<Hunter>(*this, Vec2(x, y)));   break;
         default: break;
         }
      }
   }

   // Player count in a level can only be 1.
   if (playerCount != 1) 
   {
      println("Fatal error: Level '" + name + "' contains an invalid amount of players (" + tostr(playerCount) + ").", RED);
      exit(-1);
   }
}

// Move all of the enemies.
void Level::moveEnemies(Vec2 playerPos)
{
   for (int i {}; i < enemies.size(); i++)
      enemies.at(i)->move(playerPos);
}

// Set a tile to the given tile at the given position.
void Level::setTile(Vec2 pos, tileType tile)
{
   if (withinBounds(pos))
      map.at(pos.y).at(pos.x) = static_cast<int>(tile);
}

// Get the tile from the level if it's within bounds.
tileType Level::getTile(Vec2 pos) const
{
   if (withinBounds(pos)) 
      return static_cast<tileType>(map.at(pos.y).at(pos.x));
   else
      return tileType::obstacle;
}

// Find first tile that matches the given tile.
Vec2 Level::findFirstTile(tileType tile) const
{
   for (int y {}; y < size.y; y++)
      for (int x {}; x < size.x; x++)
         if (getTile(Vec2(x, y)) == tile) 
            return Vec2(x, y);

   return Vec2::invalid;
}

// Move the tile to a new target destination.
tileType Level::moveTile(Vec2 pos, Vec2 target, tileType type)
{
   const tileType currentTile {getTile(pos)};
   const tileType targetTile {getTile(target)};

   // Swap tiles and return the target tile.
   setTile(pos, type);
   setTile(target, currentTile);
   return targetTile;
}

// Check if the player has won the level.
bool Level::won(Vec2 pos)
{
   if (killPlayerAfterWinning() && getTile(pos) != tileType::player)
   {
      setTile(pos, tileType::player);
      return true;
   }

   for (const auto& cond : condition) 
   {
      switch (static_cast<goal>(cond.at(0))) 
      {
      case goal::getCoins:   if (coins < coinCount)       return false; break;
      case goal::someCoins:  if (coins < cond.at(1))      return false; break;
      case goal::survive:    if (turn < cond.at(1))       return false; break;
      case goal::turns:      if (turn > cond.at(1))       return false; break;
      case goal::aboveX:     if (pos.x <= cond.at(1) - 1) return false; break;
      case goal::aboveY:     if (pos.y >= cond.at(1) - 1) return false; break;
      case goal::belowX:     if (pos.x >= cond.at(1) - 1) return false; break;
      case goal::belowY:     if (pos.y <= cond.at(1) - 1) return false; break;
      case goal::point:  if (pos != Vec2(cond.at(1) - 1, cond.at(2) - 1)) return false; break;
      case goal::area:   if (!pos.within(Vec2(cond.at(1) - 1, cond.at(2) - 1), Vec2(cond.at(3) - 1, cond.at(4) - 1))) return false; break;
      case goal::getHit: return false;
      default: break;
      }
   }
   // All of the conditions match.
   return true;
}

// Display story text by text.
void Level::story(storyType type) const {
   const std::vector<std::vector<std::string>> *text;
   const char *storyName;

   if (type == storyType::intro) {
      text = &intro;
      storyName = "intro";
   }
   else if (type == storyType::outro) {
      text = &outro;
      storyName = "outro";
   }
   else if (type == storyType::death) {
      text = &death;
      storyName = "death";
   }

   // skip the story
   if (text->empty()) {
      return;
   }

   if (getHiddenInputLoud("\nStory (enter to continue, s to skip) > ", YELLOW) == "s") {
      return;
   }

   // display the story
   int path = 0;
   std::string sound = "click";
   std::string endSound = "accent";
   std::string optionSound = "option";

   for (const std::vector<std::string> &entry: *text) {
      if (entry.empty()) {
         continue;
      }

      if (!entry[0].empty() && entry[0][0] != '&') {
         path = 0;
      }

      if (entry[0].size() > 1 && entry[0][0] == '&') {
         std::string substr = entry[0].substr(1);
         int entryPath = 0;
         try { entryPath = std::stoi(substr); }
         catch (...) {
            printf("%slevels.lua@%s.%s: Invalid path '%s'.%s\n", RED, name.c_str(), storyName, entry[0].c_str(), RES);
            continue;
         }

         if (entryPath != path) {
            continue;
         }
      }

      bool onPath = (path > 0);
      if (entry.size() <= onPath) {
         continue;
      }

      // commands
      if (entry[onPath].size() == 2 + onPath && entry[onPath] == "$SET_SOUND") {
         sound = entry[onPath + 1];
         continue;
      }

      if (entry[onPath].size() == 2 + onPath && entry[onPath] == "$SET_END_SOUND") {
         endSound = entry[onPath + 1];
         continue;
      }

      if (entry[onPath].size() == 2 + onPath && entry[onPath] == "$SET_OPTION_SOUND") {
         optionSound = entry[onPath + 1];
         continue;
      }

      // dialogue options
      if (entry[onPath].size() > 1 && entry[onPath][0] == '#') {
         std::string substr = entry[onPath].substr(1);
         int options = 0;
         try { options = std::stoi(substr); }
         catch (...) {
            printf("%slevels.lua@%s.%s: Invalid dialogue option count '%s'.%s\n", RED, name.c_str(), storyName, entry[onPath].c_str(), RES);
            continue;
         }

         int start = 2 + onPath;
         int expectedCount = start + options * 2;
         if (entry.size() != (size_t)expectedCount) {
            printf("%slevels.lua@%s.%s: Expected %d entries, got %zu instead.%s\n", RED, name.c_str(), storyName, expectedCount, entry.size(), RES);
            continue;
         }

         playSound(optionSound);
         printf("\n%sOption:%s\n", YELLOW, RES);

         for (int i = start; i < start + options; ++i) {
            printf(" > [%2d] %s\n", (i - start) + 1, replace(entry[i]).c_str());
         }

         int selected = getNumber("\nYour choice > ", [options](int num) -> bool {
            return num >= 1 && num <= options;
         }) - 1;

         const std::string &selectedEntry = entry[start + selected];
         const std::string &answerEntry = entry[start + options + selected];

         playSound(sound);
         getHiddenInput(wrap(data.name, data.color) + ": " + replace(selectedEntry));

         if (answerEntry.size() > 1 && answerEntry[0] == '&') {
            std::string substr = answerEntry.substr(1);
            try { path = std::stoi(substr); }
            catch (...) {
               printf("%slevels.lua@%s.%s: Invalid path in dialogue answer '%s'.%s\n", RED, name.c_str(), storyName, entry[0].c_str(), RES);
            }
         }
         else {
            const std::string prompt = (entry[onPath + 1] == "$NAR" ? "" : entry[onPath + 1] + ": ") + replace(answerEntry);
            playSound(sound);
            getHiddenInput(prompt);
         }
      }
      else {
         const std::string user = (entry.size() == 1 + onPath || entry[onPath + 1] == "$PLR") ? wrap(data.name, data.color) : entry[onPath + 1];
         const std::string prompt = (user == "$NAR" ? "" : user + ": ") + replace(entry[onPath]);
         playSound(sound);
         getHiddenInput(prompt);
      }
   }
   playSound(endSound);
}

// Print all of the level conditions to the terminal.
void Level::printConditions(Vec2 pos)
{
   bool valid {};
   std::string info {};
   println();

   for (const auto& cond : condition)
   {
      switch (static_cast<goal>(cond.at(0)))
      {
      case goal::getCoins: 
         valid = coins >= coinCount;
         info = "Fragments collected: " + tostr(coins) + "/" + tostr(coinCount);
         break;
      case goal::someCoins:
         valid = coins >= cond.at(1);
         info = "Fragments collected: " + tostr(coins) + "/" + tostr(coinCount);
         break;
      case goal::aboveX:   
         valid = pos.x >= cond.at(1);
         info = "Be above X " + tostr(cond.at(1));
         break;
      case goal::aboveY:
         valid = pos.y <= cond.at(1) - 2;
         info = "Be above Y " + tostr(cond.at(1));
         break;
      case goal::belowX:
         valid = pos.x <= cond.at(1) - 2;
         info = "Be below X " + tostr(cond.at(1));
         break;
      case goal::belowY:
         valid = pos.y >= cond.at(1);
         info = "Be below Y " + tostr(cond.at(1));
         break;
      case goal::point:
         valid = pos == Vec2(cond.at(1) - 1, cond.at(2) - 1);
         info = "Be at coordinates X " + tostr(cond.at(1)) + " Y " + tostr(cond.at(2));
         break;
      case goal::area:
         valid = pos.within(Vec2(cond.at(1) - 1, cond.at(2) - 1), Vec2(cond.at(3) - 1, cond.at(4) - 1));
         info = "Be at area X " + tostr(cond.at(1)) + " Y " + tostr(cond.at(2)) + " to X " + tostr(cond.at(3)) + " Y " + tostr(cond.at(4)); 
         break;
      case goal::survive:
         valid = turn >= cond.at(1);
         info = "Survive for " + tostr(std::max(0, cond.at(1) - turn)) + " more turns";
         break;
      case goal::turns:
         valid = turn <= cond.at(1);
         info = "Complete in " + tostr(cond.at(1)) + " turns (turn " + tostr(turn) + ")";

         // Kill the player if they took too long.
         if (!valid) setTile(pos, tileType::death);
         break;
      case goal::getHit:
         valid = getTile(pos) != tileType::player;
         info = "???";
         break;
      default: break;
      }
      // Print the condition
      println((valid ? "✓ " : "✗ ") + info + ".", (valid ? GREEN : RED));
   }
}

// Print the map to the terminal.
void Level::printMap(Vec2 pos)
{
   printConditions(pos);

   for (const auto& row : map)
   {
      for (const auto& tile : row)
      {
         switch (static_cast<tileType>(tile))
         {
         case tileType::player:   print((utf32Size(data.icon) == 1 ? " " : "") + data.icon, data.color); break;
         case tileType::empty:    print("  "); break;
         case tileType::coin:     print(" ○", YELLOW); break;
         case tileType::nyx:      print(" ♠", ORANGE); break;
         case tileType::jake:     print(" ♞", L_GREEN); break;
         case tileType::gabriel:  print(" ♦", PURPLE); break;
         case tileType::bricks:   print("██", D_BROWN); break;
         case tileType::obstacle: print("██", D_GRAY); break;
         case tileType::hBat:     print(" ═", RED); break;
         case tileType::vBat:     print(" ║", RED); break;
         case tileType::glimmer:  print(" ◆", RED); break;
         case tileType::seeker:   print(" ◊", RED); break;
         case tileType::lurker:   print(" ⯎", RED); break;
         case tileType::wanderer: print(" ✡", RED); break;
         case tileType::chaser:   print(" ●", RED); break;
         case tileType::observer: print(" ⍙", RED); break;
         case tileType::hunter:   print(" @", RED); break;
         case tileType::mine:     print(" ✸", RED); break;
         case tileType::death:    print(" ✖", RED); break;
         default:                 print("??", PURPLE); break;
         }
      }
      println();
   }
   println();
}

// Increment the coin count by one and play a sound.
void Level::incrementCoins()
{
   playSound("coin");
   coins++;
}

// Increment the turn by one.
void Level::incrementTurn()
{
   turn++;
}

// Clear the level map from the terminal.
void Level::clear() const
{
   clearln(size.y + condition.size() + 5);
}

// Check if the position is within level bounds
bool Level::withinBounds(Vec2 pos) const
{
   return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

int Level::getIndex() const
{
   return index;
}

// Check if the level should kill the player after winning
bool Level::killPlayerAfterWinning() const
{
   return condition.size() == 1 && static_cast<goal>(condition.at(0).at(0)) == goal::getHit;
}

// Return the map for saving purposes
const std::vector<std::vector<int>>& Level::getMap() const
{
   return map;
}

// Replace all occurrences of $PLR to the player's username
std::string Level::replace(const std::string& prompt) const
{
   if (prompt.find("$PLR") == std::string::npos) 
      return prompt;

   std::string result {prompt};

   // Replace every single occurrence.
   while (true) 
   {
      const size_t pos {result.find("$PLR")};

      if (pos == std::string::npos)
         break;

      result.replace(pos, 4, wrap(data.name, data.color));
   }
   return result;
}

// Play a level from the provided level class.
endState playLevel(Level& level)
{
   Player player(level, level.findFirstTile(tileType::player));
   bool moved {true};

   // Runs until the player has won.
   while (!level.won(player.getPos())) 
   {
      if (moved) level.moveEnemies(player.getPos());
      level.printMap(player.getPos());

      // The player has died.
      if (!player.isAlive()) break;

      // Get input from the user and do complete the action.
      const char input {getKey("Input ('H' for help) > ", BLUE)};
      if (input != 'r') level.clear();
      moved = false;

      if (inputMap.count(input)) 
      {
         moved = true;
         level.incrementTurn();

         playSound("move");
         player.move(inputMap.at(input));
      }
      else if (input == 'r') 
      {
         if (getConsent("Are you sure that you want to quit? [y/n] > ", RED))
            return endState::quit;

         level.clear();
         playSound("failure");
      }
      else if (input == 'h') 
      {
         println("'QWEDCXZA' to move, 'S' to stay still, 'P' to see your position, 'R' to quit.", BLUE);
         playSound("accent");
      }
      else if (input == 'p') 
      {
         Vec2 pos {player.getPos()};
         println("Your position is X " + tostr(pos.x + 1) + ", Y " + tostr(pos.y + 1) + ".", BLUE);
         playSound("accent");
      }
      else 
      {
         println("Unknown command, type H for help.", RED);
         playSound("failure");
      }
   }

   // Return the corresponding end state if the player wins or dies in the level.
   if (player.isAlive()) 
   {
      if (level.killPlayerAfterWinning())
      {
         level.setTile(player.getPos(), tileType::death);
         playSound("died");
      }
      else playSound("win");

      level.printMap(player.getPos());
      return endState::won;
   }
   else 
   {
      println("Input\nInfo");
      level.clear();
      level.printMap(player.getPos());
      playSound("died");
   }
   return endState::lost;
}
