-- Include all of the variables from variables.lua
dofile("script/variables.lua")

-- This file contains all of the dialogue used for the endless mode.
Story = {
   {
      score = 0,

      intro = {
         {"Finally.", CALIGO_ST},
         {"I've been waiting for so long!", CALIGO_ST},
         {TWO, CALIGO_ST,
            "What...", "For how long?",
            "Ah, never mind.", "A long long time.",
         },
         {"Soo...", CALIGO_ST},
         {"What's your name, my friend?", CALIGO_ST},
         {THREE, CALIGO_ST,
            PLR .. ".", JAKE .. ".", AETHER .. ".",
            PATH1, PATH2, PATH3,
         },

         {PATH1, "Interesting...", CALIGO_ST},
         {PATH1, "Good luck!", CALIGO_ST},

         {PATH2, "Interesting...", CALIGO_ST},
         {PATH2, "Oh, wait...", CALIGO_ST},
         {PATH2, "You're not going to fool me.", CALIGO_ST},
         {PATH2, "What's your real name?", CALIGO_ST},
         {PATH2, THREE, CALIGO_ST,
            PLR .. ".", JAKE .. ".", AETHER .. ".",
            "Interesting...", "You're hopeless...", "Mhm...",
         },

         {PATH3, "Oh, really?", CALIGO_ST},
         {PATH3, "Good luck!", CALIGO_ST},
      },
   },
   {
      score = 2,

      intro = {
         {"Hey again, friend!", CALIGO_ST},
         {"How're you doing?", CALIGO_ST},
         {THREE, CALIGO_ST,
            "Good!", "Bad...", "Do you have no one else to talk to?",
            PATH1, PATH2, PATH3,
         },

         {PATH1, "Great to hear!", CALIGO_ST},
         {PATH1, TWO, CALIGO_ST,
            "How about you?", "This feels familiar...",
            "Doing amazing, got a new friend today!", "Hmh?",
         },
         {PATH1, "Have fun.", CALIGO_ST},

         {PATH2, "How so?", CALIGO_ST},
         {PATH2, TWO, CALIGO_ST,
            "I don't know, what about you?", "Just feeling down, that's it.",
            PATH4, "Oh...",
         },
         {PATH2, "Have fun?", CALIGO_ST},

         {PATH3, "I haven't seen anyone in months!", CALIGO_ST},
         {PATH3, THREE, CALIGO_ST,
            "How so?", "I feel bad for you...", "I think you got your time messed up.",
            "It's not easy to get in here.", "Oh, it's nothing, you're here!", "Definitely could be possible.",
         },
         {PATH3, "Hmm...", CALIGO_ST},
         {PATH3, "Have fun.", CALIGO_ST},

         {PATH4, "How can you feel bad if you don't know why?", CALIGO_ST},
         {PATH4, "I guess I'm doing fine.", CALIGO_ST},
         {PATH4, "Have fun.", CALIGO_ST},
      },
   },
   {
      score = 4,

      intro = {
         {"You'll start seeing enemies from now on.", CALIGO_ST},
         {"I've heard from your friend, " .. JAKE .. ", that you've encountered some before.", CALIGO_ST},
         {THREE, CALIGO_ST,
            "They're nothing bad.", "Yeah. They're scary!", "How do you know about " .. JAKE .. "?",
            "That's good to hear.", "Well...", PATH1,
         },

         {PATH1, "He told me a bit about a person called \"" .. PLR .. "\".", CALIGO_ST},
         {PATH1, "How's he doing?", CALIGO_ST},
         {PATH1, "Ah, never mind.", CALIGO_ST},
         {PATH1, "More urgent matter at case.", CALIGO_ST},

         {"Enemies are present from now on.", CALIGO_ST},
         {"You might recognize some of them,", CALIGO_ST},
         {"but the rest?", CALIGO_ST},
         {"They're nothing like you've ever seen before.", CALIGO_ST},
         {"Wish you the best.", CALIGO_ST},
      },
   },
   {
      score = 7,

      intro = {
         {"I see you're catching on.", CALIGO_ST},
         {"Don't get too comfortable.", CALIGO_ST},
         {"It only gets harder from here.", CALIGO_ST},
         {"Ha ha...", CALIGO_ST},
         {"I know that you'll go far!", CALIGO_ST},
         {"I mean...", CALIGO_ST},
         {"There might be something for you if you prove yourself?", CALIGO_ST},
      },
   },
   {
      score = 11,

      intro = {
         {"Picking up speed, huh?", CALIGO_ST},
         {TWO, CALIGO_ST,
            "I indeed am.", "Not really.",
            "Great!", "Oh...",
         },
         {"By the way...", CALIGO_ST},
         {"How did you get the key? Not everyone can get one so easily...", CALIGO_ST},
         {FOUR, CALIGO_ST,
            "What key???", "I got it from " .. JAKE .. ".", "I got it from a guy I saved.", NYX .. " gave it to me.",
            PATH1, PATH2, PATH3, PATH4,
         },

         {PATH1, "The one that you used to get in here...", CALIGO_ST},
         {PATH1, "Do you really have a key?", CALIGO_ST},
         {PATH1, TWO, CALIGO_ST,
            "I do.", "I don't.",
            "Then how did you get it?", "Stop lying. How else could you get in? Tell me how'd you get the key!",
         },
         {PATH1, FOUR, CALIGO_ST,
            "I have no key!", "I got it from " .. JAKE .. ".", "I got it from a guy I saved.", NYX .. " gave it to me.",
            "You really are hopeless...", PATH2, PATH3, PATH4,
         },

         {PATH2, "Oh, did he really gave his key to you?", CALIGO_ST},
         {PATH2, "I really wanted to see him again.", CALIGO_ST},
         {PATH2, TWO, CALIGO_ST,
            "He did.", "I meant that he picked it up after a guy I saved dropped it.",
            "I really really wanted to see him again...", PATH3,
         },

         {PATH4, "Hmm...", CALIGO_ST},
         {PATH4, "And who is this \"" .. Wrap("Nox", ORANGE) .. "\"?", CALIGO_ST},
         {PATH4, "Never seen them here.", CALIGO_ST},
         {PATH4, "Are you sure that they gave it to you?", CALIGO_ST},
         {PATH4, TWO, CALIGO_ST,
            "I'm sure!", "I meant that some kid gave it to me.",
            "You must be mistaken.", PATH3,
         },

         {PATH3, "Hmm...", CALIGO_ST},
         {PATH3, "Do you know his name?", CALIGO_ST},
         {PATH3, THREE, CALIGO_ST,
            "No.", "I think that it was uhh...", "He never told me.",
            PATH3, PATH3, PATH3,
         },
         {PATH3, "Okay...", CALIGO_ST},
         {PATH3, "Are you sure that he wanted you to have it?", CALIGO_ST},
         {PATH3, TWO, CALIGO_ST,
            "Uhh...", "Uhm...",
            "That's what I thought.", "...",
         },
      },
   },
   {
      score = 16,

      intro = {
         {"Please keep your distance from all of the " .. WRAITHS .. " you encounter.", CALIGO_ST},
         {"They don't play around.", CALIGO_ST},
         {THREE, CALIGO_ST,
            "What do you mean?", "Why should I avoid them?", "I've touched one before.",
            PATH1, PATH1, PATH2,
         },

         {PATH1, "They're wild beasts.", CALIGO_ST},
         {PATH1, "They will rip you apart if you get too close.", CALIGO_ST},
         {PATH1, "Nobody stands a chance against them.", CALIGO_ST},

         {PATH2, "Did you?", CALIGO_ST},
         {PATH2, "You're lucky to still be alive.", CALIGO_ST},
         {PATH2, "It better be a good lesson to not play around.", CALIGO_ST},

         {"Always keep your distance.", CALIGO_ST},
         {"It's not worth risking it all.", CALIGO_ST},
      },
   },
   {
      score = 20,

      intro = {
         {"Hmm...", CALIGO_ST},
         {"Someone like you came by today.", CALIGO_ST},
         {"He cried that he couldn't come back.", CALIGO_ST},
         {"He told me that a \"hero\" has mistaken his key for something valuable.", CALIGO_ST},
         {"The guy looked really sad, but I can't let anyone without a key in.", CALIGO_ST},
         {"I told him that everything'll be okay, that he'll meet the \"hero\" one day.", CALIGO_ST},
         {"...", CALIGO_ST},
         {"Be honest.", CALIGO_ST},
         {"Are you this \"hero\"?", CALIGO_ST},
         {THREE, CALIGO_ST,
            "I am.", "Blame " .. JAKE .. ", not me.", "I am a hero, but not this \"hero\".",
            "Did you think that he wanted you to have it?", "Was " .. JAKE .. " really involved?", "Are you lying to me??",
         },
         {TWO, CALIGO_ST,
            "Yes.", "No.",
            CONTINUE, CONTINUE,
         },
         {"I'm not going to rush you anywhere.", CALIGO_ST},
         {"But you gotta return the key to him at some point.", CALIGO_ST},
         {"What you did wasn't right, but you can fix it.", CALIGO_ST},
      },
   },
}