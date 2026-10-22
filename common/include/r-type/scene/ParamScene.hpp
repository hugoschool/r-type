#pragma once

#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class ParamScene : public AScene {
        public:
            ParamScene(std::optional<std::vector<Button>> buttons, std::string bgTexture, bool paralax);
            ~ParamScene() override;

            void update() override;

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;
    };
}
