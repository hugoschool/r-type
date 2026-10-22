#pragma once

// #include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <optional>
#include <vector>

namespace fengine::scene {
    // Only the IScene and the Scene Objects should be in the engine
    class IScene {
        public:
            virtual ~IScene() = default;

            virtual std::optional<std::vector<Button>> &getButtons() = 0;
            virtual Background &getBackground() = 0;

            virtual void update() = 0;

            // virtual void updateButtons(modules::display::EventMouseButton &) = 0;
    };
}
