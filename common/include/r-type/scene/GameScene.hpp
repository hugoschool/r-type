#pragma once

#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class GameScene : public AScene {
        public:
            GameScene(float cameraVelocity, std::optional<std::vector<Button>> buttons, std::string bgTexture,
                bool paralax) : fengine::scene::AScene(buttons, bgTexture, paralax), _cameraVelocity(cameraVelocity) {
                    _background.offset = 0;
                    _background.texture = bgTexture;
                    _background.paralax = paralax;
                };
            ~GameScene() override {};

            void update() override  {
                _background.offset += _cameraVelocity;
                if (_background.offset > WINDOW_X) {
                    _background.offset = 0;
                }
            };

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;

            float _cameraVelocity;
    };
}
