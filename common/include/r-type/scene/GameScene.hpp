#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {
    class GameScene : public AScene {
        public:
            GameScene(float cameraVelocity, std::string bgTexture, bool paralax);
            ~GameScene() override;

            void update() override;

        private:
            float _cameraVelocity;
    };
}
