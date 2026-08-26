#include "infinity.hpp"
#include "io.hpp"
#include "audio.hpp"
#include "algorithms.hpp"
#include <sstream>
#include <thread>

// Spawn rates for the endless mode.
const std::vector<std::pair<uint16, uint8>> spawnRates {{
   {uint16(tileType::mine), 50}, {uint16(tileType::hBat), 17}, {uint16(tileType::vBat), 17}, {uint16(tileType::glimmer), 11}, {uint16(tileType::observer), 2}, {uint16(tileType::lurker), 2}, {uint16(tileType::chaser), 1}
}};

// Convert a map to string for saving purposes.
str mapToString(const m_uint& map)
{
   std::stringstream stream;

   for (const auto& row : map)
   {
      for (const auto& tile : row)
      {
         switch (static_cast<tileType>(tile)) 
         {
         case tileType::empty:    stream << "E"s; break;
         case tileType::coin:     stream << "C"s; break;
         case tileType::player:   stream << "P"s; break;
         case tileType::obstacle: stream << "O"s; break;
         case tileType::vBat:     stream << "V"s; break;
         case tileType::hBat:     stream << "H"s; break;
         case tileType::glimmer:  stream << "G"s; break;
         case tileType::seeker:   stream << "S"s; break;
         case tileType::lurker:   stream << "L"s; break;
         case tileType::wanderer: stream << "W"s; break;
         case tileType::chaser:   stream << "R"s; break;
         case tileType::observer: stream << "0"s; break;
         case tileType::mine:     stream << "M"s; break;
         default:                 stream << "?"s; break;
         }
      }
      stream << "#"s;
   }
   return stream.str();
}

// Convert a string to a map for loading purposes.
m_uint stringToMap(vstr map)
{
   m_uint result;
   v_uint row;

   for (const auto& tile : map) 
   {
      switch (tile)
      {
      case 'E': row.push_back(uint16(tileType::empty)); break;
      case 'C': row.push_back(uint16(tileType::coin)); break;
      case 'P': row.push_back(uint16(tileType::player)); break;
      case 'O': row.push_back(uint16(tileType::obstacle)); break;
      case 'V': row.push_back(uint16(tileType::vBat)); break;
      case 'H': row.push_back(uint16(tileType::hBat)); break;
      case 'G': row.push_back(uint16(tileType::glimmer)); break;
      case 'S': row.push_back(uint16(tileType::seeker)); break;
      case 'L': row.push_back(uint16(tileType::lurker)); break;
      case 'W': row.push_back(uint16(tileType::wanderer)); break;
      case 'R': row.push_back(uint16(tileType::chaser)); break;
      case '0': row.push_back(uint16(tileType::observer)); break;
      case 'M': row.push_back(uint16(tileType::mine)); break;
      case '#': result.push_back(row); row.clear(); break;
      default:  row.push_back(-1); break; // Purposefuly overflow the unsigned integer
      }
   }
   return result;
}

// Randomly choose an enemy based on score.
uint16 getRandomEnemy(uint64 score, const std::vector<std::pair<uint16, uint8>>& rates)
{
   uint16 total {};
   v_uint weights;

   // Calculate weights.
   for (const auto& enemy : rates) 
   {
      const uint16 weight = enemy.second + score / 4;
      weights.push_back(weight);
      total += weight;
   }

   const uint16 value = rand() % total;
   uint16 cumulative {};

   // Get a random value and return an enemy based on it.
   for (uint8 i {}; i < rates.size(); i++) 
   {
      cumulative += weights.at(i);

      if (value < cumulative)
         return rates.at(i).first;
   }
   return rates.back().first;
}

// Place coins and enemies around the map.
void dressLevel(m_uint& map, const Vec2& size, const Vec2& player, uint16 enemies, uint16 coins, const std::vector<std::pair<uint16, uint8>>& rates, uint64 score)
{
   // Add enemies.
   for (uint16 i {}; i < enemies; i++) {
      Vec2 target(rand() % size.x, rand() % size.y);
      uint8 count {};

      while (map.at(target.y).at(target.x) != uint8(tileType::coin) || player.distanceTo(target) < 4)
      {
         target = Vec2(rand() % size.x, rand() % size.y);
         if (++count > 10) break;
      }

      if (count <= 10)
         map.at(target.y).at(target.x) = getRandomEnemy(score, rates);
   }

   // Fill the map with coins.
   for (uint16 i {}; i < coins; i++) {
      Vec2 target(rand() % size.x, rand() % size.y);
      uint8 count {};

      while (map.at(target.y).at(target.x) != uint8(tileType::empty) || !aStar(player, target, map, size))
      {
         target = Vec2(rand() % size.x, rand() % size.y);
         if (++count > 10) break;
      }

      if (count <= 10)
         map.at(target.y).at(target.x) = uint8(tileType::coin);
   }
}

// Generate a new level.
Level generateEndless(PlayerData& data, uint64 score, bool pickedSave, vstr stringMap)
{
   if (pickedSave) 
   {
      auto map {stringToMap(stringMap)};
      return Level(""s, Vec2(map.at(0).size(), map.size()), 0, map, story.getStory(score), {}, {}, {{uint16(goal::getCoins)}}, data);
   }

   srand(time(0));
   Vec2 size(rand() % std::min(8ul, (score / 3) + 1) + 24, rand() % std::min(4ul, (score / 6) + 1) + 12);

   // Generate a new map.
   Bsp bsp(size);
   m_uint map {bsp.generate(9 - std::min(6ul, score / 6))};

   Vec2 playerPos(rand() % size.x, rand() % size.y);

   // Find a good place to put the player in.
   while (map.at(playerPos.y).at(playerPos.x) != uint8(tileType::empty))
      playerPos = Vec2(rand() % size.x, rand() % size.y);

   const uint16 enemyCount = rand() % std::min(7ul, (score / 4) + 1) + score / 60;
   const uint16 coinCount = rand() % std::min(12ul, (score / 4) + 1) + 3;

   // Add the player, enemies and coins to the level.
   dressLevel(map, size, playerPos, enemyCount, coinCount, spawnRates, score);
   map.at(playerPos.y).at(playerPos.x) = uint8(tileType::player);

   return Level(""s, size, 0, map, story.getStory(score), {}, {}, {{uint16(goal::getCoins)}}, data);
}

// Generate the daily level for today.
Level generateDaily(PlayerData& data)
{
   time_t now {time(0)};
   tm* local {localtime(&now)};

   // Calculate the seed based on the current date.
   const int32 seed {(local->tm_year + 1900) * 10000 + (local->tm_mon + 1) * 100 + local->tm_mday};
   srand(seed);

   // Get the layout index, the layout and the spawn rates.
   const uint16 index = rand() % daily.getDailyLayoutSize() + 1;
   auto layout {daily.getDailyLayout(index)};
   auto rates {daily.getDailyRates(index)};

   Vec2 playerPos(rand() % layout.second.x, rand() % layout.second.y);

   // Find a good place to put the player in.
   while (layout.first.at(playerPos.y).at(playerPos.x) != uint8(tileType::empty))
      playerPos = Vec2(rand() % layout.second.x, rand() % layout.second.y);

   const uint16 enemyCount = rand() % 3 + rates.second;
   const uint16 coinCount = rand() % 10 + 5;

   // Place the player, enemies and coins around the level.
   dressLevel(layout.first, layout.second, playerPos, enemyCount, coinCount, rates.first);
   layout.first.at(playerPos.y).at(playerPos.x) = uint8(tileType::player);

   return Level(""s, layout.second, 0, layout.first, {{"SKIP"s}}, {{"SKIP"s}}, {{"SKIP"s}}, {{uint16(goal::getCoins)}}, data);
}

// Play endless until the player dies or quits.
void endless(Save& save, PlayerData& data)
{
   auto progress {save.retrieveEndlessProgress(data.id)};
   bool pickedSave {progress.second && getConsent("Do you want to load your endless save file? ("s + wrap(tostr(progress.second), YELLOW) + " score) [y/n] > "s)};
   uint64 score {(pickedSave ? progress.second : 0ul)};

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
         playSound("horn"s);
      
      println("\nYour current score: "s + tostr(score) + (data.highScore < score ? wrap(" New Record!"s, YELLOW) : ""s) + "\n"s);
  
      // Play the level and return the result.
      endState result {playLevel(level)};

      if (result == endState::won) 
      {
         score++;
         getHiddenInput("\n(Endless Level Complete) Your score: "s + tostr(score) + (data.highScore < score ? wrap(" New Record!"s, YELLOW) : ""s));
         playSound("click"s);
         clearln(4);
         level.clear();
      }
      else if (result == endState::quit && getConsent("Do you want to save your progress? [y/n] > "s)) 
      {
         save.deleteEndlessProgress(data.id);

         const bool newRecord {data.highScore < score};
         if (newRecord)
            std::thread(&Save::updateHighScore, &save, std::ref(data), score, data.id).detach();

         std::thread(&Save::saveEndlessProgress, &save, mapToString(level.getMap()), score, data.id).detach();
         playSound("success"s);
         println("Successfuly saved endless progress."s, GREEN);
         return;
      }
      else 
      {
         playSound("died"s);

         const bool newRecord {data.highScore < score};
         if (newRecord)
            std::thread(&Save::updateHighScore, &save, std::ref(data), score, data.id).detach();
         
         getHiddenInput("\n(Endless Level Failed) Your score: "s + tostr(score) + (newRecord ? wrap(" New Record!"s, YELLOW) : ""s));
         playSound("click"s);
         return;
      }
   }
}