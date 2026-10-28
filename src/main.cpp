#include "actions.hpp"
#include "audio.hpp"
#include "io.hpp"
#include <thread>

// Program entry function.
int main()
{
   srand(time(0));
   Save save;
   Lua lua("levels.lua");
   PlayerData data;

   // Initialize audio.
   initAudio();

   int mainAccount {save.retrieveMainAccount()};

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
      getHiddenInput("(Level Start)");
      playSound("click");
      println();

      // Play the level and do the action based on the result.
      const endState result {playLevel(level)};

      if (result == endState::won) 
      {
         if (level.getIndex() >= data.lastClear)
            std::thread(&Save::updateLevels, &save, std::ref(data), data.id).detach();
         
         getHiddenInput("\n(Level Complete)");
         playSound("click");
         level.story(storyType::outro);
      }
      else if (result == endState::lost) 
      {
         getHiddenInput("\n(Level Failed)");
         playSound("click");
         level.story(storyType::death);
      }
      else playSound("died");
   }
   return 0;
}
