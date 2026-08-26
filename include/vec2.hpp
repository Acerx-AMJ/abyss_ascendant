#ifndef AA_VEC2_HPP
#define AA_VEC2_HPP

#include "syntax.hpp"

// Vector2 struct containing two int values.
struct Vec2
{
   int16 x = 0;
   int16 y = 0;

   // Constructors.
   constexpr Vec2()
      : x(0), y(0) {}
   
   constexpr Vec2(int16 size)
      : x(size), y(size) {}

   constexpr Vec2(int16 x, int16 y)
      : x(x), y(y) {}
   
   // Add to X value.
   constexpr Vec2 addX(int16 xValue) const
   {
      return Vec2(x + xValue, y);
   }

   // Add to Y value.
   constexpr Vec2 addY(int16 yValue) const
   {
      return Vec2(x, y + yValue);
   }

   // Return the distance between this vector and another one.
   constexpr int32 distanceTo(const Vec2& other) const
   {
      return std::max(other.x - x, x - other.x) + std::max(other.y - y, y - other.y);
   }

   // Check if the vec2 is withing the specified area.
   constexpr bool within(const Vec2& point1, const Vec2& point2) const
   {
      return x >= std::min(point1.x, point2.x) && x <= std::max(point1.x, point2.x)
          && y >= std::min(point1.y, point2.y) && y <= std::max(point1.y, point2.y);
   }

   // Compare if two vec2 are identical.
   constexpr bool operator==(const Vec2& other) const 
   {
      return x == other.x && y == other.y;
   }

   // Compare if two vec2 are not identical.
   constexpr bool operator!=(const Vec2& other) const 
   {
      return x != other.x || y != other.y;
   }

   // Add two vec2 structs together.
   constexpr Vec2 operator+(const Vec2& other) const 
   {
      return Vec2(x + other.x, y + other.y);
   }

   // Subtract two vectors
   constexpr Vec2 operator-(const Vec2& other) const
   {
      return Vec2(x - other.x, y - other.y);
   }

   // Multiply a vector by a factor
   constexpr Vec2 operator*(int16 factor) const
   {
      return Vec2(x * factor, y * factor);
   }

   // Divide a vector by a factor
   constexpr Vec2 operator/(int16 factor) const
   {
      return Vec2(x / factor, y / factor);
   }

   // Negate a vec2.
   constexpr Vec2 operator-() const 
   {
      return Vec2(-x, -y);
   }

   static const Vec2 up, down, right, left, invalid;
};

// Vec2 constants.
inline const Vec2 Vec2::up      = Vec2(0, -1);
inline const Vec2 Vec2::down    = Vec2(0, 1);
inline const Vec2 Vec2::left    = Vec2(-1, 0);
inline const Vec2 Vec2::right   = Vec2(1, 0);
inline const Vec2 Vec2::invalid = Vec2(-1);

#endif // AA_VEC2_HPP
