#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class MenuSceneButtons {
        PLAY,
        QUIT,
        PARAMS,
    };

    class MenuScene : public AScene {
        public:
            MenuScene(std::string bgTexture, bool paralax);
            ~MenuScene() override;

            void update() override;
    };
}
