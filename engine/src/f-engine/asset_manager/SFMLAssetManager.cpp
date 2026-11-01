#include "f-engine/asset_manager/SFMLAssetManager.hpp"

fengine::asset_manager::SFMLAssetManager::SFMLAssetManager()
    : _textures(), _musics(), _shaders(), _soundBuffers(), _fonts() {
}

fengine::asset_manager::SFMLAssetManager::~SFMLAssetManager() {
}

void fengine::asset_manager::SFMLAssetManager::loadTexture(std::string filepath) {
    if (!_textures.contains(filepath)) {
        sf::Texture texture;

        if (texture.loadFromFile(filepath)) {
            _textures.insert({filepath, std::move(texture)});
        } else {
            _textures.insert({filepath, std::nullopt});
        }
    }
}

void fengine::asset_manager::SFMLAssetManager::loadMusic(std::string filepath) {
    if (!_musics.contains(filepath)) {
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        if (music->openFromFile(filepath)) {
            music->setLooping(true);
            _musics.insert({filepath, music});
        } else {
            _musics.insert({filepath, nullptr});
        }
    }
}

void fengine::asset_manager::SFMLAssetManager::loadShader(std::string filepath) {
    if (!_shaders.contains(filepath)) {
        sf::Shader shader;

        if (shader.loadFromFile(filepath, sf::Shader::Type::Fragment)) {
            _shaders.insert({filepath, std::move(shader)});
        } else {
            _shaders.insert({filepath, std::nullopt});
        }
    }
}

void fengine::asset_manager::SFMLAssetManager::loadSound(std::string filepath) {
    if (!_soundBuffers.contains(filepath)) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filepath)) {
            _soundBuffers.insert({filepath, buffer});
            sf::Sound sound(buffer);
        } else {
            _soundBuffers.insert({filepath, std::nullopt});
        }
    }
}

void fengine::asset_manager::SFMLAssetManager::loadFont(std::string filepath) {
    if (!_fonts.contains(filepath)) {
        sf::Font font;

        if (font.openFromFile(filepath)) {
            _fonts.insert({filepath, std::move(font)});
        } else {
            _fonts.insert({filepath, std::nullopt});
        }
    }
}

std::optional<sf::Texture> &fengine::asset_manager::SFMLAssetManager::getTexture(std::string filepath) {
    return _textures.at(filepath);
}

std::shared_ptr<sf::Music> fengine::asset_manager::SFMLAssetManager::getMusic(std::string filepath) {
    return _musics.at(filepath);
}

std::optional<sf::Shader> &fengine::asset_manager::SFMLAssetManager::getShader(std::string filepath) {
    return _shaders.at(filepath);
}

std::optional<sf::SoundBuffer> &fengine::asset_manager::SFMLAssetManager::getSoundBuffer(std::string filepath) {
    return _soundBuffers.at(filepath);
}

std::optional<sf::Font> &fengine::asset_manager::SFMLAssetManager::getFont(std::string filepath) {
    return _fonts.at(filepath);
}

bool fengine::asset_manager::SFMLAssetManager::textureContains(std::string filepath) {
    return _textures.contains(filepath);
}

bool fengine::asset_manager::SFMLAssetManager::musicContains(std::string filepath) {
    return _musics.contains(filepath);
}

bool fengine::asset_manager::SFMLAssetManager::shaderContains(std::string filepath) {
    return _shaders.contains(filepath);
}

bool fengine::asset_manager::SFMLAssetManager::soundContains(std::string filepath) {
    return _soundBuffers.contains(filepath);
}

bool fengine::asset_manager::SFMLAssetManager::fontContains(std::string filepath) {
    return _fonts.contains(filepath);
}
