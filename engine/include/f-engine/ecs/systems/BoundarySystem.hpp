#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AttackComponent.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/DefenseComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

namespace fengine::ecs {
    class BoundarySystem : public ISystem {
        public:
            BoundarySystem() {};
            ~BoundarySystem() override {};

            void update(Registry &manager) override {
                auto &positions = manager.componentManager.getComponents<PositionComponent>();
                auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
                auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
                auto &attacks = manager.componentManager.getComponents<AttackComponent>();
                auto &defenses = manager.componentManager.getComponents<DefenseComponent>();

                for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
                    if (!manager.entityManager.entityFromIndex(i).has_value())
                        continue;
                    auto &pos = positions[i];
                    auto &bound = boundaries[i];

                    if (!pos.has_value() || !bound.has_value() || !bound.value().hittable)
                        continue;

                    auto &vel = velocities[i];
                    auto &att = attacks[i];
                    auto &def = defenses[i];

                    for (std::size_t j = 0; j < manager.entityManager.size(); j++) {
                        if (!manager.entityManager.entityFromIndex(j).has_value())
                            continue;
                        auto &pos_other = positions[j];
                        auto &bound_other = boundaries[j];

                        // ajouter un timer pour pas juste se faire one shot
                        if (j == i || !pos_other.has_value() || !bound_other.has_value())
                            continue;

                        if (!(pos.value().pos_x < pos_other.value().pos_x + bound_other.value().size_x &&
                            pos.value().pos_x + bound.value().size_x > pos_other.value().pos_x &&
                            pos.value().pos_y < pos_other.value().pos_y + bound_other.value().size_y &&
                            pos.value().pos_y + bound.value().size_y > pos_other.value().pos_y)) {
                                continue;
                        }
                        auto &vel_other = velocities[j];
                        auto &att_other = attacks[j];
                        auto &def_other = defenses[j];

                        std::chrono::time_point<std::chrono::steady_clock> elapsedTime = std::chrono::steady_clock::now();
                        std::chrono::duration<double> movementTime = elapsedTime - (bound.value().can_be_hit.has_value() ? bound.value().can_be_hit.value() : std::chrono::steady_clock::time_point{});
                        std::chrono::duration<double> movementTimeOther = elapsedTime - (bound_other.value().can_be_hit.has_value() ? bound_other.value().can_be_hit.value() : std::chrono::steady_clock::time_point{});
                        std::chrono::milliseconds var(100);
                        // faire la diff ici avec le relation_id si le friendly fire est activé
                        if (att.value().relation.team == att_other.value().relation.team || movementTime < var || movementTimeOther < var) {
                            continue;
                        }
                        bound.value().can_be_hit.reset();
                        bound_other.value().can_be_hit.reset();
                        vel.value() = -vel.value();
                        vel_other.value() = -vel_other.value();
                        def.value().health -= (att_other.value().damage * (1 - def.value().armor / 100));
                        def_other.value().health -= (att.value().damage * (1 - def_other.value().armor / 100));
                        bound.value().can_be_hit = std::chrono::steady_clock::now();
                        bound_other.value().can_be_hit = std::chrono::steady_clock::now();
                    }

                    if (def.value().health <= 0) {
                        std::optional<Entity> entity = manager.entityManager.entityFromIndex(i);
                            if (entity.has_value()) {
                                manager.entityManager.killEntity(entity.value());
                            }
                    }
                }
            };
    };
}
