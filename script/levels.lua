-- Import all of the variables from variables.lua.
dofile("script/variables.lua")
local EM = 0

-- All of the chapters used in the game.
Chapters = {
   {
      name = "Echoes of the Betrayed",
      from = 1,
      to = 8,
   },
   {
      name = "Unpleasant Stay",
      from = 9,
      to = 999,
   },
}

-- Levels contains all of the levels used by the C++ program.
Levels = {
   {
      name = "Test",

      layout = {
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}
      },

      intro = {{SKIP}},
      outro = {{SKIP}},
      death = {{SKIP}},
      goal  = {{SKIP}},
   },
   {
      name = "The Last Altar",

      layout = {
         {BR,BR,BR,OB,OB,OB,EM,BR,EM,OB,OB,BR,BR,BR,BR,OB},
         {BR,EM,EM,EM,EM,EM,EM,EM,BR,EM,EM,EM,EM,EM,EM,OB},
         {BR,EM,CO,EM,EM,EM,CO,EM,OB,EM,CO,EM,EM,CO,EM,EM},
         {OB,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,BR},
         {OB,OB,BR,EM,EM,EM,BR,OB,OB,OB,EM,OB,EM,EM,BR,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {BR,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,OB,BR,OB,OB,EM,EM,EM,EM},
         {EM,PL,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {EM,EM,CO,EM,BR,BR,EM,CO,EM,BR,BR,EM,CO,EM,BR,BR},
         {BR,EM,EM,EM,BR,BR,EM,EM,EM,EM,BR,EM,EM,EM,BR,BR},
         {OB,EM,OB,OB,EM,OB,EM,BR,BR,EM,OB,OB,OB,BR,OB,EM},
      },

      intro = {
         {SET_END_SOUND, "horn"},
         {"This " .. WORLD .. " was built on greed.", NARRATOR},
         {"Gold flowed, but never satisfied.", NARRATOR},
         {GODS .. " watched, patient and silent, as the temples changed into offices and markets,", NARRATOR},
         {"as everything got put a price on,", NARRATOR},
         {"as the earth itself was sold piece by piece.", NARRATOR},
         {"When kings and beggars alike started to worship gold, The " .. GODS .. " spoke as one:", NARRATOR},

         {SET_SOUND, "bell"},
         {"You have taken. Now you shall return.", GODS},
         {"We shall split the sky, swallow buildings, drain rivers and turn cities to dust.", GODS},

         {SET_SOUND, "click"},
         {"And so They did.", NARRATOR},

         {SET_SOUND, "destroy3"},
         {"Sky was split in half.", NARRATOR},
         {"Buildings were swallowed.", NARRATOR},
         {"Rivers were drained.", NARRATOR},
         {"And cities were turned to dust.", NARRATOR},

         {SET_SOUND, "bell"},
         {"We shall unleash the " .. WRAITHS .. " and turn The " .. WORLD .. " to " .. HELL .. ".", GODS},

         {SET_SOUND, "click"},
         {"And so They did.", NARRATOR},

         {SET_SOUND, "destroy3"},
         {"The " .. WRAITHS .. " were unleashed.", NARRATOR},
         {"And The " .. WORLD .. " was turned to " .. HELL .. ".", NARRATOR},

         {SET_SOUND, "bell"},
         {"Finally, We shall destroy your treasures.", GODS},

         {SET_SOUND, "click"},
         {"And so They did.", NARRATOR},
         {"The treasures were destroyed.", NARRATOR},
         {"But not all of them.", NARRATOR},
         {"And so The " .. GODS .. " said as one:", NARRATOR},

         {SET_SOUND, "bell"},
         {"To undo the curse, you must return the " .. FRAGMENTS .. " of your treasure, one by one, to The " .. GODS .. " who were betrayed.", GODS},
         {"When all of the " .. FRAGMENTS .. " are returned, one by one, to The " .. GODS .. " who were betrayed,", GODS},
         {"the sky shall mend together,", GODS},
         {"the buildings shall emerge,", GODS},
         {"the rivers shall flow", GODS},
         {"and the cities shall be restored.", GODS},

         {SET_SOUND, "click"},
         {"And so They promised.", NARRATOR},
         {"And so The " .. GODS .. " said as one once again:", NARRATOR},

         {SET_SOUND, "bell"},
         {"When all of the " .. FRAGMENTS .. " are returned, one by one, to The " .. GODS .. " who were betrayed,", GODS},
         {"the " .. WRAITHS .. " shall be expelled", GODS},
         {"and The " .. WORLD .. " shall be restored.", GODS},

         {SET_SOUND, "click"},
         {"And so They promised.", NARRATOR},

         {SET_SOUND, "accent"},
         {"And " .. PLR .. " listened.", NARRATOR},

         {SET_SOUND, "click"},
         {"This is the start of " .. PLR .. "'s journey to undo the curse.", NARRATOR},
         {"Of course, not alone, as the whole humanity has been cursed,", NARRATOR},
         {"the whole " .. WORLD .. " has been cursed.", NARRATOR},
      },

      outro = {
         {SET_END_SOUND, "horn"},
         {"Seven " .. FRAGMENTS .. " have been returned.", NARRATOR},
         {"Many more remain.", NARRATOR},
         {"When all is returned, the sky shall mend,", NARRATOR},
         {"the buildings shall emerge,", NARRATOR},
         {"the rivers shall flow", NARRATOR},
         {"and the cities shall be restored.", NARRATOR},
         {"So promised The " .. GODS .. ".", NARRATOR},
         {"When all is returned, the " .. WRAITHS .. " shall be expelled", NARRATOR},
         {"and The " .. WORLD .. " shall be restored.", NARRATOR},
         {"Such promises made The " .. GODS .. ".", NARRATOR},
         {"And that is why you must fulfill their needs,", NARRATOR},
         {"to restore The " .. WORLD .. ".", NARRATOR},
      },

      death = {{SKIP}},

      goal = {{COINS}},
   },
   {
      name = "Beach",

      layout = {
         {EM,EM,BR,BR,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM,EM,OB,OB,OB,EM,EM,EM,EM,OB},
         {EM,BR,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM,EM,OB,OB,EM,OB,OB,EM,EM},
         {EM,EM,BR,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM},
         {EM,EM,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,EM,EM,EM,OB,EM,EM},
         {EM,BR,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM},
         {EM,EM,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,BR,BR,BR,EM,EM,BR,EM,EM,EM,EM,EM,OB,OB},
         {EM,PL,EM,EM,EM,EM,EM,EM,EM,BR,BR,BR,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,BR,EM,CO,EM,EM,EM,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,OB,OB,EM,EM,CO,EM,EM,BR,BR,EM,EM,EM,BR,BR,EM,EM,EM,EM,EM,EM,OB,OB,EM},
         {EM,EM,OB,OB,EM,EM,EM,EM,EM,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM},
         {OB,EM,EM,EM,EM,EM,OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,OB,EM,EM},
         {OB,OB,EM,EM,EM,OB,OB,OB,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,EM,EM,EM,EM,EM,OB},
      },

      intro = {
         {SET_END_SOUND, "horn"},
         {"You arrive at the " .. Wrap("Beach", YELLOW) .. " - your favorite spot to view the sky", NARRATOR},
         {"to find out if The " .. GODS .. " really did split the sky.", NARRATOR},

         {SET_SOUND, "destroy3"},
         {"As you look up, gosh!", NARRATOR},
         {"The sky above is split in two halves.", NARRATOR},
         {"The sea below is completely drained.", NARRATOR},
         {"And surrounded area is demolished.", NARRATOR},

         {SET_SOUND, "click"},
         {"The " .. GODS .. " really did split the sky.", NARRATOR},
         {"People start to realize, how beautiful the sky was.", NARRATOR},
      },

      outro = {
         {SET_END_SOUND, "horn"},
         {"Six " .. FRAGMENTS .. " have been returned.", NARRATOR},
         {"Many more remain.", NARRATOR},
         {"When all is returned, the sky shall mend.", NARRATOR},
         {"So promised The " .. GODS .. ".", NARRATOR},
         {"And so believe the people.", NARRATOR},
         {"After seeing the sky split in half, they are slightly more faithful to The " .. GODS .. ".", NARRATOR},
      },

      death = {{SKIP}},

      goal = {{COINS}},
   },
}