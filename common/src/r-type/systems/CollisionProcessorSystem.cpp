#include "r-type/systems/CollisionProcessorSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/BonusComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/components/EntityTypeComponent.hpp"
#include "r-type/components/PlayerBonusComponent.hpp"
#include "r-type/presets/BonusTypes.hpp"
#include "r-type/presets/EntitiesPresets.hpp"

fengine::ecs::CollisionProcessorSystem::CollisionProcessorSystem() {};

fengine::ecs::CollisionProcessorSystem::~CollisionProcessorSystem() {};

void fengine::ecs::CollisionProcessorSystem::update(Registry &manager) {
    auto &colliders = manager.componentManager.getComponents<CollisionComponent>();
    auto &defenses = manager.componentManager.getComponents<DefenseComponent>();
    auto &attacks = manager.componentManager.getComponents<AttackComponent>();
    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
    auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
    auto &types = manager.componentManager.getComponents<EntityTypeComponent>();
    auto &bonuses = manager.componentManager.getComponents<BonusComponent>();
    auto &playerBonuses = manager.componentManager.getComponents<PlayerBonusComponent>();

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
        auto &playerBonus = playerBonuses[i];
        auto &playerBonusOther = playerBonuses[j];
        collision.value() = {0, 0, false};

        if (!att.has_value() || !def.has_value() || !other_att.has_value()
            || (playerBonus.has_value() && playerBonus.value().type == presets::BonusTypes::THE_SHIELD)
            || (playerBonusOther.has_value() && playerBonusOther.value().type == presets::BonusTypes::THE_SHIELD))
            continue;

        // faire la diff ici avec le relation_id si le friendly fire est activé
        if (att.value().relation.team == other_att.value().relation.team)
            continue;

        auto &type = types[i];
        auto &otherType = types[j];

        if (type.value().type == presets::EntityType::BONUS || otherType.value().type == presets::EntityType::BONUS) {
            if (verifyBonusCollision(type.value(), otherType.value())) {
                playerBonusOther.value()._bonusDuration = std::chrono::steady_clock::now();
                playerBonusOther.value().type = bonuses[i].value().type;
                def.value().health -= (other_att.value().damage * (1 - def.value().armor / 100));
            }
            if (verifyBonusCollision(otherType.value(), type.value())) {
                playerBonus.value()._bonusDuration = std::chrono::steady_clock::now();
                playerBonus.value().type = bonuses[j].value().type;
                auto &defOther = defenses[j];
                defOther.value().health -= (att.value().damage * (1 - defOther.value().armor / 100));
            }
        } else
            def.value().health -= (other_att.value().damage * (1 - def.value().armor / 100));

        if (vel.has_value() && bound.value().bouncy) {
            observer::SubscriberContent content(i);
            vel.value() = -vel.value();
            content.add<VelocityComponent>(vel.value());
            updateSubscribers(content);
        }
    }
}

bool fengine::ecs::CollisionProcessorSystem::verifyBonusCollision(EntityTypeComponent &type,
    EntityTypeComponent &otherType) {
    return type.type == presets::EntityType::BONUS && otherType.type == presets::EntityType::PLAYERS;
}
