#ifndef AA_ALGORITHMS_HPP
#define AA_ALGORITHMS_HPP

#include "vec2.hpp"
#include <vector>

// Struct containing the position, H-score and G-score.
struct Star
{
   Vec2 pos;
   int gScore, hScore;

   Star(Vec2 pos, int gScore, int hScore);

   // Return the F-Score of the star.
   int fScore() const;
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
   Room(Vec2 pos, Vec2 size);
};

// Node struct is used to split the rooms using BSP algorithm.
struct Node 
{
   Vec2 pos, size;
   Room* r;
   Node* left;
   Node* right;

   Node(Vec2 pos, Vec2 size);
   
   // Check if the node has no neighbors.
   bool isLeaf() const;
};

// BSP class for creating random room layouts using the BSP Algorithm.
class Bsp 
{
public:
   Bsp(Vec2 size);

   // Generate a new map with random rooms.
   std::vector<std::vector<int>>& generate(int roomSize);
   void splitSpace(Node* root, int roomSize);
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
   std::vector<std::vector<int>> map;
};

// Calculate the scores for the A* pathfinding algorithm.
int calculateHScore(Vec2 current, Vec2 goal);
int calculateGScore(Vec2 current, Vec2 start);

// See if two points are reachable using the A* pathfinding algorithm.
bool aStar(Vec2 start, Vec2 end, const std::vector<std::vector<int>>& map, Vec2 size);

#endif // AA_ALGORITHMS_HPP
