#pragma once

#include "f-engine/scene/AScene.hpp"
#include <cstddef>

namespace fengine::scene {
    class GameScene : public AScene {
        public:
            GameScene(float cameraVelocity, std::string bgTexture, bool paralax, std::size_t &level);
            ~GameScene() override;

            void update() override;

        private:
            float _cameraVelocity;
            std::size_t &_level;
    };
}
