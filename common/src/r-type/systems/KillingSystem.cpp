#include "r-type/systems/KillingSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/BonusComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/components/EntityTypeComponent.hpp"
#include "r-type/presets/BonusTypes.hpp"
#include "r-type/presets/EntitiesPresets.hpp"
#include <cstdlib>

fengine::ecs::KillingSystem::KillingSystem() {};

fengine::ecs::KillingSystem::~KillingSystem() {};

void fengine::ecs::KillingSystem::update(Registry &manager) {
    auto &defenses = manager.componentManager.getComponents<DefenseComponent>();
    auto &types = manager.componentManager.getComponents<EntityTypeComponent>();
    auto &positions = manager.componentManager.getComponents<PositionComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        auto &def = defenses[i];

        if (!def.has_value())
            continue;

        if (def.value().health <= 0) {
            std::optional<Entity> entity = manager.entityManager.entityFromIndex(i);
            if (entity.has_value()) {
                observer::SubscriberContent content(i);
                auto &type = types[i];
                if (type.has_value() && type.value().type == presets::EntityType::E_BOSS) {
                    auto &pos = positions[i];
                    fengine::ecs::Entity e = manager.entityManager.spawnEntity();

                    observer::SubscriberContent newContent(e.getId());
                    PositionComponent newPos = {pos.value().pos_x, pos.value().pos_y, true};
                    VelocityComponent newVel = {-1, 0, false};
                    BoundaryComponent newBound = {30, 30, false, true, std::nullopt};
                    AttackComponent newAtk = {{.team = team::MONSTERS}, 0, std::nullopt};
                    DefenseComponent newDef = {1, 0};
                    EntityTypeComponent newType = {presets::EntityType::BONUS};

                    manager.componentManager.addComponent(e, std::move(newPos));
                    manager.componentManager.addComponent(e, std::move(newVel));
                    manager.componentManager.addComponent(e, std::move(newBound));
                    manager.componentManager.addComponent(e, std::move(newAtk));
                    manager.componentManager.addComponent(e, std::move(newDef));
                    manager.componentManager.addComponent(e, std::move(newType));
                    auto bonusType = static_cast<presets::BonusTypes>(std::rand() % 5);
                    manager.componentManager.addComponent(e, (BonusComponent){bonusType});

                    newContent.add<PositionComponent>(newPos);
                    newContent.add<VelocityComponent>(newVel);
                    newContent.add<BoundaryComponent>(newBound);
                    newContent.add<EntityTypeComponent>(newType);
                    updateSubscribers(newContent);

                }
                DeadComponent deadComp = {entity.value().getId()};
                manager.entityManager.killEntity(entity.value());
                content.add<DeadComponent>(deadComp);
                updateSubscribers(content);
            }
        }
    }
}
