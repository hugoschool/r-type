#include "systems/TowerSystem.hpp"
#include "components/TowerComponent.hpp"
#include "components/UnitComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include <iostream>
#include <random>

freakytd::TowerSystem::TowerSystem(): _gen(std::default_random_engine()) {}

freakytd::TowerSystem::~TowerSystem() {}

void freakytd::TowerSystem::update(fengine::ecs::Registry &manager) {
    auto &towers = manager.componentManager.getComponents<TowerComponent>();
    auto &units = manager.componentManager.getComponents<UnitComponent>();
    auto &positions = manager.componentManager.getComponents<fengine::ecs::PositionComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;

        auto &tower = towers[i];
        auto &towerPos = positions[i];

        if (!tower.has_value() || !towerPos.has_value())
            continue;

        std::chrono::duration<double> duration = std::chrono::steady_clock::now() - tower.value().timeBeforeAttack;
        if (duration < std::chrono::milliseconds(tower.value().attackSpeed))
            continue;

        for (std::size_t j = 0; j < manager.entityManager.size(); j++) {
            if (!manager.entityManager.entityFromIndex(j).has_value())
                continue;

            if (j == i)
                continue;
            auto &pos = positions[j];
            auto &unit = units[j];

            if (!pos.has_value() || !unit.has_value())
                continue;
            if (unit.value().team == UnitTeam::ALLIES)
                continue;

            std::pair<float, float> towerCenter = std::pair<float, float>(towerPos.value().pos_x + 25, towerPos.value().pos_y + 25);

            if (((pos.value().pos_x - towerCenter.first) * (pos.value().pos_x - towerCenter.first) +
                (pos.value().pos_y - towerCenter.second) * (pos.value().pos_y - towerCenter.second)) <= (tower.value().range * tower.value().range)) {
                    // peut etre creer une entité ici se dirigeant vers l'unit pour plus de visuel
                    float damage = randomFloat(tower.value().damage);
                    std::cout << "unit " << j << " hit for " << damage << " damage" << std::endl;
                    tower.value().damageType == DamageType::PHYSICAL ? unit.value().health -= damage * (1 - unit.value().armors.first / 100)
                        : unit.value().health -= damage * (1 - unit.value().armors.second / 100);

                    tower.value().timeBeforeAttack = std::chrono::steady_clock::now();
                    break;
                }
        }
    }
}

float freakytd::TowerSystem::randomFloat(std::pair<float, float> damangeRange) {
    std::uniform_real_distribution<float> distribution(damangeRange.first, damangeRange.second);
    return distribution(_gen);
}
