#pragma once

#include "SFML/Audio/Sound.hpp"
#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include "f-engine/modules/sound/ISoundModule.hpp"
#include <SFML/Audio.hpp>
#include <map>
#include <string>

namespace fengine::modules::sound {
    class SFMLSoundModule : public ISoundModule {
        public:
            SFMLSoundModule(std::shared_ptr<asset_manager::SFMLAssetManager> assetManager);
            ~SFMLSoundModule() override;

            void playSound(std::string) override;
            void playMusic(std::string) override;
            void stopMusic(std::string) override;

        private:
            std::vector<std::string> _musicVec;
            std::map<std::string, std::optional<sf::SoundBuffer>> _soundBufferMap;
            std::map<std::string, std::optional<sf::Sound>> _soundMap;

            std::shared_ptr<asset_manager::SFMLAssetManager> _assetManager;

            std::size_t _tempindex;
    };
}
