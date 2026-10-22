#include "r-type/systems/CollisionDamageSystem.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"

fengine::ecs::CollisionDamageSystem::CollisionDamageSystem() {};

fengine::ecs::CollisionDamageSystem::~CollisionDamageSystem() {};

void fengine::ecs::CollisionDamageSystem::update(Registry &manager) {
    auto &colliders = manager.componentManager.getComponents<CollisionComponent>();
    auto &defenses = manager.componentManager.getComponents<DefenseComponent>();
    auto &attacks = manager.componentManager.getComponents<AttackComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        auto &collision = colliders[i];

        if (!collision.has_value())
            continue;

        if (collision.value().self_id == collision.value().other_id)
            continue;

        std::size_t j = collision.value().other_id;
        auto &att = attacks[i];
        auto &def = defenses[i];
        auto &other_att = attacks[j];
        collision.value() = {0, 0, false};

        if (!att.has_value() || !def.has_value() || !other_att.has_value())
            continue;

        // faire la diff ici avec le relation_id si le friendly fire est activé
        if (att.value().relation.team == other_att.value().relation.team)
            continue;

        def.value().health -= (other_att.value().damage * (1 - def.value().armor / 100));

        observer::SubscriberContent content(i);

        content.add<DefenseComponent>(def.value());
        content.add<CollisionComponent>(collision.value());

        updateSubscribers(content);
    }
}
