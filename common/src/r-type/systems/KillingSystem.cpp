#include "r-type/systems/KillingSystem.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"

fengine::ecs::KillingSystem::KillingSystem() {};

fengine::ecs::KillingSystem::~KillingSystem() {};

void fengine::ecs::KillingSystem::update(Registry &manager) {
    auto &defenses = manager.componentManager.getComponents<DefenseComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        auto &def = defenses[i];

        if (!def.has_value())
            continue;

        if (def.value().health <= 0) {
            observer::SubscriberContent content(i);
            std::optional<Entity> entity = manager.entityManager.entityFromIndex(i);
            if (entity.has_value()) {
                DeadComponent deadComp = {entity.value().getId()};
                manager.entityManager.killEntity(entity.value());
                content.add<DeadComponent>(deadComp);
                updateSubscribers(content);
            }
        }
    }
}
