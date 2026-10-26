#pragma once

#include "SFML/Audio/Sound.hpp"
#include "f-engine/modules/sound/ISoundModule.hpp"
#include <SFML/Audio.hpp>
#include <map>
#include <string>

namespace fengine::modules::sound {
    class SFMLSoundModule: public ISoundModule {
        public:
            SFMLSoundModule();
            ~SFMLSoundModule() override;

            void playSound(std::string) override;
            void playMusic(std::string) override;
            void stopMusic(std::string) override;
        private:
            std::map<std::string, std::unique_ptr<sf::Music>> _musicMap;
            std::map<std::string, std::optional<sf::SoundBuffer>> _soundBufferMap;
            std::map<std::string, std::optional<sf::Sound>> _soundMap;
    };
}