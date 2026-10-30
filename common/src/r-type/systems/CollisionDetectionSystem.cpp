#include "r-type/systems/CollisionDetectionSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "r-type/components/AttackComponent.hpp"

fengine::ecs::CollisionDetectionSystem::CollisionDetectionSystem() {};

fengine::ecs::CollisionDetectionSystem::~CollisionDetectionSystem() {};

void fengine::ecs::CollisionDetectionSystem::update(Registry &manager) {
    auto &positions = manager.componentManager.getComponents<PositionComponent>();
    auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
    auto &colliders = manager.componentManager.getComponents<CollisionComponent>();
    auto &attacks = manager.componentManager.getComponents<AttackComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &pos = positions[i];
        auto &bound = boundaries[i];
        auto &collision = colliders[i];
        auto &att = attacks[i];

        if (!pos.has_value() || !bound.has_value() || !bound.value().hittable || !att.has_value()
            || (collision.has_value() && collision.value().set))
            continue;

        for (std::size_t j = i + 1; j < manager.entityManager.size(); j++) {
            if (!manager.entityManager.entityFromIndex(j).has_value())
                continue;
            auto &pos_other = positions[j];
            auto &bound_other = boundaries[j];
            auto &att_other = attacks[j];

            if (!pos_other.has_value() || !bound_other.has_value() || !att_other.has_value()
                || att.value().relation.team == att_other.value().relation.team)
                continue;

            if (!(pos.value().pos_x < pos_other.value().pos_x + bound_other.value().size_x
                    && pos.value().pos_x + bound.value().size_x > pos_other.value().pos_x
                    && pos.value().pos_y < pos_other.value().pos_y + bound_other.value().size_y
                    && pos.value().pos_y + bound.value().size_y > pos_other.value().pos_y)) {
                continue;
            }

            std::chrono::time_point<std::chrono::steady_clock> elapsedTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> movementTime = elapsedTime
                - (bound.value().can_be_hit.has_value() ? bound.value().can_be_hit.value()
                                                        : std::chrono::steady_clock::time_point{});
            std::chrono::duration<double> movementTimeOther = elapsedTime
                - (bound_other.value().can_be_hit.has_value() ? bound_other.value().can_be_hit.value()
                                                              : std::chrono::steady_clock::time_point{});
            std::chrono::milliseconds var(100);

            if (movementTime < var || movementTimeOther < var) {
                continue;
            }
            CollisionComponent col = {i, j, true};
            CollisionComponent col_other = {j, i, true};

            manager.componentManager.addComponent(manager.entityManager.entityFromIndex(i).value(), std::move(col));
            manager.componentManager.addComponent(manager.entityManager.entityFromIndex(j).value(),
                std::move(col_other));
            bound.value().can_be_hit = std::chrono::steady_clock::now();
            bound_other.value().can_be_hit = std::chrono::steady_clock::now();

            break;
        }
    }
}
