#include "infinity.hpp"
#include "io.hpp"
#include "audio.hpp"
#include "algorithms.hpp"
#include <thread>

// Spawn rates for the endless mode.
const std::vector<std::pair<int, int>> spawnRates {{
   {int(tileType::mine), 50}, {int(tileType::hBat), 17}, {int(tileType::vBat), 17}, {int(tileType::glimmer), 11}, {int(tileType::observer), 2}, {int(tileType::lurker), 2}, {int(tileType::chaser), 1}
}};

// Convert a map to string for saving purposes.
std::string mapToString(const std::vector<std::vector<int>> &map) {
   std::string result;
   result.reserve(map.size() * (map.empty() ? 1 : map[0].size() + 1));

   for (const std::vector<int> &row: map) {
      for (int t: row) {
         tileType tile = (tileType)t;
         
         switch (tile) {
         case tileType::empty:    result.push_back('E'); break;
         case tileType::coin:     result.push_back('C'); break;
         case tileType::player:   result.push_back('P'); break;
         case tileType::nyx:      result.push_back('N'); break;
         case tileType::jake:     result.push_back('J'); break;
         case tileType::gabriel:  result.push_back('g'); break;
         case tileType::bricks:   result.push_back('B'); break;
         case tileType::obstacle: result.push_back('O'); break;
         case tileType::vBat:     result.push_back('V'); break;
         case tileType::hBat:     result.push_back('H'); break;
         case tileType::glimmer:  result.push_back('G'); break;
         case tileType::seeker:   result.push_back('S'); break;
         case tileType::lurker:   result.push_back('L'); break;
         case tileType::wanderer: result.push_back('W'); break;
         case tileType::chaser:   result.push_back('R'); break;
         case tileType::observer: result.push_back('0'); break;
         case tileType::hunter:   result.push_back('h'); break;
         case tileType::mine:     result.push_back('M'); break;
         case tileType::death:    result.push_back('D'); break;
         default:                 result.push_back('?'); break;
         }
      }
      result.push_back('#');
   }
   return result;
}

// Convert a string to a map for loading purposes.
std::vector<std::vector<int>> stringToMap(std::string_view map) {
   std::vector<std::vector<int>> result;
   std::vector<int> row;

   for (char tile: map) {
      switch (tile) {
      case 'E': row.push_back(int(tileType::empty)); break;
      case 'C': row.push_back(int(tileType::coin)); break;
      case 'P': row.push_back(int(tileType::player)); break;
      case 'N': row.push_back(int(tileType::nyx)); break;
      case 'J': row.push_back(int(tileType::jake)); break;
      case 'g': row.push_back(int(tileType::gabriel)); break;
      case 'B': row.push_back(int(tileType::bricks)); break;
      case 'O': row.push_back(int(tileType::obstacle)); break;
      case 'V': row.push_back(int(tileType::vBat)); break;
      case 'H': row.push_back(int(tileType::hBat)); break;
      case 'G': row.push_back(int(tileType::glimmer)); break;
      case 'S': row.push_back(int(tileType::seeker)); break;
      case 'L': row.push_back(int(tileType::lurker)); break;
      case 'W': row.push_back(int(tileType::wanderer)); break;
      case 'R': row.push_back(int(tileType::chaser)); break;
      case '0': row.push_back(int(tileType::observer)); break;
      case 'h': row.push_back(int(tileType::hunter)); break;
      case 'M': row.push_back(int(tileType::mine)); break;
      case 'D': row.push_back(int(tileType::death)); break;
      case '#': result.push_back(row); row.clear(); break;
      default:  row.push_back(-1); break; // Purposefuly overflow the int integer
      }
   }
   return result;
}

// Randomly choose an enemy based on score.
int getRandomEnemy(unsigned long score, const std::vector<std::pair<int, int>>& rates)
{
   int total {};
   std::vector<int> weights;

   // Calculate weights.
   for (const auto& enemy : rates) 
   {
      const int weight = enemy.second + score / 4;
      weights.push_back(weight);
      total += weight;
   }

   const int value = rand() % total;
   int cumulative {};

   // Get a random value and return an enemy based on it.
   for (int i {}; i < rates.size(); i++) 
   {
      cumulative += weights.at(i);

      if (value < cumulative)
         return rates.at(i).first;
   }
   return rates.back().first;
}

// Place coins and enemies around the map.
void dressLevel(std::vector<std::vector<int>>& map, Vec2 size, Vec2 player, int enemies, int coins, const std::vector<std::pair<int, int>>& rates, unsigned long score)
{
   // Add enemies.
   for (int i {}; i < enemies; i++) {
      Vec2 target(rand() % size.x, rand() % size.y);
      int count {};

      while (map.at(target.y).at(target.x) != int(tileType::empty) || player.distanceTo(target) < 4)
      {
         target = Vec2(rand() % size.x, rand() % size.y);
         if (++count > 10) break;
      }

      if (count <= 10)
         map.at(target.y).at(target.x) = getRandomEnemy(score, rates);
   }

   // Fill the map with coins.
   for (int i {}; i < coins; i++) {
      Vec2 target(rand() % size.x, rand() % size.y);
      int count {};

      while (map.at(target.y).at(target.x) != int(tileType::empty) || !aStar(player, target, map, size))
      {
         target = Vec2(rand() % size.x, rand() % size.y);
         if (++count > 10) break;
      }

      if (count <= 10)
         map.at(target.y).at(target.x) = int(tileType::coin);
   }
}

// Generate a new level.
Level generateEndless(PlayerData& data, unsigned long score, bool pickedSave, std::string_view stringMap)
{
   if (pickedSave) 
   {
      auto map {stringToMap(stringMap)};
      return Level("", Vec2(map.at(0).size(), map.size()), 0, map, story.getStory(score), {}, {}, {{int(goal::getCoins)}}, data);
   }

   srand(time(0));
   Vec2 size(rand() % std::min(8ul, (score / 3) + 1) + 24, rand() % std::min(4ul, (score / 6) + 1) + 12);

   // Generate a new map.
   Bsp bsp(size);
   std::vector<std::vector<int>> map {bsp.generate(9 - std::min(6ul, score / 6))};

   Vec2 playerPos(rand() % size.x, rand() % size.y);

   // Find a good place to put the player in.
   while (map.at(playerPos.y).at(playerPos.x) != int(tileType::empty))
      playerPos = Vec2(rand() % size.x, rand() % size.y);

   const int enemyCount = rand() % std::min(7ul, (score / 4) + 1) + score / 60;
   const int coinCount = rand() % std::min(12ul, (score / 4) + 1) + 3;

   // Add the player, enemies and coins to the level.
   dressLevel(map, size, playerPos, enemyCount, coinCount, spawnRates, score);
   map.at(playerPos.y).at(playerPos.x) = int(tileType::player);

   return Level("", size, 0, map, story.getStory(score), {{"$SKIP"}}, {{"$SKIP"}}, {{int(goal::getCoins)}}, data);
}

// Generate the daily level for today.
Level generateDaily(PlayerData& data)
{
   time_t now {time(0)};
   tm* local {localtime(&now)};

   // Calculate the seed based on the current date.
   const int seed {(local->tm_year + 1900) * 10000 + (local->tm_mon + 1) * 100 + local->tm_mday};
   srand(seed);

   // Get the layout index, the layout and the spawn rates.
   const int index = rand() % daily.getDailyLayoutSize() + 1;
   auto layout {daily.getDailyLayout(index)};
   auto rates {daily.getDailyRates(index)};

   Vec2 playerPos(rand() % layout.second.x, rand() % layout.second.y);

   // Find a good place to put the player in.
   while (layout.first.at(playerPos.y).at(playerPos.x) != int(tileType::empty))
      playerPos = Vec2(rand() % layout.second.x, rand() % layout.second.y);

   const int enemyCount = rand() % 3 + rates.second;
   const int coinCount = rand() % 10 + 5;

   // Place the player, enemies and coins around the level.
   dressLevel(layout.first, layout.second, playerPos, enemyCount, coinCount, rates.first);
   layout.first.at(playerPos.y).at(playerPos.x) = int(tileType::player);

   return Level("", layout.second, 0, layout.first, {{"SKIP"}}, {{"SKIP"}}, {{"SKIP"}}, {{int(goal::getCoins)}}, data);
}

// Play endless until the player dies or quits.
void endless(Save& save, PlayerData& data)
{
   auto progress {save.retrieveEndlessProgress(data.id)};
   bool pickedSave {progress.second && getConsent("Do you want to load your endless save file? (" + wrap(tostr(progress.second), YELLOW) + " score) [y/n] > ")};
   unsigned long score {(pickedSave ? progress.second : 0ul)};

   if (progress.second) 
      clearln(2);

   // Keep playing until the player dies or quits.
   while (true) 
   {
      Level level {generateEndless(data, score, pickedSave, progress.first)};

      if (pickedSave) 
      {
         pickedSave = false;
         save.deleteEndlessProgress(data.id);
      }

      if (data.highScore <= score) 
         level.story(storyType::intro);

      if (score == 0 || score % 5 == 0) 
         playSound("horn");
      
      println("\nYour current score: " + tostr(score) + (data.highScore < score ? wrap(" New Record!", YELLOW) : "") + "\n");
  
      // Play the level and return the result.
      endState result {playLevel(level)};

      if (result == endState::won) 
      {
         score++;
         getHiddenInput("\n(Endless Level Complete) Your score: " + tostr(score) + (data.highScore < score ? wrap(" New Record!", YELLOW) : ""));
         playSound("click");
         clearln(4);
         level.clear();
      }
      else if (result == endState::quit && getConsent("Do you want to save your progress? [y/n] > ")) 
      {
         save.deleteEndlessProgress(data.id);

         const bool newRecord {data.highScore < score};
         if (newRecord)
            std::thread(&Save::updateHighScore, &save, std::ref(data), score, data.id).detach();

         std::thread(&Save::saveEndlessProgress, &save, mapToString(level.getMap()), score, data.id).detach();
         playSound("success");
         println("Successfuly saved endless progress.", GREEN);
         return;
      }
      else 
      {
         playSound("died");

         const bool newRecord {data.highScore < score};
         if (newRecord)
            std::thread(&Save::updateHighScore, &save, std::ref(data), score, data.id).detach();
         
         getHiddenInput("\n(Endless Level Failed) Your score: " + tostr(score) + (newRecord ? wrap(" New Record!", YELLOW) : ""));
         playSound("click");
         return;
      }
   }
}