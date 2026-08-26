-- Include all of the variables.
dofile("script/variables.lua")
local EM = 0

-- Define all of the layouts used in the daily levels.
Levels = {
   {
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
      },

      enemyCount = 35,
      enemyRates = {
         {LURKER, 85}, {WANDERER, 15},
      },
   },
   {
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,OB,EM,EM,OB,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,OB,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,OB,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,OB,EM,EM,OB,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
      },

      enemyCount = 15,
      enemyRates = {
         {GLIMMER, 50}, {H_BAT, 25}, {V_BAT, 25},
      },
   },
   {
      layout = {
         {OB,OB,OB,EM,EM,EM,EM,OB,OB,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,OB,OB,EM,EM,EM,EM,OB,OB,OB},
      },

      enemyCount = 15,
      enemyRates = {
         {MINE, 80}, {OBSERVER, 20},
      },
   },
   {
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,OB,OB,OB,OB,OB,OB,OB,OB,OB,EM,OB,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,OB,OB,OB,OB,OB,OB,OB,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,EM,EM,OB,OB,OB,OB,EM,EM,OB},
         {OB,EM,EM,OB,EM,OB,OB,OB,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,OB,EM,EM,EM,OB,EM,EM,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,OB,OB,OB,OB,OB,EM,EM,OB,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
      },

      enemyCount = 25,
      enemyRates = {
         {GLIMMER, 80}, {MINES, 15}, {OBSERVER, 5},
      },
   },
}