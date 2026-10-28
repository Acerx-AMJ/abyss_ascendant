#ifndef AA_AUDIO_HPP
#define AA_AUDIO_HPP

#include <SFML/Audio.hpp>

// Initialize the audio
void initAudio();

// Play the given audio file
void playSound(const std::string& identifier);
void delaySound(const std::string& identifier);

#endif // AA_AUDIO_HPP
