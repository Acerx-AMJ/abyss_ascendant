-- Import all of the variables from variables.lua.
dofile("script/variables.lua")

-- All of the chapters used in the game.
Chapters = {
   {
      name = "The Hotel",
      from = 1,
      to = 80,
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
   {
      name = "First Encounter",
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,CO,EM,BR,OB,EM,EM,EM,EM,CO,EM,BR,BR,EM,OB,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,OB,OB,OB,OB,OB,EM,EM,EM,OB,OB,OB,OB,OB,OB,OB,EM,EM,EM,EM},
         {OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {OB,OB,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,MI,EM,EM,EM,CO,EM,EM,EM,EM},
         {OB,OB,EM,EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,MI,BR,BR,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,BR,BR,BR,MI,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,CO,EM,EM,EM,EM,EM,EM,EM,EM,MI,BR,BR,BR,BR,EM,EM,EM,EM,EM,CO,EM},
         {PL,EM,EM,EM,EM,EM,EM,MI,EM,EM,EM,EM,BR,BR,BR,MI,MI,EM,EM,EM,EM,EM,EM,EM},
         {NY,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,MI,BR,BR,BR,EM,EM,EM,CO,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,MI,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,MI,MI,EM,EM,MI,EM,EM,EM,EM,CO,EM,EM,OB,OB,OB,OB,OB,EM,EM,EM},
         {EM,EM,EM,BR,BR,BR,BR,BR,BR,BR,EM,EM,EM,EM,OB,OB,OB,OB,OB,OB,OB,OB,OB,EM},
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
      },

      intro = {
         {"What's that?", PLR},
         {"That's... Oh. Those are " .. WRAITHS .. ".", NYX},
         {"Looks like those are your people there.", NYX},
         {"Oh...", PLR},
         {"Don't touch them, okay?", NYX},
      },

      outro = {
         {"Alright, there it is!", NYX},
         {"Let's go!", NYX},
      },

      death = {
         {PLR .. "!", NYX},
         {"Are you okay?", NYX},
         {TWO, NYX, "Yes.", "No!",
            "Well, then get up!", "Come on, we don't have that much time!"},
      },

      goal = {{COINS}, {ABOVE_X, 23}},
   },
   {
      name = "Entrance",
      layout = {
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,OB,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB},
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,EM,EM,EM,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,EM,OB},
         {OB,OB,OB,OB,OB,OB,BR,BR,BR,BR,OB,OB,OB,OB,OB,OB},
         {EM,BR,BR,EM,BR,MI,EM,EM,CO,EM,BS,EM,BR,MI,BR,BR},
         {MI,BR,BR,MI,EM,EM,BS,EM,EM,EM,EM,EM,MI,EM,EM,BR},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,EM,MI},
         {EM,MI,EM,EM,EM,MI,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,CO,EM,EM,EM,EM,MI,EM},
         {EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM,EM},
         {EM,EM,EM,EM,EM,EM,EM,PL,EM,NY,EM,EM,EM,EM,EM,EM},
      },

      intro = {
         {"Where are the supplies?", PLR},
         {"Somewhere on the roof.", NYX},
         {"And how many floors are there?", PLR},
         {"Five.", NYX},
         {"I hope it's not filled with " .. WRAITHS .. "!", PLR},
         {"Oh, it probably is.", NYX},
         {"But see those two tall " .. WRAITHS .. "?", NYX},
         {"Unlike these " .. MINES .. " they start chasing you when you get close.", NYX},
         {"But we need to get inside...", NYX},
         {"So figure something out!", NYX},
         {"Why me?", PLR},
         {"Go!", NYX},
      },

      outro = {
         {"Wow, that was close...", NYX},
         {"Sure was!", PLR},
         {"Uhh, " .. NYX .. "?", PLR},
         {"What are those?", PLR},
         {"Stop being such a pussy!", NYX},
      },

      death = {
         {"Fall back!", NYX},
         {"Be more careful, okay?", NYX},
         {"Easier said than done!", PLR},
         {"Ugh. There must be a way to get through them.", NYX},
      },

      goal = {{COINS}, {ABOVE_Y, 9}},
   },
   {
      name = "Uninvited Guest",
      layout = {
         {OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB,OB},
         {OB,BR,BR,MI,EM,EM,EM,EM,EM,EM,EM,BR,BR,BR,BR,OB},
         {OB,BR,BR,EM,EM,EM,EM,CO,EM,EM,EM,BR,BR,BR,BR,OB},
         {OB,BR,BR,CO,EM,OB,EM,EM,EM,EM,MI,BR,BR,BR,BR,OB},
         {OB,BR,BR,EM,BR,OB,VB,EM,EM,EM,OB,OB,OB,OB,OB,OB},
         {OB,MI,EM,EM,MI,OB,EM,EM,CO,EM,HB,EM,CO,EM,EM,OB},
         {OB,EM,EM,EM,BR,OB,EM,EM,EM,BR,BR,EM,EM,EM,EM,OB},
         {OB,OB,OB,OB,OB,OB,EM,EM,EM,BR,BR,CO,OB,MI,CO,OB},
         {OB,BR,BR,EM,MI,OB,EM,EM,EM,MI,EM,EM,OB,BR,EM,OB},
         {OB,BR,BR,EM,EM,EM,EM,EM,EM,BR,EM,EM,OB,BR,EM,OB},
         {OB,EM,EM,CO,EM,EM,CO,EM,EM,EM,CO,EM,OB,BR,MI,OB},
         {OB,EM,EM,EM,HB,OB,EM,EM,EM,EM,EM,EM,OB,OB,OB,OB},
         {OB,EM,CO,EM,EM,OB,EM,EM,CO,EM,EM,VB,BR,BR,BR,OB},
         {OB,BR,BR,BR,EM,OB,EM,EM,EM,EM,EM,EM,EM,EM,BR,OB},
         {OB,BR,BR,BR,MI,OB,EM,NY,PL,EM,EM,EM,BR,MI,BR,OB},
         {OB,OB,OB,OB,OB,OB,BR,BR,BR,BR,OB,OB,OB,OB,OB,OB},
      },

      intro = {
         {"What are they, " .. NYX .. "??", PLR},
         {BATS .. ".", NYX},
         {"They can't see you. What are you even scared from?", NYX},
         {"I've never seen such beasts before...", PLR},
         {"Well, then get used to it.", NYX},
         {"Let's go!", NYX},
      },

      outro = {
         {"Alright, first floor done. Only four more to go.", NYX},
         {"Only!", PLR},
         {"Come on, it's not that bad.", NYX},
         {"Oh, looks like the staircase is barricaded.", NYX},
         {"Well...", NYX},
      },

      death = {
         {"Stop slowing me down!", NYX},
         {"Get up!", NYX},
         {"I'm trying!", PLR},
      },

      goal = {{COINS}, {AREA, 11, 2, 11, 4}},
   },
}