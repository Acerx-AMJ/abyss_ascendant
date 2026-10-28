#ifndef AA_SAVE_HPP
#define AA_SAVE_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

// Constants for the save class.
inline constexpr int MAX_USER_LENGTH {24};
inline constexpr int MIN_USER_LENGTH {3};
inline constexpr int MAX_ICON_LENGTH {2};
inline constexpr int MAX_ACCOUNTS {10};

inline constexpr int ENDLESS_UNLOCK {9};
inline constexpr int DAILY_UNLOCK {50};
inline constexpr int COLOR_UNLOCK {100};
inline constexpr int ICON_UNLOCK {150};

inline constexpr int INVALID_ID {MAX_ACCOUNTS + 1};

// Struct for saving and loading data.
struct PlayerData 
{
   std::string name, icon, color;
   unsigned long highScore;
   int lastClear;
   int id = INVALID_ID;

   PlayerData() = default;
   PlayerData(std::string_view name, std::string_view icon, std::string_view color, unsigned long highScore, int lastClear, int id);
};

// Class for saving, loading and editing player data.
class Save 
{
public:
   Save();
   ~Save();

   // Create/destroy a save file.
   void create(PlayerData& data);
   void destroy(int id);

   // Update a save file.
   void update(PlayerData& data, int id);
   void updateLevels(PlayerData& data, int id);
   void updateHighScore(PlayerData& data, unsigned long newScore, int id);

   // Update the main account ID or retrieve it.
   int retrieveMainAccount();
   void setMainAccount(int id);

   // Get all of the valid IDs that could be used.
   std::vector<int> getValidIds();

   // Select save file by id.
   PlayerData select(int id);
   PlayerData selectByName(const std::string& name);

   // Work with saving, retrieving or destroying endless save files.
   void saveEndlessProgress(const std::string& layout, unsigned long score, int id);
   std::pair<std::string, unsigned long> retrieveEndlessProgress(int id);
   void deleteEndlessProgress(int id); 

private:
   sqlite3* db;
   char* errorMsg;

   // Handle SQL injection errors.
   bool handleInsertion(int status, const std::string& error);
};

#endif // AA_SAVE_HPP
