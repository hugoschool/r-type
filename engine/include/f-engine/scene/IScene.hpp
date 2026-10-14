#pragma once

#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    class IScene {
        public:
            virtual ~IScene() = default;

            virtual std::optional<std::vector<Button>> &getButtons() = 0;
            virtual Background &getBackground() = 0;

            virtual void update() = 0;
    };
}
