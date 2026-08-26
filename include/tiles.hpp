#ifndef AA_TILES_HPP
#define AA_TILES_HPP

#include "vec2.hpp"

// Define tile types for levels.
enum class tileType : uint8 
{
  empty,
  coin,
  player,
  nyx,
  jake,
  gabriel,
  bricks,
  obstacle,
  hBat,
  vBat,
  glimmer,
  seeker,
  lurker,
  wanderer,
  chaser,
  observer,
  mine,
  death
};

// Forward declaration.
class LEVEL 
{
public:
  virtual tileType getTile(const Vec2& pos) const = 0;
  virtual void setTile(const Vec2& pos, tileType tile) = 0;
  virtual tileType moveTile(const Vec2& pos, const Vec2& target, tileType type) = 0;
  virtual bool withinBounds(const Vec2& pos) const = 0;
  virtual void incrementCoins() = 0;
};

#endif // AA_TILES_HPP
