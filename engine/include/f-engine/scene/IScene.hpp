#pragma once

#include "f-engine/scene/SceneObjects.hpp"
#include <map>

namespace fengine::scene {
    // Only the IScene and the Scene Objects should be in the engine
    class IScene {
        public:
            virtual ~IScene() = default;

            virtual std::map<int, Button> &getButtons() = 0;
            virtual Background &getBackground() = 0;

            virtual void update() = 0;

            virtual void updateButtons(std::pair<float, float> &) = 0;

            virtual bool empty() = 0;
            virtual int pollEvents() = 0;
    };
}
