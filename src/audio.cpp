#include "audio.hpp"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <list>
#include <random>
#include <unordered_map>

static std::unordered_map<std::string, sf::SoundBuffer> audioPool;
static std::list<sf::Sound> activeSounds;

void initAudio() {
   for (const auto &file: std::filesystem::directory_iterator("sounds/")) {
      if (!file.is_regular_file() || file.path().extension() != ".wav") {
         continue;
      }

      sf::SoundBuffer buffer;
      if (buffer.loadFromFile(file.path().string())) {
         audioPool.insert({file.path().stem(), buffer});
      }
   }
}

void playSound(const std::string &identifier) {
   auto it = audioPool.find(identifier);
   if (it == audioPool.end()) {
      return;
   }

   activeSounds.remove_if([](const sf::Sound &s) -> bool {
      return s.getStatus() == sf::Sound::Stopped;
   });

   static thread_local std::mt19937 rng(std::random_device{}());
   std::uniform_real_distribution<float> dist(0.75f, 1.25f);

   activeSounds.emplace_back(it->second);
   activeSounds.back().setPitch(dist(rng));
   activeSounds.back().play();
}
