#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

namespace fengine::ecs {
    class MovementSystem : public ISystem {
        public:
            MovementSystem() {};
            ~MovementSystem() override {};

            void update(Registry &manager) override {
                auto &positions = manager.componentManager.getComponents<PositionComponent>();
                auto &velocities = manager.componentManager.getComponents<VelocityComponent>();

                for (std::size_t i = 0; i < positions.size(); i++) {
                    auto &pos = positions[i];
                    auto &vel = velocities[i];

                    if (pos.has_value() && vel.has_value()) {
                        pos.value().pos_x += vel.value().vel_x;
                        pos.value().pos_y += vel.value().vel_y;
                    }
                }
            };
    };
}
