#pragma once

#include "ISystem.hpp"
#include "f-engine/display_module/events/EventKey.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include <queue>

namespace fengine::ecs {
    class ControlSystem : public ISystem {
        public:
            ControlSystem(std::queue<modules::display::EventKey> &pqueue, std::queue<modules::display::EventKey> &rqueue): _pressedQueue(pqueue), _releasedQueue(rqueue) {};
            ~ControlSystem() override {};

            void update(Registry &manager) override {
                auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
                auto &controllables = manager.componentManager.getComponents<ControllableComponent>();

                for (std::size_t i = 0; i < velocities.size(); i++) {
                    auto &vel = velocities[i];
                    auto &control = controllables[i];

                    if (vel.has_value() && control.has_value()) {
                        if (!_pressedQueue.empty()) {
                            modules::display::EventKey &key = _pressedQueue.front();
                            if (key == modules::display::EventKey::Z)
                                vel->vel_y = -10;
                            if (key == modules::display::EventKey::Q)
                                vel->vel_x = -10;
                            if (key == modules::display::EventKey::S)
                                vel->vel_y = 10;
                            if (key == modules::display::EventKey::D)
                                vel->vel_x = 10;
                            _pressedQueue.pop();
                        }
                        if (!_releasedQueue.empty()) {
                            modules::display::EventKey &key = _releasedQueue.front();
                            if (key == modules::display::EventKey::Z)
                                vel->vel_y = 0;
                            if (key == modules::display::EventKey::Q)
                                vel->vel_x = 0;
                            if (key == modules::display::EventKey::S)
                                vel->vel_y = 0;
                            if (key == modules::display::EventKey::D)
                                vel->vel_x = 0;
                            _releasedQueue.pop();
                        }
                    }
                }
            };

        private:
            // a modifier car c'est pas beau
            std::queue<modules::display::EventKey> &_pressedQueue;
            std::queue<modules::display::EventKey> &_releasedQueue;
    };
}
