#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class ParamSceneButtons {
        MENU = 3,
        COLORBLIND_1 = 4,
        COLORBLIND_2 = 5,
        COLORBLIND_3 = 6,
    };

    class ParamScene : public AScene {
        public:
            ParamScene(std::string bgTexture, bool paralax);
            ~ParamScene() override;

            void update() override;
    };
}
