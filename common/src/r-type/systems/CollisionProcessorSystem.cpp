#include "r-type/systems/CollisionProcessorSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"

fengine::ecs::CollisionProcessorSystem::CollisionProcessorSystem() {};

fengine::ecs::CollisionProcessorSystem::~CollisionProcessorSystem() {};

void fengine::ecs::CollisionProcessorSystem::update(Registry &manager) {
    auto &colliders = manager.componentManager.getComponents<CollisionComponent>();
    auto &defenses = manager.componentManager.getComponents<DefenseComponent>();
    auto &attacks = manager.componentManager.getComponents<AttackComponent>();
    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
    auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        auto &collision = colliders[i];

        if (!collision.has_value())
            continue;

        if (collision.value().self_id == collision.value().other_id)
            continue;

        std::size_t j = collision.value().other_id;
        auto &att = attacks[i];
        auto &vel = velocities[i];
        auto &def = defenses[i];
        auto &bound = boundaries[i];
        auto &other_att = attacks[j];
        collision.value() = {0, 0, false};

        if (!att.has_value() || !def.has_value() || !other_att.has_value())
            continue;

        // faire la diff ici avec le relation_id si le friendly fire est activé
        if (att.value().relation.team == other_att.value().relation.team)
            continue;

        def.value().health -= (other_att.value().damage * (1 - def.value().armor / 100));

        if (vel.has_value() && bound.value().bouncy) {
            observer::SubscriberContent content(i);
            vel.value() = -vel.value();
            content.add<VelocityComponent>(vel.value());
            updateSubscribers(content);
        }
    }
}
