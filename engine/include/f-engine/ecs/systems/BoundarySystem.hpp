#pragma once

#include "ISystem.hpp"
#include "f-engine/display_module/window/SFMLWindow.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

namespace fengine {
    namespace ecs {
        class BoundarySystem : public ISystem {
            public:
                BoundarySystem() {};
                ~BoundarySystem() override {};

                void update(Registry &manager) override {
                    auto &positions = manager.componentManager.getComponents<PositionComponent>();
                    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();

                    for (std::size_t i = 0; i < positions.size(); i++) {
                        auto &pos = positions[i];
                        auto &vel = velocities[i];

                        if (pos.has_value() && vel.has_value()) {
                            if (pos.value().pos_x > WINDOW_X || pos.value().pos_x < 0)
                                vel.value().vel_x = -vel.value().vel_x;
                            if (pos.value().pos_y > WINDOW_Y || pos.value().pos_y < 0)
                                vel.value().vel_y = -vel.value().vel_y;
                        }
                    }
                };
        };
    }
}
