#pragma once

#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class MenuScene : public AScene {
        public:
            MenuScene(std::optional<std::vector<Button>> buttons, std::string bgTexture, bool paralax)  : fengine::scene::AScene(buttons, bgTexture, paralax) {
                _background.offset = 0;
                _background.texture = bgTexture;
                _background.paralax = paralax;
            }
            ;
            ~MenuScene() override {};

            void update() override {return;};

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;
    };
}
