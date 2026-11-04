#include "r-type/systems/BehaviouralSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/BehaviourComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/components/EntityTypeComponent.hpp"
#include "r-type/presets/EntitiesPresets.hpp"

fengine::ecs::BehaviouralSystem::BehaviouralSystem() {};

fengine::ecs::BehaviouralSystem::~BehaviouralSystem() {};

void fengine::ecs::BehaviouralSystem::update(Registry &manager) {
    auto &positions = manager.componentManager.getComponents<PositionComponent>();
    auto &behaviours = manager.componentManager.getComponents<BehaviourComponent>();
    auto &attacks = manager.componentManager.getComponents<AttackComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &pos = positions[i];
        auto &atk = attacks[i];
        auto &behav = behaviours[i];

        if (pos.has_value() && atk.has_value() && behav.has_value()) {
            std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> behavTime = elapsedTime - behav.value().behaviourClock;
            std::chrono::milliseconds var(static_cast<int>(behav.value().behaviour));

            if (behavTime < var)
                continue;

            fengine::ecs::Entity e = manager.entityManager.spawnEntity();

            observer::SubscriberContent content(e.getId());
            PositionComponent newPos = {pos.value().pos_x, pos.value().pos_y, true};
            VelocityComponent newVel = {-30, 0, false};
            BoundaryComponent newBound = {10, 5, false, true, std::nullopt};
            AttackComponent newAtk = {atk.value().relation, atk.value().damage};
            DefenseComponent newDef = {1, 0};
            EntityTypeComponent newType = {presets::EntityType::BULLETS};

            manager.componentManager.addComponent(e, std::move(newPos));
            manager.componentManager.addComponent(e, std::move(newVel));
            manager.componentManager.addComponent(e, std::move(newBound));
            manager.componentManager.addComponent(e, std::move(newAtk));
            manager.componentManager.addComponent(e, std::move(newDef));
            manager.componentManager.addComponent(e, std::move(newType));

            content.add<PositionComponent>(newPos);
            content.add<VelocityComponent>(newVel);
            content.add<BoundaryComponent>(newBound);
            content.add<EntityTypeComponent>(newType);
            updateSubscribers(content);
            behav.value().behaviourClock = std::chrono::steady_clock::now();
        }
    }
}
