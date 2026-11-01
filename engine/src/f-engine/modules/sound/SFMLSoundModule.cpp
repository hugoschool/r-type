#include "f-engine/modules/sound/SFMLSoundModule.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include <memory>
#include <optional>
#include <string>

fengine::modules::sound::SFMLSoundModule::SFMLSoundModule(std::shared_ptr<fengine::asset_manager::SFMLAssetManager> assetManager)
    : _musicVec(), _soundBufferMap(), _soundMap(), _assetManager(assetManager), _tempindex(0) {
}

fengine::modules::sound::SFMLSoundModule::~SFMLSoundModule() {
}

void fengine::modules::sound::SFMLSoundModule::playSound(std::string filepath) {
    if (!_assetManager->soundContains(filepath)) {
        _assetManager->loadSound(filepath);
    }

    std::optional<sf::SoundBuffer> &buffer = _assetManager->getSoundBuffer(filepath);
    if (!buffer.has_value()) {
        return;
    }

    if (!_soundMap.contains(filepath) || !_soundMap.at(filepath).has_value()) {
        _soundMap.emplace(filepath, buffer.value());
        _soundMap.at(filepath).value().setVolume(50);
    }

    sf::Sound &sound = _soundMap.at(filepath).value();
    if (sound.getStatus() == sf::Sound::Status::Playing) {
        std::string tempKey = "temp" + std::to_string(_tempindex);
        _soundMap.emplace(tempKey, buffer.value());
        _soundMap.at(tempKey).value().setVolume(50);
        _soundMap.at(tempKey).value().play();
        _tempindex++;
    }
    sound.play();
}

void fengine::modules::sound::SFMLSoundModule::playMusic(std::string filepath) {
    if (!_assetManager->musicContains(filepath)) {
        _assetManager->loadMusic(filepath);
        _musicVec.push_back(filepath);
    }

    for (auto &filepath : _musicVec) {
        stopMusic(filepath);
    }

    if (_assetManager->musicContains(filepath)) {
        std::shared_ptr<sf::Music> music = _assetManager->getMusic(filepath);
        if (music) {
            music->play();
        }
    }
}

void fengine::modules::sound::SFMLSoundModule::stopMusic(std::string filepath) {
    if (!_assetManager->musicContains(filepath)) {
        return;
    }

    std::shared_ptr<sf::Music> music = _assetManager->getMusic(filepath);
    if (music) {
        music->stop();
    }
}
