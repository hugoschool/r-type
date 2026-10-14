#pragma once

#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include "f-engine/scene/IScene.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class GameScene: public IScene {
        public:
            GameScene(std::optional<std::vector<Button>> buttons, std::string bgTexture): _buttons(std::nullopt), _background() {
                if (buttons.has_value()) {
                    _buttons.reset();
                    for (auto &button: buttons.value()) {
                        _buttons.value().push_back(button);
                    }
                }
                _background.offset = 0;
                _background.texture = bgTexture;
            };
            ~GameScene() {};

            std::optional<std::vector<Button>> &getButtons() override {
                return _buttons;
            };

            Background &getBackground() override {
                return _background;
            };

            void update() override {
                _background.offset += 10;
                if (_background.offset > WINDOW_X) {
                    _background.offset = 0;
                }
            };

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;
    };
}