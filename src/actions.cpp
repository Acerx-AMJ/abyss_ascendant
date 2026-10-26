#include "actions.hpp"
#include "infinity.hpp"
#include "audio.hpp"
#include "io.hpp"

// Constants for customization.
v_str iconOptions {{"Block: []"s, "Elipsis: ()"s, "Curly: {}"s, "Money: $$"s, "Hash: ##"s, "Xs: XX"s, "Smiley face: ☻"s, "Spade: ♠"s, "Club: ♣"s, "Heart: ♥"s, "Diamond: ♦"s, "Sun: ☀"s, "Cogwheel: ⚙"s, "Pawn: ♟"s, "Knight: ♞"s, "Queen: ♛"s, "King: ♔"s, "Rook: ♖"s, "Bishop: ♗"s, "Seeker: ◊"s}};
v_str colorOptions {{wrap("Pink"s, PINK), wrap("Red"s, RED), wrap("Orange"s, ORANGE), wrap("Yellow"s, YELLOW), wrap("Green"s, GREEN), wrap("Cyan"s, CYAN), wrap("Blue"s, BLUE), wrap("Purple"s, PURPLE), wrap("Brown"s, BROWN), wrap("Gray"s, GRAY), "White"s}};

const v_str iconValues {{"[]"s, "()"s, "{}"s, "$$"s, "##"s, "XX"s, "☻"s, "♠"s, "♣"s, "♥"s, "♦"s, "☀"s, "⚙"s, "♟"s, "♞"s, "♛"s, "♔"s, "♖"s, "♗"s, "◊"s, "C"s}};
const v_str colorValues {{PINK, RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, BROWN, GRAY, ""s, "C"s}};

// Let the user create a custom color.
str createColor()
{
   const auto condition = [](int num) -> bool
   {
      return num >= 0 && num <= 255;
   };

   const uint8 red = getNumber("Input the "s + wrap("Red"s, RED) + " value > "s, condition);
   const uint8 green = getNumber("Input the "s + wrap("Green"s, GREEN) + " value > "s, condition);
   const uint8 blue = getNumber("Input the "s + wrap("Blue"s, BLUE) + " value > "s, condition);

   const bool bright {getConsent("Make your username "s + wrap("Bold"s, "\033[1;37m"s) + "? [y/n] > "s)};
   const bool italic {getConsent("Make your username "s + wrap("Italic"s, "\033[3;37m"s) + "? [y/n] > "s)};

   return "\033["s + (bright ? "1;"s : ""s) + (italic ? "3;"s : ""s) + "38;2;"s + tostr(red) + ";"s + tostr(green) + ";"s + tostr(blue) + "m"s;
}

// Let the user create an icon.
str createIcon()
{
   str input {getInput("Input an icon > "s, ""s, 1, MAX_ICON_LENGTH * 4, false)};

   while (utf32Size(input) > MAX_ICON_LENGTH) 
   {
      clearln(2);
      input = getInput("Invalid input, try again > "s, RED, 1, MAX_ICON_LENGTH * 4, false);
   }
   return input;
}

// Let the user select a color, optionally create one.
str selectColor(uint64 highScore)
{
   if (highScore >= COLOR_UNLOCK && colorOptions.size() != colorValues.size())
   {
      colorOptions.reserve(colorOptions.size() + 1);
      colorOptions.push_back("Create a custom color"s);
   }
   else if (highScore < COLOR_UNLOCK && colorOptions.size() == colorValues.size())
      colorOptions.pop_back();

   println("Select your color."s);
   const str color {getOption(colorOptions, colorValues)};

   if (color != "C"s)
      return color;

   return createColor();
}

// Let the user select an icon, optionally create one.
str selectIcon(uint64 highScore)
{
   if (highScore >= ICON_UNLOCK && iconOptions.size() != iconValues.size())
   {
      iconOptions.reserve(iconOptions.size() + 1);
      iconOptions.push_back("Create a custom icon");
   }
   else if (highScore < ICON_UNLOCK && iconOptions.size() == iconValues.size())
      iconOptions.pop_back();

   println("Select your icon."s);
   const str icon {getOption(iconOptions, iconValues)};

   if (icon != "C"s)
      return icon;

   return createIcon();
}

// Let the user type out their username.
str selectUsername(Save& save)
{
   str name {getInput("Input your name > "s, ""s, MIN_USER_LENGTH, MAX_USER_LENGTH)};
   
   while (save.selectByName(name).id != INVALID_ID && save.selectByName(name).id != save.select(save.selectByName(name).id).id)
   {
      clearln(2);
      name = getInput("Account with that name already exists, try something else > "s, RED, MIN_USER_LENGTH, MAX_USER_LENGTH);
   }
   return name;
}

// Let the user create a new save.
void createSave(Save& save, PlayerData& data)
{
   const str name {selectUsername(save)};
   data = PlayerData(name, selectIcon(0), selectColor(0), 0, 1, INVALID_ID);

   // Create the save and update it.
   save.create(data);
   println("Successfully created a new save!"s, GREEN);
   playSound("success"s);
}

// Let the user customize their name, icon or color.
void customize(Save& save, PlayerData& data)
{
   switch (getOption({"Name"s, "Icon"s, "Color"s, "Reset high score"s}, {"n"s, "i"s, "c"s, "h"s}).at(0)) 
   {
   case 'n':
   {
      const str newName = selectUsername(save);
      if (data.name != newName)
         data.name = newName;

      break;
   }
   case 'i': data.icon = selectIcon(data.highScore); break;
   case 'c': data.color = selectColor(data.highScore); break;
   case 'h':
   {
      if (getConsent("Are you sure that you want to reset your high score? [y/n] > "s, RED))
      {
         data.highScore = 0;
         save.deleteEndlessProgress(data.id);
      }
      break;
   }
   default: return;
   }

   // Update the player data.
   playSound("success"s);
   save.update(data, data.id);
}

// Let the user select the mode, the level or an action.
Level mainMenu(Save& save, Lua& lua, PlayerData& data)
{
   const v_str chapters {lua.getChapters()};
   const uint16 chapterCount = chapters.size();

   println("\nABYSS DESCENDANT"s, RED);
   println("Main menu. "s + wrap(data.name + " "s + data.icon, data.color));
   println("\nOption:"s, YELLOW);

   // List all of the actions and levels.
   println(" > [-5] Reload Lua Scripts\n > [-4] Change Save\n > [-3] Customize\n > [-2] "s 
      + (data.highScore < DAILY_UNLOCK ? wrap("LOCKED ???"s, YELLOW) : "Daily challenge"s) + "\n > [-1] "s
      + (data.lastClear < ENDLESS_UNLOCK ? wrap("LOCKED beat 'Unpleasant Stay' to unlock."s, YELLOW)
      : "Endless, HighScore: "s + wrap(tostr(data.highScore), YELLOW)) + "\n > [ 0] Quit"s);

   uint16 chaptersUnlocked {};
   bool lockedChapter {false};

   for (uint16 i {}; i < chapterCount; i++) 
   {
      if (data.lastClear >= lua.loadChapter(i + 1).x)
      {
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + chapters.at(i));
         chaptersUnlocked++;
      }
      else if (!lockedChapter)
      {
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + wrap("LOCKED"s, YELLOW));
         lockedChapter = true;
      }
   }

   const auto leftOverLevels {lua.getLeftOverLevels()};
   bool lockedLeftOver {false};

   for (uint16 i {chapterCount}; i < leftOverLevels.size() + chapterCount; i++)
   {
      if (data.lastClear >= leftOverLevels.at(i - chapterCount).second)
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + leftOverLevels.at(i - chapterCount).first);
      else if (chaptersUnlocked == chapterCount && !lockedLeftOver)
      {
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + wrap("LOCKED"s, YELLOW));
         lockedLeftOver = true;
      }
   }

   // Get choice from player and return it.
   uint16 max {std::min(chaptersUnlocked, chapterCount)};

   int16 index = getNumber("\nYour choice > "s,
   [max, chapterCount, &leftOverLevels, &data](int num) -> bool
   {
      if ((num == -2 && data.highScore < DAILY_UNLOCK) || (num == -1 && data.lastClear < ENDLESS_UNLOCK))
         return false;

      if (num > chapterCount && num - chapterCount <= leftOverLevels.size() && data.lastClear >= leftOverLevels.at(num - 1 - chapterCount).second)
         return true;
   
      return num >= -5 && num <= max;
   });

   playSound("click"s);

   // Do the action specified by the user.
   if (index == -5) 
   {
      lua.reload();
      story.reload();
      daily.reload();

      playSound("success"s);
      println("Successfully reloaded Lua scripts."s, GREEN); 
      return mainMenu(save, lua, data); 
   }
   else if (index == -4)
   {
      return manageSave(save, lua, data);
   }
   else if (index == -3) 
   {
      customize(save, data);
      println("Successfully saved the new character."s, GREEN);
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
      delaySound("accent"s);
      println("Quitting."s, BLUE); 
      exit(0); 
   }
   else if (index > chapterCount)
   {
      return lua.loadLevel(leftOverLevels.at(index - chapterCount - 1).second, data);
   }

   // Get the levels from the given chapter.
   const Vec2 chapter {lua.loadChapter(index)};
   const v_str levels {lua.getLevelNames(chapter.x, chapter.y)};
   const uint16 levelCount = levels.size();

   println("\nOption:"s, YELLOW);
   println(" > [-1] Go Back\n > [ 0] Quit"s);

   bool lockedLevel {false};

   for (uint16 i {}; i < levelCount; i++)
   {
      if (data.lastClear >= i + chapter.x)
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + levels.at(i));
      else if (!lockedLevel)
      {
         println(" > ["s + tostr(tostr(i + 1).size() == 1 ? " "s : ""s) + tostr(i + 1) + "] "s + wrap("LOCKED"s, YELLOW));
         lockedLevel = true;
      }
   }

   // Get choice from player and return it.
   max = std::min(static_cast<uint16>(data.lastClear - chapter.x + 1), levelCount);

   index = getNumber("\nYour choice > "s,
   [max](int num) -> bool
   {
      return num >= -1 && num <= max;
   });

   playSound("click"s);

   // Do the action specified by the user.
   if (index == -1)
   {
      playSound("failure"s);
      return mainMenu(save, lua, data);
   }
   else if (index == 0)
   {
      delaySound("accent"s);
      println("Quitting."s, BLUE); 
      exit(0);
   }

   // Return the selected level.
   return lua.loadLevel(index + chapter.x - 1, data);
}

// Let the user manage their saves.
Level manageSave(Save& save, Lua& lua, PlayerData& data)
{
   std::vector<PlayerData> accounts {};
   println("\nCurrent Account: "s + wrap(data.name + " "s + data.icon, data.color));

   println("\nOption:"s, YELLOW);
   println(" > [-4] Set Main Account\n > [-3] Create a New Account\n > [-2] Delete Current Account\n > [-1] Go Back\n > [ 0] Quit"s);

   const uint16 mainAccount {save.retrieveMainAccount()};
   uint16 offset {};

   for (uint16 i {1}; i <= MAX_ACCOUNTS; i++)
   {
      const PlayerData account {save.select(i)};
 
      if (account.id == INVALID_ID)
      {
         offset++;
         continue;
      }

      accounts.push_back(account);
      println(" > ["s + tostr(tostr(i - offset).size() == 1 ? " "s : ""s) + tostr(i - offset) + "] "s + wrap(account.name + " "s + account.icon, account.color) + (account.id == mainAccount ? wrap(" <- Main"s, YELLOW) : ""s));
   }

   const uint16 accountCount = accounts.size();

   const short index = getNumber("\nYour choice > "s,
   [accountCount](int num) -> bool
   {
      return num >= -4 && num <= accountCount;
   });

   if (index == -4)
   {
      const uint16 accountId = getNumber("Choose the account index > "s,
      [accountCount](int num) -> bool
      {
         return num >= 1 && num <= accountCount;
      });

      save.setMainAccount(accountId);

      playSound("success"s);
      println("Successfully set new main account."s, GREEN);
      return manageSave(save, lua, data);
   }
   else if (index == -3)
   {
      if (accountCount >= MAX_ACCOUNTS)
      {
         playSound("failure"s);
         println("Can't create a new account as there are too many."s, RED);
         return manageSave(save, lua, data);
      }

      createSave(save, data);
      return manageSave(save, lua, data);
   }
   else if (index == -2)
   {
      if (getConsent("Are you sure that you want to delete account '"s + wrap(data.name + " "s + data.icon, data.color) + wrap("'? [y/n] > "s, RED), RED))
      {
         save.destroy(data.id);
         playSound("success"s);
         println("Successfully deleted account."s, GREEN);

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
         playSound("failure"s);
         println("Account not deleted."s, RED);
      }

      return manageSave(save, lua, data);
   }
   else if (index == -1)
   {
      playSound("failure"s);
      return mainMenu(save, lua, data);
   }
   else if (index == 0)
   {
      delaySound("accent"s);
      println("Quitting."s, BLUE); 
      exit(0);
   }

   data = accounts.at(index - 1);
   playSound("success"s);
   println("Successfully switched to account '"s + wrap(data.name + " "s + data.icon, data.color) + wrap("'."s, GREEN), GREEN);
   return manageSave(save, lua, data);
}