#include "algorithms.hpp"
#include "tiles.hpp"
#include <cstdlib>
#include <queue>
#include <array>

// Define four directions for use in the path finder.
const std::array<Vec2, 4> directions {{Vec2::up, Vec2::down, Vec2::left, Vec2::right}};

// Create a new star.
Star::Star(Vec2 pos, int gScore, int hScore)
   : pos(pos), gScore(gScore), hScore(hScore) {}

// Return the F-Score of the star.
int Star::fScore() const
{
   return gScore + hScore;
}

// Find out which star has the lower F-Score.
bool StarCondition::operator()(const Star& first, const Star& second) const
{
   return first.fScore() > second.fScore();
}

// Create a new room.
Room::Room(Vec2 pos, Vec2 size)
   : pos(pos), size(size) {}

// Create a new node.
Node::Node(Vec2 pos, Vec2 size)
   : pos(pos), size(size), r(nullptr), left(nullptr), right(nullptr) {}

// Check if the node has no neighbors.
bool Node::isLeaf() const
{
   return !left && !right;
}

// Create a new BSP class.
Bsp::Bsp(Vec2 size)
   : size(size)
{
   map = std::vector<std::vector<int>>(size.y, std::vector<int>(size.x, int(tileType::empty)));
}

// Generate a new map with random rooms.
std::vector<std::vector<int>>& Bsp::generate(int roomSize)
{
   Node* root = new Node(Vec2(), size);
   splitSpace(root, roomSize);
   createRooms(root);
   connectRooms(root);
   deleteTree(root);
   return map;
}

// Recursively split the map into smaller rooms.
void Bsp::splitSpace(Node* root, int roomSize)
{
   if (root->size.x <= roomSize * 2 && root->size.y <= roomSize * 2) return;
   bool splitV {root->size.x > root->size.y};
   
   if (root->size.x == root->size.y) 
      splitV = rand() % 2;

   const int split = splitV
      ? rand() % (root->size.x - roomSize * 2) + roomSize
      : rand() % (root->size.y - roomSize * 2) + roomSize;

   if (splitV)
   {
      root->left = new Node(root->pos, Vec2(split, root->size.y));
      root->right = new Node(Vec2(root->pos.x + split, root->pos.y), Vec2(root->size.x - split, root->size.y));
   }
   else 
   {
      root->left = new Node(root->pos, Vec2(root->size.x, split));
      root->right = new Node(Vec2(root->pos.x, root->pos.y + split), Vec2(root->size.x, root->size.y - split));
   }

   splitSpace(root->left, roomSize);
   splitSpace(root->right, roomSize);
}

// Create a room withing the space of a leaf room.
void Bsp::createRooms(Node* root)
{
   if (!root->isLeaf()) 
   {
      if (root->left) createRooms(root->left);
      if (root->right) createRooms(root->right);
      return;
   }

   const int w = rand() % (std::max(1, root->size.x - 4)) + 4;
   const int h = rand() % (std::max(1, root->size.y - 4)) + 4;
   const int x = rand() % (std::max(1, root->size.x - w));
   const int y = rand() % (std::max(1, root->size.y - h));
   root->r = new Room(Vec2(root->pos.x + x, root->pos.y + y), Vec2(w, h));

   // Place the walls on the map.
   for (int y {root->r->pos.y}; y < root->r->pos.y + root->r->size.y; y++)
      for (int x {root->r->pos.x}; x < root->r->pos.x + root->r->size.x; x++)
         map.at(y).at(x) = int((y == root->r->pos.y || y == root->r->pos.y + root->r->size.y - 1
            || x == root->r->pos.x || x == root->r->pos.x + root->r->size.x - 1) ? tileType::obstacle : tileType::empty);
}

// Connect all of the room together.
void Bsp::connectRooms(Node* root)
{
   if (root->isLeaf()) return;

   connectRooms(root->left);
   connectRooms(root->right);

   Room* lRoom {findRoom(root->left)};
   Room* rRoom {findRoom(root->right)};

   if (!lRoom || !rRoom) return;

   Vec2 point1 {getRandomPos(lRoom)};
   Vec2 point2 {getRandomPos(rRoom)};

   while (point1.x != point2.x) 
   {
      map.at(point1.y).at(point1.x) = int(tileType::empty);
      point1.x += (point2.x > point1.x) ? 1 : -1;
   }

   while (point1.y != point2.y) 
   {
      map.at(point1.y).at(point1.x) = int(tileType::empty);
      point1.y += (point2.y > point1.y) ? 1 : -1;
   }
}

// Find a room from a node.
Room* Bsp::findRoom(Node* root)
{
   if (root->isLeaf())
      return root->r;
   else if (rand() % 2 && root->left)
      return findRoom(root->left);
   else if (root->right)
      return findRoom(root->right);
   
   return nullptr;
}

// Get a random point in a room.
Vec2 Bsp::getRandomPos(Room* r)
{
   return Vec2(r->pos.x + rand() % (r->size.x - 2) + 1, r->pos.y + rand() % (r->size.y - 2) + 1);
}

// Clean up and delete all of the nodes.
void Bsp::deleteTree(Node* root)
{
   if (!root) return;
   deleteTree(root->left);
   deleteTree(root->right);
   delete root->r;
   delete root;
}

// Calculate the H-Score for the A* pathfinding algorithm.
int calculateHScore(Vec2 current, Vec2 goal)
{
   return abs(current.x - goal.x) + abs(current.y - goal.y);
}

// Calculate the G-Score for the A* pathfinding algorithm.
int calculateGScore(Vec2 current, Vec2 start)
{
   return abs(current.x - start.x) + abs(current.y - start.y);
}

// See if two points are reachable using the A* pathfinding algorithm.
bool aStar(Vec2 start, Vec2 end, const std::vector<std::vector<int>>& map, Vec2 size)
{
   std::priority_queue<Star, std::vector<Star>, StarCondition> stars;
   std::vector<std::vector<bool>> evaluated (size.y, std::vector<bool>(size.x, false));

   // Push the start to the stars queue.
   stars.push(Star(start, 0, calculateHScore(start, end)));

   while (!stars.empty())
   {
      Star current {stars.top()};
      stars.pop();

      // Skip evaluated nodes.
      if (evaluated.at(current.pos.y).at(current.pos.x))
         continue;

      // Look in all 4 directions.
      for (int i {}; i < 4; i++)
      {
         const Vec2 pos {current.pos + directions.at(i)};

         // The goal has been successfully reached.
         if (pos == end)
            return true;

         if (pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y && (map.at(pos.y).at(pos.x) == int(tileType::empty) || map.at(pos.y).at(pos.x) == int(tileType::coin)) && !evaluated.at(pos.y).at(pos.x))
            stars.push(Star(pos, calculateGScore(pos, start), calculateHScore(pos, end)));
      }

      evaluated.at(current.pos.y).at(current.pos.x) = true;
   }
   return false;
}
