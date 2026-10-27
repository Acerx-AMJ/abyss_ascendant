-- Wraps a string in the given color.
function Wrap(string, color)
   return color .. string .. RESET
end

-- Constants, please do not edit.
SET_SOUND        = "$SET_SOUND"
SET_END_SOUND    = "$SET_END_SOUND"
SET_OPTION_SOUND = "$SET_OPTION_SOUND"
SKIP             = "$SKIP"

ONE       = "#1"
TWO       = "#2"
THREE     = "#3"
FOUR      = "#4"
FIVE      = "#5"
SIX       = "#6"
SEVEN     = "#7"
EIGHT     = "#8"
NINE      = "#9"

PATH1    = "&1"
PATH2    = "&2"
PATH3    = "&3"
PATH4    = "&4"
PATH5    = "&5"
PATH6    = "&6"
PATH7    = "&7"
PATH8    = "&8"
CONTINUE = "&9"

COINS      = 0
COIN_COUNT = 1
POINT      = 2
AREA       = 3
ABOVE_X    = 4
ABOVE_Y    = 5
BELOW_X   = 6
BELOW_Y   = 7
TURNS      = 8
SURVIVE    = 9
GET_HIT    = 10

-- Tiles used in the map editor.
UN = -1 -- this will overflow and print as '??' in the game.
EM = 0
CO = 1
PL = 2
NY = 3
JA = 4
GA = 5
BR = 6
OB = 7
HB = 8
VB = 9
GL = 10
SE = 11
LU = 12
WA = 13
CH = 14
BS = 15
HU = 16
MI = 17

H_BAT    = HB
V_BAT    = VB
GLIMMER  = GL
SEEKER   = SE
LURKER   = LU
WANDERER = WA
CHASER   = CH
OBSERVER = BS
HUNTER   = HU
MINE     = MI

-- You can make your own colors like this: "\027[38;2;" + RED VALUE + ";" + GREEN_VALUE
-- + ";" + BLUE_VALUE + "m" red, green and blue range is 0-255. You can create a new 
-- constant and use it in the game!
PINK       = "\027[38;2;255;128;255m"
RED        = "\027[38;2;255;0;0m"
ORANGE     = "\027[38;2;255;128;0m"
L_ORANGE   = "\027[38;2;255;192;0m"
YELLOW     = "\027[38;2;255;255;0m"
L_GREEN    = "\027[38;2;128;255;0m"
GREEN      = "\027[38;2;0;255;0m"
CYAN       = "\027[38;2;0;255;255m"
BLUE       = "\027[38;2;0;0;255m"
PURPLE     = "\027[38;2;128;0;255m"
BROWN      = "\027[38;2;139;69;19m"
BLACK      = "\027[38;2;0;0;0m"
DARK_GRAY  = "\027[38;2;64;64;64m"
GRAY       = "\027[38;2;128;128;128m"
LIGHT_GRAY = "\027[38;2;192;192;192m"
WHITE      = "\027[38;2;255;255;255m"
RESET      = "\027[0m"

-- Characters used as sender or in conversation.
NYX_ST      = Wrap("???", ORANGE)
NYX         = Wrap("Nyx", ORANGE)
JAKE_ST     = Wrap("???", L_GREEN)
JAKE        = Wrap("Jake", L_GREEN)
GABRIEL_ST  = Wrap("???", PURPLE)
GABRIEL     = Wrap("Gabriel", PURPLE)
AETHER_ST   = Wrap("???", CYAN)
AETHER      = Wrap("Aether", CYAN)
CALIGO_ST   = Wrap("???", BLACK)
CALIGO      = Wrap("Caligo", BLACK)

ABYSS       = Wrap("Abyss", RED)
WRAITHS     = Wrap("Wraiths", RED)
WRAITH      = Wrap("Wraith", RED)
BATS        = Wrap("Bats", RED)
GLIMMERS    = Wrap("Glimmers", RED)
SEEKERS     = Wrap("Seekers", RED)
CHASERS     = Wrap("Chasers", RED)
LURKERS     = Wrap("Lurkers", RED)
WANDERERS   = Wrap("Wanderers", RED)
OBSERVERS   = Wrap("Observers", RED)
HUNTERS     = Wrap("Hunters", RED)
MINES       = Wrap("Mines", RED)

-- This will be replaced in dialogue by the player's / narrator's name.
PLR = "$PLR"
NAR = "$NAR"
