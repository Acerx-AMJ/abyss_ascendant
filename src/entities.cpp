#include "entities.hpp"
#include "io.hpp"
#include "level.hpp"
#include <cstdlib>

// Create a brand new player class.
Player::Player(Level& level, const Vec2& pos)
   : level(level), pos(pos)
{
   level.setTile(pos, tileType::player);
}

void Player::move(const Vec2& dir)
{
   const Vec2 target = pos + dir;

   // Player tried to move outside of the map.
   if (!level.withinBounds(target)) 
   {
      println("Cannot move outside of the map."s, RED);
      return;
   }

   // Player has died.
   if (level.getTile(pos) > tileType::player) 
      return;

   const tileType tile = level.getTile(target);

   // Handle action based on target tile.
   if ((tile < tileType::hBat && tile > tileType::player) || tile == tileType::death) 
   {
      println("Cannot move as there's an obstacle in the way"s, RED);
      return;
   }
   else if (tile == tileType::coin) 
   {
      level.incrementCoins();
   }
   else if (tile > tileType::obstacle) 
   {
      level.moveTile(pos, target, tileType::empty);
      level.setTile(target, tileType::death);
      pos = target;
      return;
   }

   // Update position.
   level.moveTile(pos, target, tileType::empty);
   pos = target;
   println("Moved successfully."s, GREEN);
}

// Check if the player is alive.
bool Player::isAlive() const
{
   const bool alive {level.getTile(pos) == tileType::player};

   if (!alive) 
      level.setTile(pos, tileType::death);
   
   return alive; 
}

// Return the player's position
const Vec2& Player::getPos() const
{
   return pos;
}

// Create a new enemy.
Enemy::Enemy(Level& level, const Vec2& pos, tileType type)
   : level(level), pos(pos), type(type)
{
   level.setTile(pos, type);
}

// Get a random direction.
Vec2 Enemy::randomDir()
{
   return Vec2(rand() % 3 - 1, rand() % 3 - 1);
}

// Swap the tiles and update the position.
void Enemy::moveTo(const Vec2& target)
{
   level.setTile(pos, tile);
   tile = level.getTile(target);

   level.setTile(target, type);
   pos = target;
}

// Check if the target is in bounds and not on another enemy
bool Enemy::canMove(const Vec2& target) const
{
   return level.getTile(target) < tileType::nyx && level.withinBounds(target);
}

// Move the given enemy based on its move pattern.
void VBat::move(const Vec2&)
{
   Vec2 target {pos.addY(dir)};

   // Change the direction if the bat was unable to move.
   if (!canMove(target)) {
      dir = -dir;
      target = pos.addY(dir);

      if (!canMove(target)) return;
   }

   moveTo(target);
}

void HBat::move(const Vec2&)
{
   Vec2 target {pos.addX(dir)};

   // Change the direction if the bat was unable to move.
   if (!canMove(target)) {
      dir = -dir;
      target = pos.addX(dir);

      if (!canMove(target)) return;
   }

   moveTo(target);
}

void Glimmer::move(const Vec2&)
{
   const Vec2 originalDir {dir};
   Vec2 target {pos + dir};

   // Change the direction if Glimmer was unable to move.
   for (uint8 i {}; i < 10 && (!canMove(target) || abs(dir.x + dir.y) != 1); i++) 
   {
      dir = randomDir();
      target = pos + dir;
   }

   // Move the Glimmer if it didn't change the direction.
   if (dir == originalDir && canMove(target) && abs(dir.x + dir.y) == 1)
      moveTo(target);
}

void Seeker::move(const Vec2& player)
{
   if (++turn % 2 != 0) return;
   Vec2 dir;

   // Calculate the direction in which the seeker should move.
   if (player.x > pos.x)
      dir.x = 1;
   else if (player.x < pos.x) 
      dir.x = -1;

   if (player.y > pos.y) 
      dir.y = 1;
   else if (player.y < pos.y)
      dir.y = -1;

   Vec2 target {pos + dir};

   // Handle collision with obstacles.
   if (!canMove(target) && dir.x != 0 && canMove(pos.addX(dir.x))) 
      target.y = pos.y;
   else if (!canMove(target) && dir.y != 0 && canMove(pos.addY(dir.y))) 
      target.x = pos.x;
   else if (!canMove(target)) 
      return;

   moveTo(target);
}

void Chaser::move(const Vec2& player)
{
   if (++turn % 2 == 0) return;
   Vec2 dir;

   // Handle horizontal movement first and handle vertical after.
   if (player.x > pos.x) 
      dir.x = 1;
   else if (player.x < pos.x) 
      dir.x = -1;

   Vec2 target {pos + dir};

   // Move horizontally, swapping tiles and updating the position.
   if (dir.x != 0 && canMove(target)) 
   {
      moveTo(target);
      return;
   }

   // Handle vertical movement if horizontal movement could not be handled.
   if (player.y > pos.y) 
      dir.y = 1;
   else if (player.y < pos.y) 
      dir.y = -1;

   target = pos.addY(dir.y);

   // Move vertically, swapping tiles and updating the position.
   if (dir.y != 0 && canMove(target)) 
   {
      moveTo(target);
      return;
   }
}

void Lurker::move(const Vec2& player)
{
   if (++turn % 2 == 0) return;
   Vec2 target {pos};

   // If the player is close enough, move on them.
   if (pos.distanceTo(player) <= 1) 
   {
      moveTo(player);
      return;
   }

   uint8 count {};

   // Find a valid, random position to move to. The Lurker can only attack if the
   // player is exactly a tile away.
   while (target == pos || !canMove(target) || level.getTile(target) == tileType::player) 
   {
      target = randomDir() + pos;
      if (++count > 10) return;
   }

   moveTo(target);
}

void Wanderer::move(const Vec2&)
{
   Vec2 target {pos};
   uint8 count {};

   // Generate a valid, random position that is not the current position.
   while (target == pos || !canMove(target)) 
   {
      target = randomDir() + pos;
      if (++count > 10) return; 
   }

   moveTo(target);
}

void Observer::move(const Vec2& player)
{
   if (pos.distanceTo(player) >= 3) return;
   Vec2 target {pos};

   // Move based on the player's position or straight on top of it.
   if (pos.distanceTo(player) == 1) 
      target = player;
   else if (pos.x > player.x && canMove(pos.addX(-1))) 
      target = pos.addX(-1);
   else if (pos.x < player.x && canMove(pos.addX(1))) 
      target = pos.addX(1);
   else if (pos.y > player.y && canMove(pos.addY(-1))) 
      target = pos.addY(-1);
   else if (pos.y < player.y && canMove(pos.addY(1))) 
      target = pos.addY(1);

   // Swap tiles and update position.
   if (target != pos) 
      moveTo(target);
}

void Hunter::move(const Vec2 &player)
{
   if (++turn % 4 == 0) return;
   Vec2 dir;

   // Calculate the direction in which the seeker should move.
   if (player.x > pos.x)
      dir.x = 1;
   else if (player.x < pos.x) 
      dir.x = -1;

   if (player.y > pos.y) 
      dir.y = 1;
   else if (player.y < pos.y)
      dir.y = -1;

   Vec2 target {pos + dir};

   // Handle collision with obstacles.
   if (!canMove(target) && dir.x != 0 && canMove(pos.addX(dir.x))) 
      target.y = pos.y;
   else if (!canMove(target) && dir.y != 0 && canMove(pos.addY(dir.y))) 
      target.x = pos.x;
   else if (!canMove(target)) 
      return;

   moveTo(target);
}
