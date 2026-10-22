#pragma once

#include "f-engine/scene/IScene.hpp"
namespace fengine::scene {
    class AScene: public IScene {
        public:
            AScene(std::optional<std::vector<Button>> buttons, std::string bgTexture, bool paralax);
            ~AScene() override;

            std::optional<std::vector<Button>> &getButtons() override;

            Background &getBackground() override;
        protected:
            std::optional<std::vector<Button>> _buttons;
            Background _background;
    };
}
