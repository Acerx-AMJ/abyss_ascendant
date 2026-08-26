#include "actions.hpp"
#include "audio.hpp"
#include "io.hpp"
#include <thread>

// Program entry function.
int32 main()
{
   srand(time(0));
   Save save;
   Lua lua("levels.lua"s);
   PlayerData data;

   // Initialize audio.
   initAudio();

   uint16 mainAccount {save.retrieveMainAccount()};

   if (mainAccount == 0)
   {
      mainAccount = 1;
      save.setMainAccount(mainAccount);
   }

   // Load or create new save file.
   if (save.select(mainAccount).id == INVALID_ID) 
      createSave(save, data);
   else
      data = save.select(mainAccount);

   while (true) 
   {
      Level level {mainMenu(save, lua, data)};

      // Print the intro.
      level.story(storyType::intro);
      getHiddenInput("(Level Start)"s);
      playSound("click"s);
      println();

      // Play the level and do the action based on the result.
      const endState result {playLevel(level)};

      if (result == endState::won) 
      {
         if (level.getIndex() >= data.lastClear)
            std::thread(&Save::updateLevels, &save, std::ref(data), data.id).detach();
         
         getHiddenInput("\n(Level Complete)"s);
         playSound("click"s);
         level.story(storyType::outro);
      }
      else if (result == endState::lost) 
      {
         getHiddenInput("\n(Level Failed)"s);
         playSound("click"s);
         level.story(storyType::death);
      }
      else playSound("died"s);
   }
   return 0;
}
