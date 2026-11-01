#pragma once

#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "f-engine/asset_manager/IAssetManager.hpp"
#include <map>
#include <string>
namespace fengine::asset_manager {
    class SFMLAssetManager: public IAssetManager {
        public:
            SFMLAssetManager();
            ~SFMLAssetManager() override;

            void loadTexture(std::string filepath);
            void loadMusic(std::string filepath);
            void loadShader(std::string filepath);
            void loadSound(std::string filepath);
            void loadFont(std::string filepath);

            std::optional<sf::Texture> &getTexture(std::string filepath);
            std::shared_ptr<sf::Music> getMusic(std::string filepath);
            std::optional<sf::Shader> &getShader(std::string filepath);
            std::optional<sf::SoundBuffer> &getSoundBuffer(std::string filepath);
            std::optional<sf::Font> &getFont(std::string filepath);

            bool textureContains(std::string filepath);
            bool musicContains(std::string filepath);
            bool shaderContains(std::string filepath);
            bool soundContains(std::string filepath);
            bool fontContains(std::string filepath);

        private:
            std::map<std::string, std::optional<sf::Texture>> _textures;
            std::map<std::string, std::shared_ptr<sf::Music>> _musics;
            std::map<std::string, std::optional<sf::Shader>> _shaders;
            std::map<std::string, std::optional<sf::SoundBuffer>> _soundBuffers;
            std::map<std::string, std::optional<sf::Font>> _fonts;
    };
}
