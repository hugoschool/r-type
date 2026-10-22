#pragma once

#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class GameScene : public AScene {
        public:
            GameScene(float cameraVelocity, std::optional<std::vector<Button>> buttons, std::string bgTexture,
                bool paralax);
            ~GameScene() override;

            void update() override;

        private:
            std::optional<std::vector<Button>> _buttons;
            Background _background;

            float _cameraVelocity;
    };
}
