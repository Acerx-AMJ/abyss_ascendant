#ifndef AA_ENTITIES_HPP
#define AA_ENTITIES_HPP

#include "tiles.hpp"
#include "vec2.hpp"

struct Level;

// Player class for controlling the character.
class Player
{
public:
   Player(Level& level, Vec2 pos);

   // Move the player in the given direction if possible.
   void move(Vec2 dir);

   // Check if the player is alive.
   bool isAlive() const;

   // Return the player's position.
   Vec2 getPos() const;

private:
   Level& level;
   Vec2 pos;
};

// Enemy class used to create enemies.
class Enemy
{
public:
   Enemy(Level& level, Vec2 pos, tileType type);

   virtual ~Enemy() = default;
   virtual void move(Vec2 player) = 0;

protected:
   Level& level;
   Vec2 pos;
   tileType type, tile {tileType::empty};

   // Returns a random direction from (-1, -1) to (1, 1);
   Vec2 randomDir();

   // Swaps the tiles and sets the position to the new one.
   void moveTo(Vec2 target);
   bool canMove(Vec2 target) const;
};

// Vertical bat moves vertically, changing the direction on collision.
class VBat : public Enemy
{
public:
   VBat(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::vBat) {}
   
   void move(Vec2) override;

private:
   int dir {1};
};

// Horizontal bat moves horizontally, changing the direction on collision.
class HBat : public Enemy
{
public:
   HBat(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::hBat) {}
   
   void move(Vec2) override;

private:
   int dir {1};
};

// Glimmer moves straight in a random direction on collision.
class Glimmer : public Enemy{
public:
   Glimmer(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::glimmer) {}

   void move(Vec2) override;

private:
   Vec2 dir {Vec2::down};
};

// Seeker slowly chases the player.
class Seeker : public Enemy{
public:
   Seeker(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::seeker) {}
   
   void move(Vec2 player) override;

private:
   int turn {};
};

// Chaser chases the player, but cannot move diagonally.
class Chaser : public Enemy
{
public:
   Chaser(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::chaser) {}
   
   void move(Vec2 player) override;

private:
   int turn {};
};

// Lurker wanders around randomly and attacks the player if they're close.
class Lurker : public Enemy
{
public:
   Lurker(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::lurker) {}
   
   void move(Vec2 player) override;

private:
   int turn {};
};

// Wanderer wanders randomly around the map.
class Wanderer : public Enemy
{
public:
   Wanderer(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::wanderer) {}
   
   void move(Vec2) override;
};

// Observer only moves if the player is in range.
class Observer : public Enemy
{
public:
   Observer(Level& level, Vec2 pos)
      : Enemy(level, pos, tileType::observer) {}
   
   void move(Vec2 player) override;
};

// Hunter is a more aggressive version of seeker
class Hunter : public Enemy
{
public:
   Hunter(Level &level, Vec2 pos)
      : Enemy(level, pos, tileType::hunter) {}
   
   void move(Vec2 player) override;

private:
   int turn {};
};

#endif // AA_ENTITIES_HPP
