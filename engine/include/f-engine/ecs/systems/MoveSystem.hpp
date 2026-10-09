#pragma once

#include "ISystem.hpp"
#include "f-engine/display_module/window/SFMLWindow.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
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
                auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
                BoundaryComponent boundary = {0, 0, false, std::nullopt};

                for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
                    if (!manager.entityManager.entityFromIndex(i).has_value())
                        continue;
                    auto &pos = positions[i];
                    auto &vel = velocities[i];
                    auto &bound = boundaries[i];

                    if (!pos.has_value() || !vel.has_value())
                        continue;

                    if (bound.has_value()) {
                        boundary = {bound.value().size_x, bound.value().size_y, bound.value().hittable,
                            bound.value().can_be_hit};
                    } else {
                        boundary = {0, 0, false, std::nullopt};
                    }

                    if (!pos.value().can_be_out_window) {
                        if (pos.value().pos_x > WINDOW_X - boundary.size_x || pos.value().pos_x < 0)
                            vel.value().vel_x = -vel.value().vel_x;
                        if (pos.value().pos_y > WINDOW_Y - boundary.size_y || pos.value().pos_y < 0)
                            vel.value().vel_y = -vel.value().vel_y;
                    }
                    if (pos.value().can_be_out_window) {
                        // should be changed so that a message is sent to client to say that the entity is killed instead of deleting here (maybe ?)
                        // also handle this differently so that entities can come from any corner of the window without getting slimed out instantly
                        if (pos.value().pos_x > WINDOW_X || pos.value().pos_y > WINDOW_Y || pos.value().pos_x < 0
                            || pos.value().pos_y < 0) {
                            std::optional<Entity> entity = manager.entityManager.entityFromIndex(i);
                            if (entity.has_value())
                                manager.entityManager.killEntity(entity.value());
                        }
                    }

                    pos.value().pos_x += vel.value().vel_x;
                    pos.value().pos_y += vel.value().vel_y;
                }
            };
    };
}
