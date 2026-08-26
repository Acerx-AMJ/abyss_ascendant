#ifndef AA_ENTITIES_HPP
#define AA_ENTITIES_HPP

#include "tiles.hpp"

// Player class for controlling the character.
class Player
{
public:
   Player(LEVEL& level, const Vec2& pos);

   // Move the player in the given direction if possible.
   void move(const Vec2& dir);

   // Check if the player is alive.
   bool isAlive() const;

   // Return the player's position.
   const Vec2& getPos() const;

private:
   LEVEL& level;
   Vec2 pos;
};

// Enemy class used to create enemies.
class Enemy
{
public:
   Enemy(LEVEL& level, const Vec2& pos, tileType type);

   virtual ~Enemy() = default;
   virtual void move(const Vec2& player) = 0;

protected:
   LEVEL& level;
   Vec2 pos;
   tileType type, tile {tileType::empty};

   // Returns a random direction from (-1, -1) to (1, 1);
   Vec2 randomDir();

   // Swaps the tiles and sets the position to the new one.
   void moveTo(const Vec2& target);
   bool canMove(const Vec2& target) const;
};

// Vertical bat moves vertically, changing the direction on collision.
class VBat : public Enemy
{
public:
   VBat(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::vBat) {}
   
   void move(const Vec2&) override;

private:
   uint8 dir {1};
};

// Horizontal bat moves horizontally, changing the direction on collision.
class HBat : public Enemy
{
public:
   HBat(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::hBat) {}
   
   void move(const Vec2&) override;

private:
   uint8 dir {1};
};

// Glimmer moves straight in a random direction on collision.
class Glimmer : public Enemy{
public:
   Glimmer(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::glimmer) {}

   void move(const Vec2&) override;

private:
   Vec2 dir {Vec2::down};
};

// Seeker slowly chases the player.
class Seeker : public Enemy{
public:
   Seeker(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::seeker) {}
   
   void move(const Vec2& player) override;

private:
   uint8 turn {};
};

// Chaser chases the player, but cannot move diagonally.
class Chaser : public Enemy
{
public:
   Chaser(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::chaser) {}
   
   void move(const Vec2& player) override;

private:
   uint8 turn {};
};

// Lurker wanders around randomly and attacks the player if they're close.
class Lurker : public Enemy
{
public:
   Lurker(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::lurker) {}
   
   void move(const Vec2& player) override;

private:
   uint8 turn {};
};

// Wanderer wanders randomly around the map.
class Wanderer : public Enemy
{
public:
   Wanderer(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::wanderer) {}
   
   void move(const Vec2&) override;
};

// Observer only moves if the player is in range.
class Observer : public Enemy
{
public:
   Observer(LEVEL& level, const Vec2& pos)
      : Enemy(level, pos, tileType::observer) {}
   
   void move(const Vec2& player) override;
};

#endif // AA_ENTITIES_HPP
