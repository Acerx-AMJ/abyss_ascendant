#include "actions.hpp"
#include "infinity.hpp"
#include "audio.hpp"
#include "io.hpp"

// Constants for customization.
std::vector<std::string> iconOptions {{"Block: []", "Elipsis: ()", "Curly: {}", "Money: $$", "Hash: ##", "Xs: XX", "Smiley face: ☻", "Spade: ♠", "Club: ♣", "Heart: ♥", "Diamond: ♦", "Sun: ☀", "Cogwheel: ⚙", "Pawn: ♟", "Knight: ♞", "Queen: ♛", "King: ♔", "Rook: ♖", "Bishop: ♗", "Seeker: ◊"}};
std::vector<std::string> colorOptions {{wrap("Pink", PINK), wrap("Red", RED), wrap("Orange", ORANGE), wrap("Yellow", YELLOW), wrap("Green", GREEN), wrap("Cyan", CYAN), wrap("Blue", BLUE), wrap("Purple", PURPLE), wrap("Brown", BROWN), wrap("Gray", GRAY), "White"}};

const std::vector<std::string> iconValues {{"[]", "()", "{}", "$$", "##", "XX", "☻", "♠", "♣", "♥", "♦", "☀", "⚙", "♟", "♞", "♛", "♔", "♖", "♗", "◊", "C"}};
const std::vector<std::string> colorValues {{PINK, RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, BROWN, GRAY, "", "C"}};

// Let the user create a custom color.
std::string createColor()
{
   const auto condition = [](int num) -> bool
   {
      return num >= 0 && num <= 255;
   };

   const int red = getNumber("Input the " + wrap("Red", RED) + " value > ", condition);
   const int green = getNumber("Input the " + wrap("Green", GREEN) + " value > ", condition);
   const int blue = getNumber("Input the " + wrap("Blue", BLUE) + " value > ", condition);

   const bool bright {getConsent("Make your username " + wrap("Bold", "\033[1;37m") + "? [y/n] > ")};
   const bool italic {getConsent("Make your username " + wrap("Italic", "\033[3;37m") + "? [y/n] > ")};

   return std::string("\033[") + (bright ? "1;" : "") + (italic ? "3;" : "") + "38;2;" + tostr(red) + ";" + tostr(green) + ";" + tostr(blue) + "m";
}

// Let the user create an icon.
std::string createIcon()
{
   std::string input {getInput("Input an icon > ", "", 1, MAX_ICON_LENGTH * 4, false)};

   while (utf32Size(input) > MAX_ICON_LENGTH) 
   {
      clearln(2);
      input = getInput("Invalid input, try again > ", RED, 1, MAX_ICON_LENGTH * 4, false);
   }
   return input;
}

// Let the user select a color, optionally create one.
std::string selectColor(unsigned long highScore)
{
   if (highScore >= COLOR_UNLOCK && colorOptions.size() != colorValues.size())
   {
      colorOptions.reserve(colorOptions.size() + 1);
      colorOptions.push_back("Create a custom color");
   }
   else if (highScore < COLOR_UNLOCK && colorOptions.size() == colorValues.size())
      colorOptions.pop_back();

   println("Select your color.");
   const std::string color {getOption(colorOptions, colorValues)};

   if (color != "C")
      return color;

   return createColor();
}

// Let the user select an icon, optionally create one.
std::string selectIcon(unsigned long highScore)
{
   if (highScore >= ICON_UNLOCK && iconOptions.size() != iconValues.size())
   {
      iconOptions.reserve(iconOptions.size() + 1);
      iconOptions.push_back("Create a custom icon");
   }
   else if (highScore < ICON_UNLOCK && iconOptions.size() == iconValues.size())
      iconOptions.pop_back();

   println("Select your icon.");
   const std::string icon {getOption(iconOptions, iconValues)};

   if (icon != "C")
      return icon;

   return createIcon();
}

// Let the user type out their username.
std::string selectUsername(Save& save)
{
   std::string name {getInput("Input your name > ", "", MIN_USER_LENGTH, MAX_USER_LENGTH)};
   
   while (save.selectByName(name).id != INVALID_ID && save.selectByName(name).id != save.select(save.selectByName(name).id).id)
   {
      clearln(2);
      name = getInput("Account with that name already exists, try something else > ", RED, MIN_USER_LENGTH, MAX_USER_LENGTH);
   }
   return name;
}

// Let the user create a new save.
void createSave(Save& save, PlayerData& data)
{
   const std::string name {selectUsername(save)};
   data = PlayerData(name, selectIcon(0), selectColor(0), 0, 1, INVALID_ID);

   // Create the save and update it.
   save.create(data);
   println("Successfully created a new save!", GREEN);
   playSound("success");
}

// Let the user customize their name, icon or color.
void customize(Save& save, PlayerData& data)
{
   switch (getOption({"Name", "Icon", "Color", "Reset high score"}, {"n", "i", "c", "h"}).at(0)) 
   {
   case 'n':
   {
      const std::string newName = selectUsername(save);
      if (data.name != newName)
         data.name = newName;

      break;
   }
   case 'i': data.icon = selectIcon(data.highScore); break;
   case 'c': data.color = selectColor(data.highScore); break;
   case 'h':
   {
      if (getConsent("Are you sure that you want to reset your high score? [y/n] > ", RED))
      {
         data.highScore = 0;
         save.deleteEndlessProgress(data.id);
      }
      break;
   }
   default: return;
   }

   // Update the player data.
   playSound("success");
   save.update(data, data.id);
}

// Let the user select the mode, the level or an action.
Level mainMenu(Save& save, Lua& lua, PlayerData& data)
{
   const std::vector<std::string> chapters {lua.getChapters()};
   const int chapterCount = chapters.size();

   println("\nABYSS DESCENDANT", RED);
   println("Main menu. " + wrap(data.name + " " + data.icon, data.color));
   println("\nOption:", YELLOW);

   // List all of the actions and levels.
   println(" > [-5] Reload Lua Scripts\n > [-4] Change Save\n > [-3] Customize\n > [-2] " 
      + (data.highScore < DAILY_UNLOCK ? wrap("LOCKED ???", YELLOW) : "Daily challenge") + "\n > [-1] "
      + (data.lastClear < ENDLESS_UNLOCK ? wrap("LOCKED beat 'Unpleasant Stay' to unlock.", YELLOW)
      : "Endless, HighScore: " + wrap(tostr(data.highScore), YELLOW)) + "\n > [ 0] Quit");

   int chaptersUnlocked {};
   bool lockedChapter {false};

   for (int i {}; i < chapterCount; i++) 
   {
      if (data.lastClear >= lua.loadChapter(i + 1).x)
      {
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + chapters.at(i));
         chaptersUnlocked++;
      }
      else if (!lockedChapter)
      {
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + wrap("LOCKED", YELLOW));
         lockedChapter = true;
      }
   }

   const auto leftOverLevels {lua.getLeftOverLevels()};
   bool lockedLeftOver {false};

   for (int i {chapterCount}; i < leftOverLevels.size() + chapterCount; i++)
   {
      if (data.lastClear >= leftOverLevels.at(i - chapterCount).second)
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + leftOverLevels.at(i - chapterCount).first);
      else if (chaptersUnlocked == chapterCount && !lockedLeftOver)
      {
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + wrap("LOCKED", YELLOW));
         lockedLeftOver = true;
      }
   }

   // Get choice from player and return it.
   int max = std::min(chaptersUnlocked, chapterCount);

   int index = getNumber("\nYour choice > ",
   [max, chapterCount, &leftOverLevels, &data](int num) -> bool
   {
      if ((num == -2 && data.highScore < DAILY_UNLOCK) || (num == -1 && data.lastClear < ENDLESS_UNLOCK))
         return false;

      if (num > chapterCount && num - chapterCount <= leftOverLevels.size() && data.lastClear >= leftOverLevels.at(num - 1 - chapterCount).second)
         return true;
   
      return num >= -5 && num <= max;
   });

   playSound("click");

   // Do the action specified by the user.
   if (index == -5) 
   {
      lua.reload();
      story.reload();
      daily.reload();

      playSound("success");
      println("Successfully reloaded Lua scripts.", GREEN); 
      return mainMenu(save, lua, data); 
   }
   else if (index == -4)
   {
      return manageSave(save, lua, data);
   }
   else if (index == -3) 
   {
      customize(save, data);
      println("Successfully saved the new character.", GREEN);
      return mainMenu(save, lua, data);
   }
   else if (index == -2) 
   {
      // Return the today's daily level.
      return generateDaily(data);
   }
   else if (index == -1)
   {
      // Play endless and update high score if there's a new record.
      endless(save, data);
      return mainMenu(save, lua, data);
   }
   else if (index == 0) 
   { 
      delaySound("accent");
      println("Quitting.", BLUE); 
      exit(0); 
   }
   else if (index > chapterCount)
   {
      return lua.loadLevel(leftOverLevels.at(index - chapterCount - 1).second, data);
   }

   // Get the levels from the given chapter.
   const Vec2 chapter {lua.loadChapter(index)};
   const std::vector<std::string> levels {lua.getLevelNames(chapter.x, chapter.y)};
   const int levelCount = levels.size();

   println("\nOption:", YELLOW);
   println(" > [-1] Go Back\n > [ 0] Quit");

   bool lockedLevel {false};

   for (int i {}; i < levelCount; i++)
   {
      if (data.lastClear >= i + chapter.x)
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + levels.at(i));
      else if (!lockedLevel)
      {
         println(" > [" + tostr(tostr(i + 1).size() == 1 ? " " : "") + tostr(i + 1) + "] " + wrap("LOCKED", YELLOW));
         lockedLevel = true;
      }
   }

   // Get choice from player and return it.
   max = std::min<int>(data.lastClear - chapter.x + 1, levelCount);

   index = getNumber("\nYour choice > ",
   [max](int num) -> bool
   {
      return num >= -1 && num <= max;
   });

   playSound("click");

   // Do the action specified by the user.
   if (index == -1)
   {
      playSound("failure");
      return mainMenu(save, lua, data);
   }
   else if (index == 0)
   {
      delaySound("accent");
      println("Quitting.", BLUE); 
      exit(0);
   }

   // Return the selected level.
   return lua.loadLevel(index + chapter.x - 1, data);
}

// Let the user manage their saves.
Level manageSave(Save& save, Lua& lua, PlayerData& data)
{
   std::vector<PlayerData> accounts {};
   println("\nCurrent Account: " + wrap(data.name + " " + data.icon, data.color));

   println("\nOption:", YELLOW);
   println(" > [-4] Set Main Account\n > [-3] Create a New Account\n > [-2] Delete Current Account\n > [-1] Go Back\n > [ 0] Quit");

   const int mainAccount {save.retrieveMainAccount()};
   int offset {};

   for (int i {1}; i <= MAX_ACCOUNTS; i++)
   {
      const PlayerData account {save.select(i)};
 
      if (account.id == INVALID_ID)
      {
         offset++;
         continue;
      }

      accounts.push_back(account);
      println(" > [" + tostr(tostr(i - offset).size() == 1 ? " " : "") + tostr(i - offset) + "] " + wrap(account.name + " " + account.icon, account.color) + (account.id == mainAccount ? wrap(" <- Main", YELLOW) : ""));
   }

   const int accountCount = accounts.size();

   const short index = getNumber("\nYour choice > ",
   [accountCount](int num) -> bool
   {
      return num >= -4 && num <= accountCount;
   });

   if (index == -4)
   {
      const int accountId = getNumber("Choose the account index > ",
      [accountCount](int num) -> bool
      {
         return num >= 1 && num <= accountCount;
      });

      save.setMainAccount(accountId);

      playSound("success");
      println("Successfully set new main account.", GREEN);
      return manageSave(save, lua, data);
   }
   else if (index == -3)
   {
      if (accountCount >= MAX_ACCOUNTS)
      {
         playSound("failure");
         println("Can't create a new account as there are too many.", RED);
         return manageSave(save, lua, data);
      }

      createSave(save, data);
      return manageSave(save, lua, data);
   }
   else if (index == -2)
   {
      if (getConsent("Are you sure that you want to delete account '" + wrap(data.name + " " + data.icon, data.color) + wrap("'? [y/n] > ", RED), RED))
      {
         save.destroy(data.id);
         playSound("success");
         println("Successfully deleted account.", GREEN);

         if (accountCount - 1 <= 0)
         {
            createSave(save, data);
            save.setMainAccount(1);
         }
         else if (data.id == mainAccount)
         {
            save.setMainAccount(accounts.at(0).id);
            data = accounts.at(0);
         }
         else
         {
            for (const auto& account : accounts)
            {
               if (account.id == mainAccount)
               {
                  data = account;
                  break;
               }
            }
         }
      }
      else
      {
         playSound("failure");
         println("Account not deleted.", RED);
      }

      return manageSave(save, lua, data);
   }
   else if (index == -1)
   {
      playSound("failure");
      return mainMenu(save, lua, data);
   }
   else if (index == 0)
   {
      delaySound("accent");
      println("Quitting.", BLUE); 
      exit(0);
   }

   data = accounts.at(index - 1);
   playSound("success");
   println("Successfully switched to account '" + wrap(data.name + " " + data.icon, data.color) + wrap("'.", GREEN), GREEN);
   return manageSave(save, lua, data);
}