#pragma once

#include <string>

namespace fengine::modules::sound {
    class ISoundModule {
        public:
            virtual ~ISoundModule() = default;

            virtual void playSound(std::string) = 0;
            virtual void playMusic(std::string) = 0;
            virtual void stopMusic(std::string) = 0;
    };
}