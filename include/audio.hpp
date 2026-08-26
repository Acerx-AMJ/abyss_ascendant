#ifndef AA_AUDIO_HPP
#define AA_AUDIO_HPP

#include <SFML/Audio.hpp>
#include "syntax.hpp"

// Initialize the audio
void initAudio();

// Play the given audio file
void playSound(const str& identifier);
void delaySound(const str& identifier);

#endif // AA_AUDIO_HPP
