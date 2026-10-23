#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class MenuSceneButtons {
        PLAY = 0,
        QUIT = 1,
        PARAMS = 2,
    };

    class MenuScene : public AScene {
        public:
            MenuScene(std::string bgTexture, bool paralax);
            ~MenuScene() override;

            void update() override;
    };
}
