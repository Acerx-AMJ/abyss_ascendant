#ifndef AA_SAVE_HPP
#define AA_SAVE_HPP

#include "syntax.hpp"
#include <sqlite3.h>

// Constants for the save class.
inline constexpr uint16 MAX_USER_LENGTH {24};
inline constexpr uint16 MIN_USER_LENGTH {3};
inline constexpr uint16 MAX_ICON_LENGTH {2};
inline constexpr uint16 MAX_ACCOUNTS {10};

inline constexpr uint16 ENDLESS_UNLOCK {9};
inline constexpr uint16 DAILY_UNLOCK {50};
inline constexpr uint16 COLOR_UNLOCK {100};
inline constexpr uint16 ICON_UNLOCK {150};

inline constexpr uint16 INVALID_ID {MAX_ACCOUNTS + 1};

// Struct for saving and loading data.
struct PlayerData 
{
   str name, icon, color;
   uint64 highScore;
   uint16 lastClear;
   uint16 id = INVALID_ID;

   PlayerData() = default;
   PlayerData(vstr name, vstr icon, vstr color, uint64 highScore, uint16 lastClear, uint16 id);
};

// Class for saving, loading and editing player data.
class Save 
{
public:
   Save();
   ~Save();

   // Create/destroy a save file.
   void create(PlayerData& data);
   void destroy(uint16 id);

   // Update a save file.
   void update(PlayerData& data, uint16 id);
   void updateLevels(PlayerData& data, uint16 id);
   void updateHighScore(PlayerData& data, uint64 newScore, uint16 id);

   // Update the main account ID or retrieve it.
   uint16 retrieveMainAccount();
   void setMainAccount(uint16 id);

   // Get all of the valid IDs that could be used.
   v_uint getValidIds();

   // Select save file by id.
   PlayerData select(uint16 id);
   PlayerData selectByName(const str& name);

   // Work with saving, retrieving or destroying endless save files.
   void saveEndlessProgress(const str& layout, uint64 score, uint16 id);
   std::pair<str, uint64> retrieveEndlessProgress(uint16 id);
   void deleteEndlessProgress(uint16 id); 

private:
   sqlite3* db;
   char* errorMsg;

   // Handle SQL injection errors.
   bool handleInsertion(int32 status, const str& error);
};

#endif // AA_SAVE_HPP
