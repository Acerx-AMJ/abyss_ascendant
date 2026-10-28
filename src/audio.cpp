#include "audio.hpp"
#include <thread>
#include <filesystem>
#include <random>
#include <unordered_map>

// Buffer storage
static std::unordered_map<std::string, sf::SoundBuffer> audioPool;

// Initialize the audio
void initAudio()
{
   for (const auto& file : std::filesystem::directory_iterator("sounds/"))
   {
      if (file.is_regular_file() && file.path().extension() == ".wav")
      {
         sf::SoundBuffer buffer;

         if (buffer.loadFromFile(file.path().string()))
            audioPool.insert({file.path().stem(), buffer});
      }
   }
}

// Play the give audio file
void playSound(const std::string& identifier)
{
   if (!audioPool.contains(identifier)) return;

   sf::SoundBuffer& buffer = audioPool.at(identifier);

   std::thread([&buffer]()
   {
      sf::Sound sound;
      sound.setBuffer(buffer);

      static thread_local std::random_device rd;
      static thread_local std::mt19937 generator(rd());
      std::uniform_real_distribution<float> dist(.75f, 1.25f);

      sound.setPitch(dist(generator));
      sound.play();

      // Delay the sound to let it play.
      while (sound.getStatus() == sf::Sound::Playing)
         sf::sleep(sf::milliseconds(100));
   }).detach();
}

// Delay the given audio file
void delaySound(const std::string& identifier)
{
   if (!audioPool.contains(identifier)) return;

   sf::Sound sound;
   sound.setBuffer(audioPool.at(identifier));

   static thread_local std::random_device rd;
   static thread_local std::mt19937 generator(rd());
   std::uniform_real_distribution<float> dist(.75f, 1.25f);

   sound.setPitch(dist(generator));
   sound.play();

   // Delay the sound to let it play.
   while (sound.getStatus() == sf::Sound::Playing)
      sf::sleep(sf::milliseconds(100));
}
