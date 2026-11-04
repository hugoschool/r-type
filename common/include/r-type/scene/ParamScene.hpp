#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class ParamSceneButtons {
        MENU = 6,
        COLORBLIND_1 = 7,
        COLORBLIND_2 = 5,
        COLORBLIND_3 = 4,
        TTS = 3,
    };

    class ParamScene : public AScene {
        public:
            ParamScene(std::string bgTexture, bool paralax);
            ~ParamScene() override;

            void update() override;
    };
}
