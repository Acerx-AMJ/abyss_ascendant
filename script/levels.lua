-- Import all of the variables from variables.lua.
dofile("script/variables.lua")

-- All of the chapters used in the game.
Chapters = {
   {
      name = "Abyss Ascendant",
      from = 1,
      to = 8,
   },
}

-- Levels contains all of the levels used by the C++ program.
Levels = {
   {
      name = "Stranger",
      layout = {
         {OB,OB,OB,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,OB,OB,OB},
         {OB,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,BR,OB},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,BR,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,EM,EM,EM,CO,EM,EM},
         {EM,EM,EM,EM,EM,OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM},
         {BR,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,OB,EM,EM,EM,EM},
         {BR,PL,EM,EM,OB,EM,EM,OB,OB,EM,EM,OB,EM,EM,EM,EM},
         {BR,NY,EM,EM,OB,EM,EM,OB,OB,EM,EM,OB,EM,EM,CO,EM},
         {BR,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,OB,EM,EM,EM,EM},
         {EM,EM,CO,EM,EM,OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM},
         {OB,BR,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,BR,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,BR,BR,OB},
         {OB,OB,OB,EM,EM,EM,BR,BR,BR,BR,EM,EM,EM,OB,OB,OB},
      },

      intro = {
         {"It was a quiet day at the park.", NAR},
         {"Unusually quiet.", NAR},
         {PLR .. " was sitting on a bench.", NAR},
         {"He was completely alone.", NAR},
         {"Something's wrong. It's never this quiet here.", PLR},
         {"It was Sunday afternoon. A few weeks ago the park at a time like this would always be full.", NAR},
         {"As " .. PLR .. " started getting anxious, they saw a stranger run up to them.", NAR},
         {"... Hey!", NYX_ST},
         {THREE, NYX_ST, "Hey!", "Hey! Is everything okay?", "You look worried!",
            PATH1, PATH2, PATH2},

         {PATH1, "I uhh...", NYX_ST},
         {PATH1, "Do you know about the " .. ABYSS .. "?", NYX_ST},
         {PATH1, "No... what's that?", PLR},

         {PATH2, "Do you not know?", NYX_ST},

         {"The " .. ABYSS .. " is going rampant on this planet!", NYX_ST},
         {"Just... come with me!", NYX_ST},
         {"What? Where?", PLR},
         {"We don't have the time for this!", NYX_ST},
      },

      outro = {
         {"Looks like you're actually going to be useful.", NYX_ST},
         {"Great.", PLR},
         {"What do we do with these?", PLR},
         {"They'll aid me in research.", NYX_ST},
         {"By the way, I'm " .. NYX .. ".", NYX},
         {PLR .. ".", PLR},
         {"Alright, let's continue.", NYX},
      },
      death = {{SKIP}},
      goal = {{COINS}, {ABOVE_Y, 2}},
   },
   {
      name = "Apprentice",
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {OB,OB,EM,EM,EM,BR,BR,BR,BR,BR,EM,EM,EM,EM,EM,EM,BR,BR,BR,BR,BR,EM,EM,EM},
         {OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,OB,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,OB,EM,EM,CO,EM,BR,BR,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,BR,BR,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,BR,BR,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,EM,BR,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,BR,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,CO,EM,EM,CO,EM},
         {OB,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM},
         {OB,BR,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM},
         {OB,BR,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB,EM,EM,EM,EM,EM,EM},
         {OB,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,EM,EM,EM,EM,EM,EM,EM,EM,BR,BR,BR,BR,BR,BR,EM,EM,EM,EM,EM,OB,OB,OB,OB},
         {OB,EM,EM,NY,PL,EM,EM,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
      },

      intro = {
         {"Where are we going?", PLR},
         {"To the hotel.", NYX},
         {"Why there?", PLR},
         {"There are important supplies there.", NYX},
         {"Besides, a " .. ORANGE .. "Scorian" .. RESET .. " base is close to there.", NYX},
         {"What supplies are there?", PLR},
         {"Stop asking so many questions and let's go!", NYX},
      },
      outro = {
         {"Alright, we aren't that far away from it.", NYX},
         {"Have you seen a " .. WRAITH .. " yet?", NYX},
         {"No.", PLR},
         {"Well, when you do, don't touch them.", NYX},
         {"They aren't friendly.", NYX},
         {"Okay.", PLR},
         {THREE, NYX, "What do they look like?", "What happens when I touch them?", "How do I kill them?",
            "Otherworldly.", "Bad things.", "Leave that to me."},
         {"Wow.", PLR},
      },
      death = {{SKIP}},
      goal = {{COINS}, {ABOVE_X, 23}},
   },
}