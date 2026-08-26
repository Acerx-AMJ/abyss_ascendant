#ifndef AA_ALGORITHMS_HPP
#define AA_ALGORITHMS_HPP

#include "vec2.hpp"

// Struct containing the position, H-score and G-score.
struct Star
{
   Vec2 pos;
   uint16 gScore, hScore;

   Star(const Vec2& pos, uint16 gScore, uint16 hScore);

   // Return the F-Score of the star.
   uint16 fScore() const;
};

// Struct for checking if the F-Score is lower than a different star.
struct StarCondition
{
   bool operator()(const Star& first, const Star& second) const;
};

// Room struct is used in nodes.
struct Room
{
   Vec2 pos, size;
   Room(const Vec2& pos, const Vec2& size);
};

// Node struct is used to split the rooms using BSP algorithm.
struct Node 
{
   Vec2 pos, size;
   Room* r;
   Node* left;
   Node* right;

   Node(const Vec2& pos, const Vec2& size);
   
   // Check if the node has no neighbors.
   bool isLeaf() const;
};

// BSP class for creating random room layouts using the BSP Algorithm.
class Bsp 
{
public:
   Bsp(const Vec2& size);

   // Generate a new map with random rooms.
   m_uint& generate(uint16 roomSize);
   void splitSpace(Node* root, uint16 roomSize);
   void createRooms(Node* root);
   void connectRooms(Node* root);

   // Find a room from a node.
   Room* findRoom(Node* root);

   // Get a random point in a room.
   Vec2 getRandomPos(Room* r);

   // Clean up and delete all of the nodes.
   void deleteTree(Node* root);

private:
   Vec2 size;
   m_uint map;
};

// Calculate the scores for the A* pathfinding algorithm.
uint16 calculateHScore(const Vec2& current, const Vec2& goal);
uint16 calculateGScore(const Vec2& current, const Vec2& start);

// See if two points are reachable using the A* pathfinding algorithm.
bool aStar(const Vec2& start, const Vec2& end, const m_uint& map, const Vec2& size);

#endif // AA_ALGORITHMS_HPP
