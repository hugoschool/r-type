#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class MenuSceneButtons {
        PARAMS = 0,
        PLAY = 1,
        QUIT = 2,
    };

    class MenuScene : public AScene {
        public:
            MenuScene(std::string bgTexture, bool paralax);
            ~MenuScene() override;

            void update() override;
    };
}
