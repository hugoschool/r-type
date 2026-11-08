#include "systems/EnnemyPathfindingSystem.hpp"
#include "components/UnitComponent.hpp"
#include "Orchestrator.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

freakytd::EnnemyPathfindingSystem::EnnemyPathfindingSystem(Path &path, int &lives): _path(path), _lives(lives) {}

freakytd::EnnemyPathfindingSystem::~EnnemyPathfindingSystem() {}

void freakytd::EnnemyPathfindingSystem::update(fengine::ecs::Registry &manager) {
    auto &units = manager.componentManager.getComponents<UnitComponent>();
    auto &velocities = manager.componentManager.getComponents<fengine::ecs::VelocityComponent>();
    auto &positions = manager.componentManager.getComponents<fengine::ecs::PositionComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;

        auto &unit = units[i];
        auto &pos = positions[i];
        auto &vel = velocities[i];

        if (!unit.has_value() || !pos.has_value() || !vel.has_value())
            continue;
        if (unit.value().team != UnitTeam::ENNEMIES)
            continue;

        vel.value().vel_x = 0;
        vel.value().vel_y = 0;

        auto &tiles = _path.getTiles();
        for (std::size_t j = 0; j < tiles.size(); j++) {
            float tileX = tiles[j].getX() * 50;
            float tileY = tiles[j].getY() * 50;
            bool inTile = (tileX < pos.value().pos_x + ENNEMY_SIZE
                            && tileX + 50 > pos.value().pos_x
                            && tileY < pos.value().pos_y + ENNEMY_SIZE
                            && tileY + 50 > pos.value().pos_y);

            if (!inTile)
                continue;

            if (j == tiles.size() - 1) {
                _lives -= 1;
                unit.value().health = 0;
                break;
            }

            float nextX = tiles[j + 1].getX() * 50;
            float nextY = tiles[j + 1].getY() * 50;

            if (nextX > tileX)
                vel.value().vel_x = unit.value().speed;
            else if (nextX < tileX)
                vel.value().vel_x = -unit.value().speed;
            if (nextY > tileY)
                vel.value().vel_y = unit.value().speed;
            else if (nextY < tileY)
                vel.value().vel_y = -unit.value().speed;
            break;
        }
    }
}
