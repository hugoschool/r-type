#include "f-engine/modules/sound/SFMLSoundModule.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include <string>

fengine::modules::sound::SFMLSoundModule::SFMLSoundModule(): _musicMap(), _soundBufferMap(), _soundMap() {}

fengine::modules::sound::SFMLSoundModule::~SFMLSoundModule() {}

void fengine::modules::sound::SFMLSoundModule::playSound(std::string filepath) {
    // tempindex is a value allowing to play the same sound more than once at a time
    static std::size_t tempindex = 0;

    if (!_soundBufferMap.contains(filepath)) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filepath)) {
            _soundBufferMap.insert({filepath, buffer});
            sf::Sound sound(buffer);
            _soundMap.insert({filepath, sound});
            _soundMap.at(filepath).value().setBuffer(_soundBufferMap.at(filepath).value());
            _soundMap.at(filepath).value().setVolume(50);
        } else {
            _soundBufferMap.insert({filepath, std::nullopt});
            _soundMap.insert({filepath, std::nullopt});
        }
    }

    if (_soundMap.at(filepath).has_value()) {
        auto status = _soundMap.at(filepath).value().getStatus();
        if (status == sf::Sound::Status::Playing) {
            sf::Sound sound(_soundMap.at(filepath).value().getBuffer());
            _soundMap.insert({"temp" + std::to_string(tempindex), sound});
            _soundMap["temp" + std::to_string(tempindex)].value().setBuffer(_soundBufferMap.at(filepath).value());
            _soundMap["temp" + std::to_string(tempindex)].value().play();
            tempindex++;
        }
        _soundMap.at(filepath).value().play();
    }
}

void fengine::modules::sound::SFMLSoundModule::playMusic(std::string filepath) {
    if (!_musicMap.contains(filepath)) {
        std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
        if (music->openFromFile(filepath)) {
            music->setLooping(true);
            _musicMap.insert({filepath, std::move(music)});
        }
    }

    for (auto &[path, music]: _musicMap) {
        if (path != filepath && music && (music->getStatus() == sf::Music::Status::Playing)) {
            music->stop();
        }
    }

    if (_musicMap.at(filepath)) {
        _musicMap.at(filepath)->play();
    }
}

void fengine::modules::sound::SFMLSoundModule::stopMusic(std::string filepath) {
    if (_musicMap.at(filepath)) {
        _musicMap.at(filepath)->stop();
    }
}
