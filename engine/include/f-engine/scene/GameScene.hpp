#pragma once

#include "f-engine/scene/IScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class GameScene : public IScene {
        public:
            GameScene(float cameraVelocity, std::optional<std::vector<Button>> buttons, std::string bgTexture,
                bool paralax);
            ~GameScene() override;

            std::optional<std::vector<Button>> &getButtons() override;

            Background &getBackground() override;

            void update() override;

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;

            float _cameraVelocity;
    };
}
