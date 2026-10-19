#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include <queue>

namespace fengine::ecs {
    class ControlSystem : public ASystem {
        public:
            ControlSystem(std::queue<modules::display::EventKey> &pqueue,
                std::queue<modules::display::EventKey> &rqueue);
            ~ControlSystem() override;

            void update(Registry &manager) override;

        private:
            // a modifier car c'est pas beau
            std::queue<modules::display::EventKey> &_pressedQueue;
            std::queue<modules::display::EventKey> &_releasedQueue;
    };
}
