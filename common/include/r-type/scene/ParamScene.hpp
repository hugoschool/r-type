#pragma once

#include "f-engine/scene/AScene.hpp"

namespace fengine::scene {

    enum class ParamSceneButtons {
        MENU,
        COLORBLIND_1,
        COLORBLIND_2,
        COLORBLIND_3,
    };

    class ParamScene : public AScene {
        public:
            ParamScene(std::string bgTexture, bool paralax);
            ~ParamScene() override;

            void update() override;
    };
}
