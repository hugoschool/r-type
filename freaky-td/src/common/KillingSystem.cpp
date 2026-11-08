#include "systems/KillingSystem.hpp"
#include "components/UnitComponent.hpp"

freakytd::KillingSystem::KillingSystem(int &money): _money(money) {}

freakytd::KillingSystem::~KillingSystem() {}

void freakytd::KillingSystem::update(fengine::ecs::Registry &manager) {
    auto &units = manager.componentManager.getComponents<UnitComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;

        auto &unit = units[i];

        if (!unit.has_value())
            continue;

        if (unit.value().health <= 0) {
            if (unit.value().team == UnitTeam::ENNEMIES) {
                _money += unit.value().value;
            }
            manager.entityManager.killEntity(manager.entityManager.entityFromIndex(i).value());
        }
    }
}
