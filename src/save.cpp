#include "save.hpp"
#include "io.hpp"

#include <filesystem>
#include <map>
using std::map;

// Initialize a full player data struct.
PlayerData::PlayerData(std::string_view name, std::string_view icon, std::string_view color, unsigned long highScore, int lastClear, int id)
   : name(name), icon(icon), color(color), highScore(highScore), lastClear(lastClear), id(id) {}

// Create a new database and a new table if they do not exist yet.
Save::Save()
{
   std::filesystem::create_directories("data/");
   if (sqlite3_open("data/save.db", &db)) 
   {
      printf("%sFatal error: Could not open save file: %s\n%s ", RED, sqlite3_errmsg(db), RES);
      exit(-1);
   }

   // Create a new table if it does not exist.
   const char * sql {
      "CREATE TABLE IF NOT EXISTS SAVE("
      "ID INTEGER NOT NULL,"
      "NAME TEXT NOT NULL,"
      "ICON TEXT NOT NULL,"
      "COLOR TEXT NOT NULL,"
      "LAST_CLEAR INTEGER NOT NULL,"
      "HIGH_SCORE INTEGER NOT NULL);"
      "CREATE TABLE IF NOT EXISTS ENDLESS_SAVE("
      "ID INTEGER PRIMARY KEY,"
      "LAYOUT TEXT NOT NULL,"
      "SCORE INTEGER NOT NULL);"
      "CREATE TABLE IF NOT EXISTS MAIN_ACC(ID INTEGER PRIMARY KEY, ACC_ID INTEGER);"
   };

   if (sqlite3_exec(db, sql, nullptr, 0, &errorMsg) != SQLITE_OK) 
   {
      printf("%sFatal error: Could not create table: %s\n%s", RED, errorMsg, RES);
      sqlite3_free(errorMsg);
      exit(-1);
   }
}

// Close the database when the class is deleted.
Save::~Save()
{
   sqlite3_close(db);
}

// Create a brand new save file.
void Save::create(PlayerData& data)
{
   // Create an SQL prepared statement.
   const char * sql {"INSERT INTO SAVE(ID,NAME,ICON,COLOR,LAST_CLEAR,HIGH_SCORE)VALUES(?,?,?,?,1,0);"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, getValidIds().at(0));
   sqlite3_bind_text(stmt, 2, data.name.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 3, data.icon.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 4, data.color.c_str(), -1, SQLITE_STATIC);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not create account: ");
   data = selectByName(data.name);
}

// Delete a save file, return boolean based on the success.
void Save::destroy(int id)
{
   // Create an SQL prepared statement.
   const char * sql {"DELETE FROM SAVE WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, id);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not delete save: ");
}

// Update an existing save file.
void Save::update(PlayerData& data, int id)
{
   // Create an SQL prepared statement.
   const char * sql {"UPDATE SAVE SET NAME = ?, ICON = ?, COLOR = ?, LAST_CLEAR = ?, HIGH_SCORE = ? WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_text(stmt, 1, data.name.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, data.icon.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 3, data.color.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 4, data.lastClear);
   sqlite3_bind_int(stmt, 5, data.highScore);
   sqlite3_bind_int(stmt, 6, id);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not update save: ");
}

// Increment the last cleared level by one.
void Save::updateLevels(PlayerData& data, int id)
{
   // Create an SQL prepared statement.
   const char * sql {"UPDATE SAVE SET LAST_CLEAR = ? WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, data.lastClear + 1);
   sqlite3_bind_int(stmt, 2, id);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not update save: ");
   data.lastClear++;
}

// Update the endless mode high score.
void Save::updateHighScore(PlayerData& data, unsigned long newScore, int id)
{
   // Create an SQL prepared statement.
   const char * sql {"UPDATE SAVE SET HIGH_SCORE = ? WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, newScore);
   sqlite3_bind_int(stmt, 2, id);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);

   handleInsertion(status, "Could not update save: ");
   data.highScore = newScore;
}

// Retrieve the main account id.
int Save::retrieveMainAccount()
{
   // Prepare sql prepared statement.
   const char * sql {"SELECT ACC_ID FROM MAIN_ACC WHERE ID = 1;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   int id {};

   while (sqlite3_step(stmt) == SQLITE_ROW)
      id = sqlite3_column_int(stmt, 0);

   sqlite3_finalize(stmt);
   return id;
}

// Set the main account id.
void Save::setMainAccount(int id)
{
   // Create an SQL prepared statement.
   const char * sql {"INSERT OR REPLACE INTO MAIN_ACC(ID,ACC_ID)VALUES(1,?);"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1,id);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not update main account: ");
}

// Get all of the available IDs that could be used.
std::vector<int> Save::getValidIds()
{
   // Prepare sql prepared statement.
   const char * sql {"SELECT ID FROM SAVE;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   map<int, bool> ids;

   // Retrieve all of the data.
   while (sqlite3_step(stmt) == SQLITE_ROW)
      ids.insert({sqlite3_column_int(stmt, 0), true});

   std::vector<int> validIds;
   
   for (int i {1}; i <= MAX_ACCOUNTS; i++)
      if (!ids.count(i))
         validIds.push_back(i);

   // Finalize and return.
   sqlite3_finalize(stmt);
   return validIds;
}

// Return the player's data based on the id.
PlayerData Save::select(int id)
{
   // Prepare sql prepared statement.
   const char * sql {"SELECT * FROM SAVE WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind ID to the statement and return the save file if it exists.
   sqlite3_bind_int(stmt, 1, id);
   PlayerData data {};

   // Retrieve all of the data.
   while (sqlite3_step(stmt) == SQLITE_ROW) 
   {
      data.id = sqlite3_column_int(stmt, 0);
      data.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      data.icon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      data.color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
      data.lastClear = sqlite3_column_int(stmt, 4);
      data.highScore = sqlite3_column_int(stmt, 5);
   }

   // Finalize and return.
   sqlite3_finalize(stmt);
   return data;
}

// Return the player's data based on the id.
PlayerData Save::selectByName(const std::string& name)
{
   // Prepare sql prepared statement.
   const char * sql {"SELECT * FROM SAVE WHERE NAME = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind ID to the statement and return the save file if it exists.
   sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
   PlayerData data;

   // Retrieve all of the data.
   while (sqlite3_step(stmt) == SQLITE_ROW) 
   {
      data.id = sqlite3_column_int(stmt, 0);
      data.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      data.icon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      data.color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
      data.lastClear = sqlite3_column_int(stmt, 4);
      data.highScore = sqlite3_column_int(stmt, 5);
   }

   // Finalize and return.
   sqlite3_finalize(stmt);
   return data;
}

// Save the endless progress, including the map and the score.
void Save::saveEndlessProgress(const std::string& layout, unsigned long score, int id)
{
   // Create an SQL prepared statement.
   const char * sql {"INSERT INTO ENDLESS_SAVE(ID,LAYOUT,SCORE)VALUES(?,?,?);"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, id);
   sqlite3_bind_text(stmt, 2, layout.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 3, score);

   // Finalize.
   const int status {sqlite3_step(stmt)};
   sqlite3_finalize(stmt);
   handleInsertion(status, "Could not create an endless save: ");
}

// Retrieve the endless progress, both the map and the score.
std::pair<std::string, unsigned long> Save::retrieveEndlessProgress(int id)
{
    // Prepare sql prepared statement.
   const char * sql {"SELECT * FROM ENDLESS_SAVE WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind ID to the statement and return the save file if it exists.
   sqlite3_bind_int(stmt, 1, id);
   std::pair<std::string, unsigned long> result;

   // Retrieve all of the data and return the latest one.
   while (sqlite3_step(stmt) == SQLITE_ROW) 
   {
      result.first = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      result.second = sqlite3_column_int(stmt, 2);
   }

   // Finalize and return.
   sqlite3_finalize(stmt);
   return result;
}

// Delete the current endless save.
void Save::deleteEndlessProgress(int id)
{
   // Create an SQL prepared statement.
   const char * sql {"DELETE FROM ENDLESS_SAVE WHERE ID = ?;"};
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   // Bind to the statement.
   sqlite3_bind_int(stmt, 1, id);

   // Finalize.
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
}

// Handle SQL injection errors.
bool Save::handleInsertion(int status, const std::string& error)
{
   if (status != SQLITE_DONE)
      println(error + sqlite3_errmsg(db), RED);

   return status == SQLITE_DONE;
}
