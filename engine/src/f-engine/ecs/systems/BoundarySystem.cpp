#include "f-engine/ecs/systems/BoundarySystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

fengine::ecs::BoundarySystem::BoundarySystem() {};

fengine::ecs::BoundarySystem::~BoundarySystem() {};

void fengine::ecs::BoundarySystem::update(Registry &manager) {
    auto &positions = manager.componentManager.getComponents<PositionComponent>();
    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
    auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
    auto &colliders = manager.componentManager.getComponents<CollisionComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &pos = positions[i];
        auto &bound = boundaries[i];
        auto &collision = colliders[i];

        if (!pos.has_value() || !bound.has_value() || !bound.value().hittable || (collision.has_value() && collision.value().set))
            continue;

        auto &vel = velocities[i];

        observer::SubscriberContent content(i);
        for (std::size_t j = i + 1; j < manager.entityManager.size(); j++) {
            if (!manager.entityManager.entityFromIndex(j).has_value())
                continue;
            auto &pos_other = positions[j];
            auto &bound_other = boundaries[j];

            if (!pos_other.has_value() || !bound_other.has_value())
                continue;

            if (!(pos.value().pos_x < pos_other.value().pos_x + bound_other.value().size_x
                    && pos.value().pos_x + bound.value().size_x > pos_other.value().pos_x
                    && pos.value().pos_y < pos_other.value().pos_y + bound_other.value().size_y
                    && pos.value().pos_y + bound.value().size_y > pos_other.value().pos_y)) {
                continue;
            }
            auto &vel_other = velocities[j];

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
            observer::SubscriberContent content_other(j);
            content.add<CollisionComponent>(col);
            content_other.add<CollisionComponent>(col_other);
            manager.componentManager.addComponent(manager.entityManager.entityFromIndex(i).value(), std::move(col));
            manager.componentManager.addComponent(manager.entityManager.entityFromIndex(j).value(), std::move(col_other));
            if (vel.has_value() && bound.value().bouncy)
                vel.value() = -vel.value();
            if (vel_other.has_value() && bound_other.value().bouncy)
                vel_other.value() = -vel_other.value();
            bound.value().can_be_hit = std::chrono::steady_clock::now();
            bound_other.value().can_be_hit = std::chrono::steady_clock::now();

            content_other.add<VelocityComponent>(vel_other.value());
            content_other.add<BoundaryComponent>(bound_other.value());
            updateSubscribers(content_other);
            break;
        }

        content.add<VelocityComponent>(vel.value());
        content.add<BoundaryComponent>(bound.value());
        updateSubscribers(content);
    }
}
